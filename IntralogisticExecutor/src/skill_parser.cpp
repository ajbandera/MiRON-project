#include <fstream>
#include <unordered_set>
#include <algorithm>
#include <string>
#include "Intralogistic/skill_interface.hpp"
#include "Intralogistic/json.hpp"


std::vector<SkillDefinition> ParseSkillFile(const std::string &filename)
{
    std::vector<SkillDefinition> definitions;

    std::ifstream input_file(filename);
    if(input_file.fail())
    {
        throw std::runtime_error("The json file can't be opened");
    }

    nlohmann::json doc;
    input_file >> doc;

    const std::unordered_set<std::string> supported_types =
    {
        "String", "Int", "Double", "Integer"
    };

    for (const auto& item: doc)
    {
        const auto& skill = item["skill"];
        SkillDefinition definition;
        definition.ID = skill["name"];
        definition.skill_FQN = skill["skill-definition-fqn"];

        const auto& inAttributes = skill["in-attribute"];

        for (auto it = inAttributes.begin(); it != inAttributes.end(); it++)
        {
            std::string key   = it.key();
            std::string type = it.value();

            if( supported_types.count(type) == 0)
            {
                throw std::runtime_error("Error in [in-attribute]: We don't support this type: " + type);
            }

            definition.params.insert( { key, type} );
        }

        const auto& results = skill["results"];
        for (const auto& result: results)
        {
            std::string res   = result["result"];
            std::string value = result["result-value"];

            BT::NodeStatus status;
            if( res == "SUCCESS")
            {
                status = BT::NodeStatus::SUCCESS;
            }
            else if( res == "ERROR")
            {
                status = BT::NodeStatus::FAILURE;
            }
            else{
                throw std::runtime_error( "Error in skill->results->result: only accepted values "
                                          "are SUCCESS and ERROR");
            }
            definition.possible_results.push_back( {status,value} );
        }
        definitions.push_back( std::move(definition) );
    }

    return definitions;
}

std::string GenerateRequest(const SkillDefinition& definition,
                            unsigned msg_uid,
                            int indent)
{
    nlohmann::json json;
    json["msg-type"] = "push-skill";
    json["id"] = msg_uid;

    auto& skill = json["skill"];
    skill["name"] = definition.ID;
    skill["skill-definition-fqn"] = definition.skill_FQN;
    skill["out-attribute"] = nlohmann::json({});
    skill["in-attribute"] = definition.params;

    return json.dump(indent);
}


