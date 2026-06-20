#ifndef BOT_HPP
#define BOT_HPP

#include <fstream>
#include <string>
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <set>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <map>
#include <cstdlib>
#include <curl/curl.h>

struct Config
{
    std::string groq_key;
    std::string groq_url;
    std::string groq_model;
    std::string host;
    std::string port;
    std::string password;
    std::string bot_nick;
};

Config loadEnv(const std::string& path);
std::string solverAI(const Config& cfg, std::string& question);

#endif
