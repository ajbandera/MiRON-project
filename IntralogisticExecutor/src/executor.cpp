//#include <behaviortree_cpp_v3/blackboard/blackboard_local.h>
#include <behaviortree_cpp_v3/bt_factory.h>
#include <behaviortree_cpp_v3/behavior_tree.h>
#include <behaviortree_cpp_v3/xml_parsing.h>
#include <list>

#include <behaviortree_cpp_v3/loggers/bt_cout_logger.h>
#include <behaviortree_cpp_v3/loggers/bt_file_logger.h>
#include <behaviortree_cpp_v3/loggers/bt_zmq_publisher.h>

#include "Intralogistic/args.hpp"
#include "Intralogistic/skill_interface.hpp"

using namespace BT;

struct Arguments{
    std::string skills_file;
    std::string tree_file;
    std::string IP;
};

// Read the arguments: skills_file (json), tree_file (xml) and ip ("localhost" by default)
Arguments ParseArguments(int argc, char** argv);

// main
int main(int argc, char** argv)
{   
    Arguments arg = ParseArguments(argc, argv);

    std::cout << "\ttree file: "   << arg.tree_file << std::endl;
    std::cout << "\tIP: "          << arg.IP << std::endl;
    std::cout << "\tskills file: " << arg.skills_file << std::endl;

    //------------------------------------------------------
    // Populate the factory from Skills file and
    // create the tree from BehaviorTree file.

    zmq::context_t zmq_context(1);
	
    NodeParameters params;

    auto definitions = ParseSkillFile( arg.skills_file );

    BehaviorTreeFactory factory;

    // register an action for each Skill
    for (const auto& def: definitions )
    {
	std::cout << "NEW skill: " << def.ID << " " << def.skill_FQN << std::endl; 
	if (def.ID == "approachLocation")
		{
  		NodeBuilder creator = [def, &zmq_context, &arg](const std::string& name, const BT::NodeConfiguration& config)
    			{return std::unique_ptr<TreeNode>( new ApproachLocation_action(def, name, config, arg.IP.c_str(), zmq_context) );};
		factory.registerBuilder<ApproachLocation_action>( def.ID, creator );
		}
	if (def.ID == "moveRoboterPosition")
		{
  		NodeBuilder creator = [def, &zmq_context, &arg](const std::string& name, const BT::NodeConfiguration& config)
    			{return std::unique_ptr<TreeNode>( new MoveRoboterPosition_action(def, name, config, arg.IP.c_str(), zmq_context) );};
		factory.registerBuilder<MoveRoboterPosition_action>( def.ID, creator );
		}
	if (def.ID == "moveRoboterBackwardsLocation")
		{
  		NodeBuilder creator = [def, &zmq_context, &arg](const std::string& name, const BT::NodeConfiguration& config)
    			{return std::unique_ptr<TreeNode>( new MoveRoboterBackwardsLocation_action(def, name, config, arg.IP.c_str(), zmq_context) );};
		factory.registerBuilder<MoveRoboterBackwardsLocation_action>( def.ID, creator );
		}
	if (def.ID == "moveRoboterBackwardsRelative")
		{
  		NodeBuilder creator = [def, &zmq_context, &arg](const std::string& name, const BT::NodeConfiguration& config)
    			{return std::unique_ptr<TreeNode>( new MoveRoboterBackwardsRelative_action(def, name, config, arg.IP.c_str(), zmq_context) );};
		factory.registerBuilder<MoveRoboterBackwardsRelative_action>( def.ID, creator );
		}
	if (def.ID == "moveRoboterOrientateRobotAbs")
		{
  		NodeBuilder creator = [def, &zmq_context, &arg](const std::string& name, const BT::NodeConfiguration& config)
    			{return std::unique_ptr<TreeNode>( new MoveRoboterOrientateRobotAbs_action(def, name, config, arg.IP.c_str(), zmq_context) );};
		factory.registerBuilder<MoveRoboterOrientateRobotAbs_action>( def.ID, creator );
		}
	if (def.ID == "moveRoboterOrientateRobotRel")
		{
  		NodeBuilder creator = [def, &zmq_context, &arg](const std::string& name, const BT::NodeConfiguration& config)
    			{return std::unique_ptr<TreeNode>( new MoveRoboterOrientateRobotRel_action(def, name, config, arg.IP.c_str(), zmq_context) );};
		factory.registerBuilder<MoveRoboterOrientateRobotRel_action>( def.ID, creator );
		}
    }

    std::cout << "===============" << std::endl;
    std::cout << "List of Actions" << std::endl;
    std::cout << "===============" << std::endl;

    for (const auto& model : factory.manifests())
    {
        std::cout << "Action: " << model.second.registration_ID << std::endl;
    }

    auto tree = factory.createTreeFromFile(arg.tree_file);

    //------------------------------------------------------
    // Execute the tree
    std::cout << "===============" << std::endl;
    std::cout << "Tree Execution " << std::endl;
    std::cout << "===============" << std::endl;
    tree.root_node->executeTick(); 

#if 0
    auto blackboard = Blackboard::create<BlackboardLocal>();

    // add loggers
    StdCoutLogger logger_cout(tree.root_node);
    FileLogger logger_file(tree.root_node, "ulm_trace.fbl");
    PublisherZMQ publisher_zmq(tree.root_node);
#endif

    return 0;
}

///////////////////////////////////////////////////////////////

Arguments ParseArguments(int argc, char** argv)
{
    // Read more about args here: https://github.com/Taywee/args

    args::ArgumentParser parser("BehaviorTree.CPP Executor", "Load one or multiple plugins and the XML with the tree definition.");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});

    args::Group arguments(parser, "arguments", args::Group::Validators::DontCare, args::Options::Global);

    args::ValueFlag<std::string> tree_path(arguments, "path",
                                           "The XML containing the BehaviorTree ", {'t', "tree"},
                                           args::Options::Required);

    args::ValueFlag<std::string> skills_path( arguments, "path",
                                              "JSON file containing the list of SmartSoft skills", {'s', "skills"},
                                              args::Options::Required);

    args::ValueFlag<std::string> server_ip(arguments, "ip",
                                           "IP of the server", {"ip"}, "localhost" );
    try
    {
        parser.ParseCLI(argc, argv);
    }
    catch (const args::Completion& e)
    {
        std::cout << e.what();
        exit(0);
    }
    catch (const args::Help&)
    {
        std::cout << parser;
        exit(0);
    }
    catch (const args::ParseError& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        exit(0);
    }
    catch (const args::RequiredError& e)
    {
        std::cerr << "One of the mandatory arguments is missing:" << std::endl;
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        exit(0);
    }

    Arguments output;

    output.skills_file =  args::get(skills_path);
    output.tree_file   =  args::get(tree_path);
    output.IP =  args::get(server_ip);

    return output;
}
