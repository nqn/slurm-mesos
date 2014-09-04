#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

#include <cluster.hpp>
#include <network.hpp>

#include <rapidjson/document.h>
#include <curl/curl.h>

using namespace std;

struct MemoryStruct {
  char *memory;
  size_t size;
} chunk;
 
static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
    /* out of memory! */ 
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}

Cluster::Cluster(string masterUrl) {
  CURL *curl;
  CURLcode res;

  chunk.memory = (char*)malloc(1);
  chunk.size = 0;
 
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, masterUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
 
    res = curl_easy_perform(curl);
    if(res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    } else {
      const char* json = chunk.memory;

      rapidjson::Document document;
      document.Parse<0>(json);

      const rapidjson::Value& slaves = document["slaves"];
      for (rapidjson::SizeType i = 0; i < slaves.Size(); i++) {
        const rapidjson::Value& slave = slaves[i];

        string node(slave["hostname"].GetString());

        // Resolve hostname in case of IP.
        string nodeHostname = network::hostnameFromIP(node);

        nodes_.push_back(nodeHostname);
      }
    }
 
    curl_easy_cleanup(curl);
  }
}

vector<string> Cluster::nodes() {
  return nodes_;
}

