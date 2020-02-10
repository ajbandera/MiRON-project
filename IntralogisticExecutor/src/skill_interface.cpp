#define NOSENDING 0

#include <fstream>
#include <behaviortree_cpp_v3/bt_factory.h>

#include "Intralogistic/skill_interface.hpp"
#include "Intralogistic/json.hpp"

//1. ApproachLocation_action

ApproachLocation_action::ApproachLocation_action(SkillDefinition definition,
                         const std::string &name,
			 const BT::NodeConfiguration& config,
                         const char* ip,
                         zmq::context_t& context):
    BT::SyncActionNode(name, config),
    _definition(std::move(definition)),
    _request_socket( context, ZMQ_REQ ),
    _reply_socket( context, ZMQ_SUB  ),
    _current_uid(0)
{

    sprintf(_request_address, "tcp://%s:5557", ip );
    _request_socket.connect( _request_address );

    int timeout_ms = 500;
    int linger_ms = 0;
    _request_socket.setsockopt(ZMQ_SNDTIMEO, timeout_ms);
    _request_socket.setsockopt(ZMQ_RCVTIMEO, timeout_ms);
    _request_socket.setsockopt(ZMQ_LINGER, linger_ms);

    sprintf( _reply_address, "tcp://%s:5558", ip );
    _reply_socket.connect( _reply_address );
    _reply_socket.setsockopt(ZMQ_SUBSCRIBE, "", 0);
}

ApproachLocation_action::~ApproachLocation_action()
{
    _request_socket.disconnect(_request_address);
    _reply_socket.disconnect(_reply_address);
}

BT::NodeStatus ApproachLocation_action::tick()
{
    std::cout << "[ TICK: OK ]" << std::endl; //ABR

    // create the message
    _current_uid = GetUID();
    std::string request_msg = generateRequest();
    
    std::cout << request_msg << std::endl;
#if NOSENDING
    return BT::NodeStatus::SUCCESS;         //ABR - decomment this allows to run all the tree without a zmq client on the other side ////////
#endif
    // send the message
    zmq::message_t zmq_request_msg( request_msg.size() );
    memcpy( zmq_request_msg.data(), request_msg.c_str(), request_msg.size() );

    //std::cout << request_msg << std::endl;

    if( !_request_socket.send( zmq_request_msg ) )
    {
       // timeout
       std::cout << "send TIMEOUT" << std::endl;
       return BT::NodeStatus::FAILURE;
    }
    std::cout << "message sent" << std::endl;

    zmq::message_t ack;
    if( !_request_socket.recv( &ack ) )
    {
        std::cout << "ack TIMEOUT" << std::endl;
        return BT::NodeStatus::FAILURE;
    }

    std::cout << "ack received" << std::endl;


    std::cout << "wait reply" << std::endl;
    BT::NodeStatus reply_status = BT::NodeStatus::IDLE;

    while( reply_status == BT::NodeStatus::IDLE )
    {
        zmq::message_t reply;
        _reply_socket.recv( &reply );

        std::string reply_json( static_cast<const char*>(reply.data()), reply.size() );
        reply_status = convertResultToStatus(reply_json);

        std::cout << "REPLY --> " << reply_json << std::endl;
    }

    std::cout << "STATUS ---> " << reply_status << std::endl;

    return reply_status;

}
std::string ApproachLocation_action::generateRequest()
{
    nlohmann::json json;
    json["msg-type"] = "push-skill";
    json["id"] = _current_uid;

    auto& skill = json["skill"];
    skill["name"] = _definition.ID;
    skill["skill-definition-fqn"] = _definition.skill_FQN;
    skill["out-attribute"] = nlohmann::json({});
    auto& in_attribute = skill["in-attribute"] = nlohmann::json({});


    // read node parameters
   for(const auto& param: _definition.params)
    {
        const auto& key  = param.first;
        const auto& type = _definition.params.at(key);

        if( type == "String")
        {
	    auto val = getInput<std::string>(key);	              
            if( !val ){
                throw std::runtime_error( "Invalid parameter at key: " + key );
            }
            in_attribute.push_back( {key, val.value() } );
            std::cout << key << ": " << val.value() <<'\n';
        }
        else if( type == "Double")
        {
            auto val = getInput<double>(key);
            if( !val ){
                throw std::runtime_error( "Invalid parameter at key: " + key );
            }
            in_attribute.push_back( {key, val.value() } );
            std::cout << key << ": " << val.value() <<'\n';
        }
        else if( type == "Integer" || type == "Int" )
        {
            auto val = getInput<int>(key); //ABR - reading the locationId///////////////////////////////////////
            if( !val ){
                throw std::runtime_error( "Invalid parameter at key: " + key );
            }
            in_attribute.push_back( {key, val.value() } );
            std::cout << key << ": " << val.value() <<'\n';
        }
    }

    return json.dump(1);
}

