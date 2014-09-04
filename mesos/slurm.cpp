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
  const string statePath = "/master/state.json";

  if (argc < 2) {
    return EXIT_FAILURE;
  }

  string master(argv[1]);

  Cluster cluster("http://" + master + statePath);

  nodeList nodes = cluster.nodes();
  Config config("hostname", nodes);

  // Write config to temporary file.
  char* path = tmpnam(NULL);
  if (!path) {
    return EXIT_FAILURE;
  }
  ofstream temporaryConfig(path);
  temporaryConfig << config.expand("config.tpl") << endl;
  temporaryConfig.close();

  cout << "Wrote config to " << path << endl;

  // Start slurmctld (backed by Mesos Scheduler).

  return EXIT_SUCCESS;
}
