#ifndef SKILL_INTERFACE_HPP
#define SKILL_INTERFACE_HPP

#include <behaviortree_cpp_v3/behavior_tree.h>
#include <zmq.hpp>

//GENERAL

typedef std::unordered_map<std::string, std::string> NodeParameters;

struct SkillDefinition
{
    std::string ID;
    std::string skill_FQN;
    NodeParameters params;

    struct ReturnValue{
        BT::NodeStatus res;
        std::string value;
    };
    std::vector<ReturnValue> possible_results;
};

std::vector<SkillDefinition> ParseSkillFile( const std::string& filename );

inline unsigned GetUID()
{
    static unsigned count = 0; return ++count;
}

std::string GenerateRequest(const SkillDefinition& definition, unsigned msg_uid , int indent = -1);

#if 0
//template
class SkillAction: public BT::SyncActionNode
{
public:
    SkillAction(SkillDefinition definition,
                const std::string& name, const BT::NodeConfiguration& config,
                const char *ip, zmq::context_t& context);

    ~SkillAction();

    BT::NodeStatus tick() override;

    //void halt() override {}

    static BT::PortsList providedPorts() {
		return{ BT::InputPort<std::string>("location") };
	}
private:
    SkillDefinition _definition;
    zmq::socket_t  _request_socket;
    zmq::socket_t  _reply_socket;
    unsigned        _current_uid;
    char _request_address[100];
    char _reply_address[100];

    BT::NodeStatus convertResultToStatus(const std::string& result_value);

    std::string generateRequest();
};
#endif

//1. APPROACHLOCATION
class ApproachLocation_action: public BT::SyncActionNode
{
public:
    ApproachLocation_action(SkillDefinition definition,
                const std::string& name, const BT::NodeConfiguration& config,
                const char *ip, zmq::context_t& context);

    ~ApproachLocation_action();

    BT::NodeStatus tick() override;

    //void halt() override {}

    static BT::PortsList providedPorts() {
		return{ BT::InputPort<std::string>("locationId") };
	}
private:
    SkillDefinition _definition;
    zmq::socket_t  _request_socket;
    zmq::socket_t  _reply_socket;
    unsigned        _current_uid;
    char _request_address[100];
    char _reply_address[100];

    BT::NodeStatus convertResultToStatus(const std::string& result_value);

    std::string generateRequest();
};

//2. MOVEROBOTERPOSITION
class MoveRoboterPosition_action: public BT::SyncActionNode
{
public:
    MoveRoboterPosition_action(SkillDefinition definition,
                const std::string& name, const BT::NodeConfiguration& config,
                const char *ip, zmq::context_t& context);

    ~MoveRoboterPosition_action();

    BT::NodeStatus tick() override;

    //void halt() override {}

    static BT::PortsList providedPorts()
    {
        static BT::PortsList ports_list = { {"x", BT::PortDirection::INPUT}, 
                                            {"y", BT::PortDirection::INPUT},
					    {"approachRadius", BT::PortDirection::INPUT} };
        return ports_list;
    } 

private:
    SkillDefinition _definition;
    zmq::socket_t  _request_socket;
    zmq::socket_t  _reply_socket;
    unsigned        _current_uid;
    char _request_address[100];
    char _reply_address[100];

    BT::NodeStatus convertResultToStatus(const std::string& result_value);

    std::string generateRequest();
};

//3. MOVEROBOTERBACKWARDSLOCATION
class MoveRoboterBackwardsLocation_action: public BT::SyncActionNode
{
public:
    MoveRoboterBackwardsLocation_action(SkillDefinition definition,
                const std::string& name, const BT::NodeConfiguration& config,
                const char *ip, zmq::context_t& context);

    ~MoveRoboterBackwardsLocation_action();

    BT::NodeStatus tick() override;

    //void halt() override {}

    static BT::PortsList providedPorts() {
		return{ BT::InputPort<std::string>("locationID") };
	}
private:
    SkillDefinition _definition;
    zmq::socket_t  _request_socket;
    zmq::socket_t  _reply_socket;
    unsigned        _current_uid;
    char _request_address[100];
    char _reply_address[100];

    BT::NodeStatus convertResultToStatus(const std::string& result_value);

    std::string generateRequest();
};

//4. MOVEROBOTERBACKWARDSRELATIVE
class MoveRoboterBackwardsRelative_action: public BT::SyncActionNode
{
public:
    MoveRoboterBackwardsRelative_action(SkillDefinition definition,
                const std::string& name, const BT::NodeConfiguration& config,
                const char *ip, zmq::context_t& context);

    ~MoveRoboterBackwardsRelative_action();

    BT::NodeStatus tick() override;

    //void halt() override {}

    static BT::PortsList providedPorts() {
		return{ BT::InputPort<std::string>("dist") };
	}
private:
    SkillDefinition _definition;
    zmq::socket_t  _request_socket;
    zmq::socket_t  _reply_socket;
    unsigned        _current_uid;
    char _request_address[100];
    char _reply_address[100];

    BT::NodeStatus convertResultToStatus(const std::string& result_value);

    std::string generateRequest();
};

//5. MOVEROBOTERORIENTATEROBOTABS
class MoveRoboterOrientateRobotAbs_action: public BT::SyncActionNode
{
public:
    MoveRoboterOrientateRobotAbs_action(SkillDefinition definition,
                const std::string& name, const BT::NodeConfiguration& config,
                const char *ip, zmq::context_t& context);

    ~MoveRoboterOrientateRobotAbs_action();

    BT::NodeStatus tick() override;

    //void halt() override {}

    static BT::PortsList providedPorts() {
		return{ BT::InputPort<std::string>("angleDeg") };
	}
private:
    SkillDefinition _definition;
    zmq::socket_t  _request_socket;
    zmq::socket_t  _reply_socket;
    unsigned        _current_uid;
    char _request_address[100];
    char _reply_address[100];

    BT::NodeStatus convertResultToStatus(const std::string& result_value);

    std::string generateRequest();
};

//6. MOVEROBOTERORIENTATEROBOTREL
class MoveRoboterOrientateRobotRel_action: public BT::SyncActionNode
{
public:
    MoveRoboterOrientateRobotRel_action(SkillDefinition definition,
                const std::string& name, const BT::NodeConfiguration& config,
                const char *ip, zmq::context_t& context);

    ~MoveRoboterOrientateRobotRel_action();

    BT::NodeStatus tick() override;

    //void halt() override {}

    static BT::PortsList providedPorts() {
		return{ BT::InputPort<std::string>("angleDeg") };
	}
private:
    SkillDefinition _definition;
    zmq::socket_t  _request_socket;
    zmq::socket_t  _reply_socket;
    unsigned        _current_uid;
    char _request_address[100];
    char _reply_address[100];

    BT::NodeStatus convertResultToStatus(const std::string& result_value);

    std::string generateRequest();
};


#endif // SKILL_INTERFACE_HPP