BT::NodeStatus ApproachLocation_action::convertResultToStatus(const std::string &result_string)
{
    nlohmann::json json = nlohmann::json::parse(result_string);

    unsigned msg_id = json["id"];
    if( msg_id != _current_uid)
    {
        return  BT::NodeStatus::IDLE;
    }
    std::string msg_type  = json["msg-type"];
    std::string result    = json["result"]["result"];
    auto json_res_value = json["result"]["result-value"];

    if( json_res_value.is_string() )
    {
        const std::string bb_result_key = _definition.ID + "::last_result";
        std::string res_value = json_res_value;
        //blackboard()->set( bb_result_key, res_value ); //TO CHECK //////////////////////////////////////////////////////////////
    }

    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    if( result == "SUCCESS")
    {
        return BT::NodeStatus::SUCCESS;
    }
    else if( result == "ERROR")
    {
        return BT::NodeStatus::FAILURE;
    }
    else{
        std::cout << "Error parsing this reply:\n" << result << std::endl;
        throw std::runtime_error("Unrecognized [result]" );
    }
}

//2. MoveRoboterPosition_action

MoveRoboterPosition_action::MoveRoboterPosition_action(SkillDefinition definition,
                         const std::string &name,
			 const BT::NodeConfiguration& config,
                         const char* ip,
                         zmq::context_t& context):
    BT::SyncActionNode(name, config),
    _definition(std::move(definition)),
    _request_socket( context, ZMQ_REQ ),
    _reply_socket( context, ZMQ_SUB  ),
    _current_uid(0)
{

    sprintf(_request_address, "tcp://%s:5557", ip );
    _request_socket.connect( _request_address );

    int timeout_ms = 500;
    int linger_ms = 0;
    _request_socket.setsockopt(ZMQ_SNDTIMEO, timeout_ms);
    _request_socket.setsockopt(ZMQ_RCVTIMEO, timeout_ms);
    _request_socket.setsockopt(ZMQ_LINGER, linger_ms);

    sprintf( _reply_address, "tcp://%s:5558", ip );
    _reply_socket.connect( _reply_address );
    _reply_socket.setsockopt(ZMQ_SUBSCRIBE, "", 0);
}

MoveRoboterPosition_action::~MoveRoboterPosition_action()
{
    _request_socket.disconnect(_request_address);
    _reply_socket.disconnect(_reply_address);
}

