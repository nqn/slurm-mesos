#include <string>
#include <vector>

class Config {
public:
  Config(
      std::string primaryControlDaemon,
      std::vector<std::string> nodes 
  );

  std::string expand(std::string templatePath);

private:
  std::string primaryControlDaemon_;
  std::vector<std::string> nodes_;
};
