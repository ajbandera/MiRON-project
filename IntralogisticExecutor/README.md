
Current version:

Each action is managed in an individual manner

  	NodeBuilder creator = [def, &zmq_context, &arg](const std::string& name, const BT::NodeConfiguration& config)
    			{return std::unique_ptr<TreeNode>( new ApproachLocation_action(def, name, config, arg.IP.c_str(), zmq_context) );};
		factory.registerBuilder<ApproachLocation_action>( def.ID, creator );
    
for allowing us to define its specific ports. For testing, six actions have been defined and tested.

Next step:

The need of defining each action individually because of the ports have been solved in the new version of the BT. See

https://github.com/BehaviorTree/BehaviorTree.CPP/blob/master/examples/t11_runtime_ports.cpp