BT::NodeStatus MoveRoboterPosition_action::tick()
{
    std::cout << "[ TICK: OK ]" << std::endl; //ABR

    // create the message
    _current_uid = GetUID();
    std::string request_msg = generateRequest();
    
    std::cout << request_msg << std::endl;
#if NOSENDING
    return BT::NodeStatus::SUCCESS;         //ABR - decomment this allows to run all the tree without a zmq client on the other side ////////
#endif
    // send the message
    zmq::message_t zmq_request_msg( request_msg.size() );
    memcpy( zmq_request_msg.data(), request_msg.c_str(), request_msg.size() );

    //std::cout << request_msg << std::endl;

    if( !_request_socket.send( zmq_request_msg ) )
    {
       // timeout
       std::cout << "send TIMEOUT" << std::endl;
       return BT::NodeStatus::FAILURE;
    }
    std::cout << "message sent" << std::endl;

    zmq::message_t ack;
    if( !_request_socket.recv( &ack ) )
    {
        std::cout << "ack TIMEOUT" << std::endl;
        return BT::NodeStatus::FAILURE;
    }

    std::cout << "ack received" << std::endl;


    std::cout << "wait reply" << std::endl;
    BT::NodeStatus reply_status = BT::NodeStatus::IDLE;

    while( reply_status == BT::NodeStatus::IDLE )
    {
        zmq::message_t reply;
        _reply_socket.recv( &reply );

        std::string reply_json( static_cast<const char*>(reply.data()), reply.size() );
        reply_status = convertResultToStatus(reply_json);
        std::cout << "REPLY --> " << reply_json << std::endl;
    }

    std::cout << "STATUS ---> " << reply_status << std::endl;

    return reply_status;

}
std::string MoveRoboterPosition_action::generateRequest()
{
    nlohmann::json json;
    json["msg-type"] = "push-skill";
    json["id"] = _current_uid;

    auto& skill = json["skill"];
    skill["name"] = _definition.ID;
    skill["skill-definition-fqn"] = _definition.skill_FQN;
    skill["out-attribute"] = nlohmann::json({});
    auto& in_attribute = skill["in-attribute"] = nlohmann::json({});


    // read node parameters
   for(const auto& param: _definition.params)
    {
        const auto& key  = param.first;
        const auto& type = _definition.params.at(key);

        if( type == "String")
        {
	    auto val = getInput<std::string>(key);	              
            if( !val ){
                throw std::runtime_error( "Invalid parameter at key: " + key );
            }
            in_attribute.push_back( {key, val.value() } );
            std::cout << key << ": " << val.value() <<'\n';
        }
        else if( type == "Double")
        {
            auto val = getInput<double>(key);
            if( !val ){
                throw std::runtime_error( "Invalid parameter at key: " + key );
            }
            in_attribute.push_back( {key, val.value() } );
            std::cout << key << ": " << val.value() <<'\n';
        }
        else if( type == "Integer" || type == "Int" )
        {
            auto val = getInput<int>(key); 
            if( !val ){
                throw std::runtime_error( "Invalid parameter at key: " + key );
            }
            in_attribute.push_back( {key, val.value() } );
            std::cout << key << ": " << val.value() <<'\n';
        }
    }

    return json.dump(1);
}

BT::NodeStatus MoveRoboterPosition_action::convertResultToStatus(const std::string &result_string)
{
    nlohmann::json json = nlohmann::json::parse(result_string);

    unsigned msg_id = json["id"];
    if( msg_id != _current_uid)
    {
        return  BT::NodeStatus::IDLE;
    }
    std::string msg_type  = json["msg-type"];
    std::string result    = json["result"]["result"];
    auto json_res_value = json["result"]["result-value"];

    if( json_res_value.is_string() )
    {
        const std::string bb_result_key = _definition.ID + "::last_result";
        std::string res_value = json_res_value;
        //blackboard()->set( bb_result_key, res_value ); //TO CHECK //////////////////////////////////////////////////////////////
    }

    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    if( result == "SUCCESS")
    {
        return BT::NodeStatus::SUCCESS;
    }
    else if( result == "ERROR")
    {
        return BT::NodeStatus::FAILURE;
    }
    else{
        std::cout << "Error parsing this reply:\n" << result << std::endl;
        throw std::runtime_error("Unrecognized [result]" );
    }
}

//3. MoveRoboterBackwardsLocation_action

MoveRoboterBackwardsLocation_action::MoveRoboterBackwardsLocation_action(SkillDefinition definition,
                         const std::string &name,
			 const BT::NodeConfiguration& config,
                         const char* ip,
                         zmq::context_t& context):
    BT::SyncActionNode(name, config),
    _definition(std::move(definition)),
    _request_socket( context, ZMQ_REQ ),
    _reply_socket( context, ZMQ_SUB  ),
    _current_uid(0)
{

    sprintf(_request_address, "tcp://%s:5557", ip );
    _request_socket.connect( _request_address );

    int timeout_ms = 500;
    int linger_ms = 0;
    _request_socket.setsockopt(ZMQ_SNDTIMEO, timeout_ms);
    _request_socket.setsockopt(ZMQ_RCVTIMEO, timeout_ms);
    _request_socket.setsockopt(ZMQ_LINGER, linger_ms);

    sprintf( _reply_address, "tcp://%s:5558", ip );
    _reply_socket.connect( _reply_address );
    _reply_socket.setsockopt(ZMQ_SUBSCRIBE, "", 0);
}

MoveRoboterBackwardsLocation_action::~MoveRoboterBackwardsLocation_action()
{
    _request_socket.disconnect(_request_address);
    _reply_socket.disconnect(_reply_address);
}

