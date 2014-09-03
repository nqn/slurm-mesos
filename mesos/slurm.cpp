#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <vector>
#include <iostream>

#include <cluster.hpp>
#include <config.hpp>

using namespace std;

int main(int argc, char** argv) {
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

  cout << config.expand("config.tpl") << endl;

  return EXIT_SUCCESS;
}
