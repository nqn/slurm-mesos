#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

#include <cluster.hpp>
#include <config.hpp>
#include <network.hpp>

using namespace std;

int main(int argc, char** argv) {
  const string slurmdPath = "/usr/local/sbin/slurmctld";
  const string statePath = "/master/state.json";
  const string configTemplatePath = "config.tpl";
  const string daemonFlags = " -D -f ";

  if (argc < 2) {
    return EXIT_FAILURE;
  }

  string master(argv[1]);

  Cluster cluster("http://" + master + statePath);

  string hostname = network::hostname();
  cout << "Using hostname '" << hostname << "'" << endl;

  nodeList nodes = cluster.nodes();

  nodes.push_back(network::hostnameFromIP("10.221.11.55"));

  Config config(hostname, nodes);

  std::string path = config.expandToTemporaryFile(configTemplatePath);
  cout << "Wrote config to " << path << endl;

  // Start slurmctld (backed by Mesos Scheduler).
  char line[512];
  string command = slurmdPath + daemonFlags + string(path);
  FILE* fpin = popen(command.c_str(), "r");
  if(fpin) {
    while (fgets(line, sizeof(line), fpin) != NULL) {
      printf("> %s", line);
    }
  }
  pclose(fpin);

  return EXIT_SUCCESS;
}