BT::NodeStatus MoveRoboterBackwardsLocation_action::tick()
{
    std::cout << "[ TICK: OK ]" << std::endl; //ABR

    // create the message
    _current_uid = GetUID();
    std::string request_msg = generateRequest();
    
    std::cout << request_msg << std::endl;
#if NOSENDING
    return BT::NodeStatus::SUCCESS;         //ABR - decomment this allows to run all the tree without a zmq client on the other side ////////
#endif
    // send the message
    zmq::message_t zmq_request_msg( request_msg.size() );
    memcpy( zmq_request_msg.data(), request_msg.c_str(), request_msg.size() );

    //std::cout << request_msg << std::endl;

    if( !_request_socket.send( zmq_request_msg ) )
    {
       // timeout
       std::cout << "send TIMEOUT" << std::endl;
       return BT::NodeStatus::FAILURE;
    }
    std::cout << "message sent" << std::endl;

    zmq::message_t ack;
    if( !_request_socket.recv( &ack ) )
    {
        std::cout << "ack TIMEOUT" << std::endl;
        return BT::NodeStatus::FAILURE;
    }

    std::cout << "ack received" << std::endl;


    std::cout << "wait reply" << std::endl;
    BT::NodeStatus reply_status = BT::NodeStatus::IDLE;

    while( reply_status == BT::NodeStatus::IDLE )
    {
        zmq::message_t reply;
        _reply_socket.recv( &reply );

        std::string reply_json( static_cast<const char*>(reply.data()), reply.size() );
        reply_status = convertResultToStatus(reply_json);
        std::cout << "REPLY --> " << reply_json << std::endl;
    }

    std::cout << "STATUS ---> " << reply_status << std::endl;

    return reply_status;

}
std::string MoveRoboterBackwardsLocation_action::generateRequest()
{
    nlohmann::json json;
    json["msg-type"] = "push-skill";
    json["id"] = _current_uid;

    auto& skill = json["skill"];
    skill["name"] = _definition.ID;
    skill["skill-definition-fqn"] = _definition.skill_FQN;
    skill["out-attribute"] = nlohmann::json({});
    auto& in_attribute = skill["in-attribute"] = nlohmann::json({});


    // read node parameters
   for(const auto& param: _definition.params)
    {
        const auto& key  = param.first;
        const auto& type = _definition.params.at(key);

        if( type == "String")
        {
	    auto val = getInput<std::string>(key);	              
            if( !val ){
                throw std::runtime_error( "Invalid parameter at key: " + key );
            }
            in_attribute.push_back( {key, val.value() } );
            std::cout << key << ": " << val.value() <<'\n';
        }
        else if( type == "Double")
        {
            auto val = getInput<double>(key);
            if( !val ){
                throw std::runtime_error( "Invalid parameter at key: " + key );
            }
            in_attribute.push_back( {key, val.value() } );
            std::cout << key << ": " << val.value() <<'\n';
        }
        else if( type == "Integer" || type == "Int" )
        {
            auto val = getInput<int>(key); 
            if( !val ){
                throw std::runtime_error( "Invalid parameter at key: " + key );
            }
            in_attribute.push_back( {key, val.value() } );
            std::cout << key << ": " << val.value() <<'\n';
        }
    }

    return json.dump(1);
}

BT::NodeStatus MoveRoboterBackwardsLocation_action::convertResultToStatus(const std::string &result_string)
{
    nlohmann::json json = nlohmann::json::parse(result_string);

    unsigned msg_id = json["id"];
    if( msg_id != _current_uid)
    {
        return  BT::NodeStatus::IDLE;
    }
    std::string msg_type  = json["msg-type"];
    std::string result    = json["result"]["result"];
    auto json_res_value = json["result"]["result-value"];

    if( json_res_value.is_string() )
    {
        const std::string bb_result_key = _definition.ID + "::last_result";
        std::string res_value = json_res_value;
        //blackboard()->set( bb_result_key, res_value ); //TO CHECK //////////////////////////////////////////////////////////////
    }

    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    if( result == "SUCCESS")
    {
        return BT::NodeStatus::SUCCESS;
    }
    else if( result == "ERROR")
    {
        return BT::NodeStatus::FAILURE;
    }
    else{
        std::cout << "Error parsing this reply:\n" << result << std::endl;
        throw std::runtime_error("Unrecognized [result]" );
    }
}

