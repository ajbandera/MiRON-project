# MiRON-project

Initial framework for testing BT executor and its connection to the smartMDSD environment

The aim is to close a complete loop where the BT executor drives the course of action sending skills to a SkillInterface component on the smartMDSD environment. The received skill will be updated on the knowledge base (KB component), allowing the Sequencer component to manage its execution.

Current state:
- The BT executor works with BT V3
- The skill is sent by the executor to the SkillInterface component. This component does not manage the skill: it only sends a SUCCESS result as an ACK reply

Next steps:
- Update the BehaviorTree.CPP. Currently, each skill is associated to an individual action as they have a specific collection of ports. This problem was solved (see https://github.com/BehaviorTree/BehaviorTree.CPP/blob/master/examples/t11_runtime_ports.cpp)
- Update the skill on the KB component by the SkillInterface component
- The Sequencer component should read the skill from the KB and manages its execution

Testing:
- ABR_test.xml - Behavior tree sequencing the six available skills
- ABRTesting.skills.json - File with the collection of six available skills
- SystemTiagoNavigation.skills.json - File with the whole collection of skills from the SystemTiagoNavigation environment on SmartMDSD

# Compilation instructions (Ubuntu Linux) 
`mkdir build`  
`cd build`  
`cmake ..`  
`make`
