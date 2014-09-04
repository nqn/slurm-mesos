#include <stdio.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#include <string>

#include <network.hpp>

using namespace std;

string network::hostnameFromIP(string ip) {
  string out = "";
  in_addr_t addr = inet_addr(ip.c_str());
  if ((int)(addr) == -1) {
    return "";
  }
  struct hostent *hp = gethostbyaddr((char *)&addr, 4, AF_INET);
  if (hp == NULL) {
    return "";
  }

  for (char **p = hp->h_addr_list; *p != 0; p++) {
    struct in_addr in;
    char **q;
    memcpy(&in.s_addr, *p, sizeof (in.s_addr));
    out = hp->h_name;
  }
  
  return out;
}


std::string network::hostname() {
  char hostname[512];
  if (gethostname(hostname, sizeof(hostname)) != 0) {
    return "";
  }
  
  return hostname;
}