//4. MoveRoboterBackwardsRelative_action

MoveRoboterBackwardsRelative_action::MoveRoboterBackwardsRelative_action(SkillDefinition definition,
                         const std::string &name,
			 const BT::NodeConfiguration& config,
                         const char* ip,
                         zmq::context_t& context):
    BT::SyncActionNode(name, config),
    _definition(std::move(definition)),
    _request_socket( context, ZMQ_REQ ),
    _reply_socket( context, ZMQ_SUB  ),
    _current_uid(0)
{

    sprintf(_request_address, "tcp://%s:5557", ip );
    _request_socket.connect( _request_address );

    int timeout_ms = 500;
    int linger_ms = 0;
    _request_socket.setsockopt(ZMQ_SNDTIMEO, timeout_ms);
    _request_socket.setsockopt(ZMQ_RCVTIMEO, timeout_ms);
    _request_socket.setsockopt(ZMQ_LINGER, linger_ms);

    sprintf( _reply_address, "tcp://%s:5558", ip );
    _reply_socket.connect( _reply_address );
    _reply_socket.setsockopt(ZMQ_SUBSCRIBE, "", 0);
}

MoveRoboterBackwardsRelative_action::~MoveRoboterBackwardsRelative_action()
{
    _request_socket.disconnect(_request_address);
    _reply_socket.disconnect(_reply_address);
}

BT::NodeStatus MoveRoboterBackwardsRelative_action::tick()
{
    std::cout << "[ TICK: OK ]" << std::endl; //ABR

    // create the message
    _current_uid = GetUID();
    std::string request_msg = generateRequest();
    
    std::cout << request_msg << std::endl;
#if NOSENDING
    return BT::NodeStatus::SUCCESS;         //ABR - decomment this allows to run all the tree without a zmq client on the other side ////////
#endif
    // send the message
    zmq::message_t zmq_request_msg( request_msg.size() );
    memcpy( zmq_request_msg.data(), request_msg.c_str(), request_msg.size() );

    //std::cout << request_msg << std::endl;

    if( !_request_socket.send( zmq_request_msg ) )
    {
       // timeout
       std::cout << "send TIMEOUT" << std::endl;
       return BT::NodeStatus::FAILURE;
    }
    std::cout << "message sent" << std::endl;

    zmq::message_t ack;
    if( !_request_socket.recv( &ack ) )
    {
        std::cout << "ack TIMEOUT" << std::endl;
        return BT::NodeStatus::FAILURE;
    }

    std::cout << "ack received" << std::endl;


    std::cout << "wait reply" << std::endl;
    BT::NodeStatus reply_status = BT::NodeStatus::IDLE;

    while( reply_status == BT::NodeStatus::IDLE )
    {
        zmq::message_t reply;
        _reply_socket.recv( &reply );

        std::string reply_json( static_cast<const char*>(reply.data()), reply.size() );
        reply_status = convertResultToStatus(reply_json);
        std::cout << "REPLY --> " << reply_json << std::endl;
    }

    std::cout << "STATUS ---> " << reply_status << std::endl;

    return reply_status;

}
std::string MoveRoboterBackwardsRelative_action::generateRequest()
{
    nlohmann::json json;
    json["msg-type"] = "push-skill";
    json["id"] = _current_uid;

    auto& skill = json["skill"];
    skill["name"] = _definition.ID;
    skill["skill-definition-fqn"] = _definition.skill_FQN;
    skill["out-attribute"] = nlohmann::json({});
    auto& in_attribute = skill["in-attribute"] = nlohmann::json({});


    // read node parameters
   for(const auto& param: _definition.params)
    {
        const auto& key  = param.first;
        const auto& type = _definition.params.at(key);

        if( type == "String")
        {
	    auto val = getInput<std::string>(key);	              
            if( !val ){
                throw std::runtime_error( "Invalid parameter at key: " + key );
            }
            in_attribute.push_back( {key, val.value() } );
            std::cout << key << ": " << val.value() <<'\n';
        }
        else if( type == "Double")
        {
            auto val = getInput<double>(key);
            if( !val ){
                throw std::runtime_error( "Invalid parameter at key: " + key );
            }
            in_attribute.push_back( {key, val.value() } );
            std::cout << key << ": " << val.value() <<'\n';
        }
        else if( type == "Integer" || type == "Int" )
        {
            auto val = getInput<int>(key); 
            if( !val ){
                throw std::runtime_error( "Invalid parameter at key: " + key );
            }
            in_attribute.push_back( {key, val.value() } );
            std::cout << key << ": " << val.value() <<'\n';
        }
    }

    return json.dump(1);
}

