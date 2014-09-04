#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <cluster.hpp>
#include <config.hpp>

using namespace std;

int main(int argc, char** argv) {
  const string slurmdPath = "/usr/local/sbin/slurmd";

  if (argc < 2) {
    return EXIT_FAILURE;
  }

  string master(argv[1]);

  Cluster cluster("http://" + master + "/master/state.json");

  nodeList nodes = cluster.nodes();
  for (nodeList::iterator it = nodes.begin(); it != nodes.end(); it++) {
    string& node = *it;
    cout << node << endl;
  }

  Config config("hostname", nodes);

  string temporaryConfigPath = mktemp("");
  ofstream temporaryConfig(temporaryConfigPath);

  cout << config.expand("config.tpl") << endl;

  temporaryConfig.close();

  // TODO(nnielsen): Write new config.

  // TODO(nnielsen): Start slurmctld

  return EXIT_SUCCESS;
}
