#include <string>
#include <vector>

typedef std::vector<std::string> nodeList;

class Cluster {
public:
  Cluster(std::string masterUrl);

  nodeList nodes();

private:
  std::vector<std::string> nodes_;
};