BT::NodeStatus MoveRoboterBackwardsRelative_action::convertResultToStatus(const std::string &result_string)
{
    nlohmann::json json = nlohmann::json::parse(result_string);

    unsigned msg_id = json["id"];
    if( msg_id != _current_uid)
    {
        return  BT::NodeStatus::IDLE;
    }
    std::string msg_type  = json["msg-type"];
    std::string result    = json["result"]["result"];
    auto json_res_value = json["result"]["result-value"];

    if( json_res_value.is_string() )
    {
        const std::string bb_result_key = _definition.ID + "::last_result";
        std::string res_value = json_res_value;
        //blackboard()->set( bb_result_key, res_value ); //TO CHECK //////////////////////////////////////////////////////////////
    }

    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    if( result == "SUCCESS")
    {
        return BT::NodeStatus::SUCCESS;
    }
    else if( result == "ERROR")
    {
        return BT::NodeStatus::FAILURE;
    }
    else{
        std::cout << "Error parsing this reply:\n" << result << std::endl;
        throw std::runtime_error("Unrecognized [result]" );
    }
}

//5. MoveRoboterOrientateRobotAbs_action

MoveRoboterOrientateRobotAbs_action::MoveRoboterOrientateRobotAbs_action(SkillDefinition definition,
                         const std::string &name,
			 const BT::NodeConfiguration& config,
                         const char* ip,
                         zmq::context_t& context):
    BT::SyncActionNode(name, config),
    _definition(std::move(definition)),
    _request_socket( context, ZMQ_REQ ),
    _reply_socket( context, ZMQ_SUB  ),
    _current_uid(0)
{

    sprintf(_request_address, "tcp://%s:5557", ip );
    _request_socket.connect( _request_address );

    int timeout_ms = 500;
    int linger_ms = 0;
    _request_socket.setsockopt(ZMQ_SNDTIMEO, timeout_ms);
    _request_socket.setsockopt(ZMQ_RCVTIMEO, timeout_ms);
    _request_socket.setsockopt(ZMQ_LINGER, linger_ms);

    sprintf( _reply_address, "tcp://%s:5558", ip );
    _reply_socket.connect( _reply_address );
    _reply_socket.setsockopt(ZMQ_SUBSCRIBE, "", 0);
}

MoveRoboterOrientateRobotAbs_action::~MoveRoboterOrientateRobotAbs_action()
{
    _request_socket.disconnect(_request_address);
    _reply_socket.disconnect(_reply_address);
}

