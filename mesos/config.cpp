#include <iostream>
#include <sstream>

#include <ctemplate/template.h>

#include <config.hpp>

using namespace std;

Config::Config(
    std::string primaryControlDaemon,
    std::vector<std::string> nodes 
) : primaryControlDaemon_(primaryControlDaemon),
    nodes_(nodes) {}


std::string Config::expand(std::string templatePath) {
  ctemplate::TemplateDictionary dict("config");
  dict["PRIMARY_CONTROL_DAEMON"] = primaryControlDaemon_;

  std::stringstream ss;
  for(size_t i = 0; i < nodes_.size(); ++i) {
    if(i != 0)
      ss << ",";
    ss << nodes_[i];
  }
  std::string nodeString = ss.str();

  dict["COMPUTE_NODES"] = nodeString;
  
  string output;
  ctemplate::ExpandTemplate(templatePath, ctemplate::DO_NOT_STRIP, &dict, &output);
  
  return output;
}
