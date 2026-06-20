#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "Server.hpp"
# include "Channel.hpp"
# include "Client.hpp"

class Client;
class Channel;
class Server;

void cmdCap(std::istringstream &iss, Client &client);
void cmdJoin(Server &s, Client& client, std::string line);
void cmdNick(Server& s, Client &client, std::istringstream &iss);
void cmdPass(std::istringstream &iss, Client &client, std::string pass);
void cmdTopic(Server &s, Client &client, std::string line);
void cmdUser(std::istringstream &iss, Client& client);
void cmdPrivmsg(Server &s, Client &c, std::string &line);
void cmdNotice(Server &s, Client &c, std::string &line);
void cmdPing(Client &c, std::string &line);
void cmdInvite(Server &s, Client &c, std::string &line);
void cmdKick(Server &s, Client& cli, std::string line);
void cmdWho(Server& s, Client& client, std::string& line);
void cmdQuit(Server &s, Client &cli, std::string line);
void cmdMode(Server &s, Client &c, std::string &line);
void cmdList(Server& s, Client& client, std::string& line);
void cmdNames(Server& s, Client& client, std::string& line);
void cmdPart(Server &s, Client& client, std::string line);

void namesCommand(Server&s, Channel& ch, Client& client);
void validate_command(Server &s, const std::string& cmd, Client &client);
void commandParse(const std::string& line, Client& client, std::string pass, Server &s);
bool checkName(std::string name);

#endif