BT::NodeStatus MoveRoboterOrientateRobotAbs_action::tick()
{
    std::cout << "[ TICK: OK ]" << std::endl; //ABR

    // create the message
    _current_uid = GetUID();
    std::string request_msg = generateRequest();
    
    std::cout << request_msg << std::endl;
#if NOSENDING
    return BT::NodeStatus::SUCCESS;         //ABR - decomment this allows to run all the tree without a zmq client on the other side ////////
#endif
    // send the message
    zmq::message_t zmq_request_msg( request_msg.size() );
    memcpy( zmq_request_msg.data(), request_msg.c_str(), request_msg.size() );

    //std::cout << request_msg << std::endl;

    if( !_request_socket.send( zmq_request_msg ) )
    {
       // timeout
       std::cout << "send TIMEOUT" << std::endl;
       return BT::NodeStatus::FAILURE;
    }
    std::cout << "message sent" << std::endl;

    zmq::message_t ack;
    if( !_request_socket.recv( &ack ) )
    {
        std::cout << "ack TIMEOUT" << std::endl;
        return BT::NodeStatus::FAILURE;
    }

    std::cout << "ack received" << std::endl;


    std::cout << "wait reply" << std::endl;
    BT::NodeStatus reply_status = BT::NodeStatus::IDLE;

    while( reply_status == BT::NodeStatus::IDLE )
    {
        zmq::message_t reply;
        _reply_socket.recv( &reply );

        std::string reply_json( static_cast<const char*>(reply.data()), reply.size() );
        reply_status = convertResultToStatus(reply_json);
        std::cout << "REPLY --> " << reply_json << std::endl;
    }

    std::cout << "STATUS ---> " << reply_status << std::endl;

    return reply_status;

}
std::string MoveRoboterOrientateRobotAbs_action::generateRequest()
{
    nlohmann::json json;
    json["msg-type"] = "push-skill";
    json["id"] = _current_uid;

    auto& skill = json["skill"];
    skill["name"] = _definition.ID;
    skill["skill-definition-fqn"] = _definition.skill_FQN;
    skill["out-attribute"] = nlohmann::json({});
    auto& in_attribute = skill["in-attribute"] = nlohmann::json({});


    // read node parameters
   for(const auto& param: _definition.params)
    {
        const auto& key  = param.first;
        const auto& type = _definition.params.at(key);

        if( type == "String")
        {
	    auto val = getInput<std::string>(key);	              
            if( !val ){
                throw std::runtime_error( "Invalid parameter at key: " + key );
            }
            in_attribute.push_back( {key, val.value() } );
            std::cout << key << ": " << val.value() <<'\n';
        }
        else if( type == "Double")
        {
            auto val = getInput<double>(key);
            if( !val ){
                throw std::runtime_error( "Invalid parameter at key: " + key );
            }
            in_attribute.push_back( {key, val.value() } );
            std::cout << key << ": " << val.value() <<'\n';
        }
        else if( type == "Integer" || type == "Int" )
        {
            auto val = getInput<int>(key); 
            if( !val ){
                throw std::runtime_error( "Invalid parameter at key: " + key );
            }
            in_attribute.push_back( {key, val.value() } );
            std::cout << key << ": " << val.value() <<'\n';
        }
    }

    return json.dump(1);
}

BT::NodeStatus MoveRoboterOrientateRobotAbs_action::convertResultToStatus(const std::string &result_string)
{
    nlohmann::json json = nlohmann::json::parse(result_string);

    unsigned msg_id = json["id"];
    if( msg_id != _current_uid)
    {
        return  BT::NodeStatus::IDLE;
    }
    std::string msg_type  = json["msg-type"];
    std::string result    = json["result"]["result"];
    auto json_res_value = json["result"]["result-value"];

    if( json_res_value.is_string() )
    {
        const std::string bb_result_key = _definition.ID + "::last_result";
        std::string res_value = json_res_value;
        //blackboard()->set( bb_result_key, res_value ); //TO CHECK //////////////////////////////////////////////////////////////
    }

    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    if( result == "SUCCESS")
    {
        return BT::NodeStatus::SUCCESS;
    }
    else if( result == "ERROR")
    {
        return BT::NodeStatus::FAILURE;
    }
    else{
        std::cout << "Error parsing this reply:\n" << result << std::endl;
        throw std::runtime_error("Unrecognized [result]" );
    }
}

//6. MoveRoboterOrientateRobotRel_action

MoveRoboterOrientateRobotRel_action::MoveRoboterOrientateRobotRel_action(SkillDefinition definition,
                         const std::string &name,
			 const BT::NodeConfiguration& config,
                         const char* ip,
                         zmq::context_t& context):
    BT::SyncActionNode(name, config),
    _definition(std::move(definition)),
    _request_socket( context, ZMQ_REQ ),
    _reply_socket( context, ZMQ_SUB  ),
    _current_uid(0)
{

    sprintf(_request_address, "tcp://%s:5557", ip );
    _request_socket.connect( _request_address );

    int timeout_ms = 500;
    int linger_ms = 0;
    _request_socket.setsockopt(ZMQ_SNDTIMEO, timeout_ms);
    _request_socket.setsockopt(ZMQ_RCVTIMEO, timeout_ms);
    _request_socket.setsockopt(ZMQ_LINGER, linger_ms);

    sprintf( _reply_address, "tcp://%s:5558", ip );
    _reply_socket.connect( _reply_address );
    _reply_socket.setsockopt(ZMQ_SUBSCRIBE, "", 0);
}

MoveRoboterOrientateRobotRel_action::~MoveRoboterOrientateRobotRel_action()
{
    _request_socket.disconnect(_request_address);
    _reply_socket.disconnect(_reply_address);
}

BT::NodeStatus MoveRoboterOrientateRobotRel_action::tick()
{
    std::cout << "[ TICK: OK ]" << std::endl; //ABR

    // create the message
    _current_uid = GetUID();
    std::string request_msg = generateRequest();
    
    std::cout << request_msg << std::endl;
#if NOSENDING
    return BT::NodeStatus::SUCCESS;         //ABR - decomment this allows to run all the tree without a zmq client on the other side ////////
#endif
    // send the message
    zmq::message_t zmq_request_msg( request_msg.size() );
    memcpy( zmq_request_msg.data(), request_msg.c_str(), request_msg.size() );

    //std::cout << request_msg << std::endl;

    if( !_request_socket.send( zmq_request_msg ) )
    {
       // timeout
       std::cout << "send TIMEOUT" << std::endl;
       return BT::NodeStatus::FAILURE;
    }
    std::cout << "message sent" << std::endl;

    zmq::message_t ack;
    if( !_request_socket.recv( &ack ) )
    {
        std::cout << "ack TIMEOUT" << std::endl;
        return BT::NodeStatus::FAILURE;
    }

    std::cout << "ack received" << std::endl;


    std::cout << "wait reply" << std::endl;
    BT::NodeStatus reply_status = BT::NodeStatus::IDLE;

    while( reply_status == BT::NodeStatus::IDLE )
    {
        zmq::message_t reply;
        _reply_socket.recv( &reply );

        std::string reply_json( static_cast<const char*>(reply.data()), reply.size() );
        reply_status = convertResultToStatus(reply_json);
        std::cout << "REPLY --> " << reply_json << std::endl;
    }

    std::cout << "STATUS ---> " << reply_status << std::endl;

    return reply_status;

}
std::string MoveRoboterOrientateRobotRel_action::generateRequest()
{
    nlohmann::json json;
    json["msg-type"] = "push-skill";
    json["id"] = _current_uid;

    auto& skill = json["skill"];
    skill["name"] = _definition.ID;
    skill["skill-definition-fqn"] = _definition.skill_FQN;
    skill["out-attribute"] = nlohmann::json({});
    auto& in_attribute = skill["in-attribute"] = nlohmann::json({});


    // read node parameters
   for(const auto& param: _definition.params)
    {
        const auto& key  = param.first;
        const auto& type = _definition.params.at(key);

        if( type == "String")
        {
	    auto val = getInput<std::string>(key);	              
            if( !val ){
                throw std::runtime_error( "Invalid parameter at key: " + key );
            }
            in_attribute.push_back( {key, val.value() } );
            std::cout << key << ": " << val.value() <<'\n';
        }
        else if( type == "Double")
        {
            auto val = getInput<double>(key);
            if( !val ){
                throw std::runtime_error( "Invalid parameter at key: " + key );
            }
            in_attribute.push_back( {key, val.value() } );
            std::cout << key << ": " << val.value() <<'\n';
        }
        else if( type == "Integer" || type == "Int" )
        {
            auto val = getInput<int>(key); 
            if( !val ){
                throw std::runtime_error( "Invalid parameter at key: " + key );
            }
            in_attribute.push_back( {key, val.value() } );
            std::cout << key << ": " << val.value() <<'\n';
        }
    }

    return json.dump(1);
}

BT::NodeStatus MoveRoboterOrientateRobotRel_action::convertResultToStatus(const std::string &result_string)
{
    nlohmann::json json = nlohmann::json::parse(result_string);

    unsigned msg_id = json["id"];
    if( msg_id != _current_uid)
    {
        return  BT::NodeStatus::IDLE;
    }
    std::string msg_type  = json["msg-type"];
    std::string result    = json["result"]["result"];
    auto json_res_value = json["result"]["result-value"];

    if( json_res_value.is_string() )
    {
        const std::string bb_result_key = _definition.ID + "::last_result";
        std::string res_value = json_res_value;
        //blackboard()->set( bb_result_key, res_value ); //TO CHECK //////////////////////////////////////////////////////////////
    }

    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    if( result == "SUCCESS")
    {
        return BT::NodeStatus::SUCCESS;
    }
    else if( result == "ERROR")
    {
        return BT::NodeStatus::FAILURE;
    }
    else{
        std::cout << "Error parsing this reply:\n" << result << std::endl;
        throw std::runtime_error("Unrecognized [result]" );
    }
}

