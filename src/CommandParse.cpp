#include "Server.hpp"
#include "Client.hpp"
#include "commands.hpp"
#include "utils.hpp"

int command_level(std::string cmd)
{
    if(cmd == "KICK")
        return(1);
    else if(cmd == "INVITE")
        return(2);
    else if(cmd == "TOPIC")
        return(3);
    else if(cmd == "MODE")
        return(4);
    else if(cmd == "JOIN")
        return(5);
    else if(cmd == "PRIVMSG")
        return(6);
    else if(cmd == "PART")
        return(7);
    else if(cmd == "PING")
        return(8);
    else if (cmd == "NICK")
        return(9);
    else if(cmd == "WHO")
        return(10);
    else if (cmd == "USER")
        return (11);
    else if (cmd == "PASS")
        return (12);
    else if (cmd == "QUIT")
        return (13);
    else if(cmd == "NOTICE")
        return(14);
    else if(cmd == "LIST")
        return(15);
    else if(cmd == "NAMES")
        return(16);
    return(0);
}

void validate_command(Server &s, const std::string& cmd, Client &client)
{
    if (cmd.empty())
        return;
    std::istringstream str(cmd);
    std::string command;
    std::string line;

    str >> command;
    client.setCliCmd(command);
    std::getline(str, line);
    if(!line.empty() && line[0] == ' ')
        line = line.substr(1);
    ft_toupper(command);
    int level = command_level(command); 

    if(command.empty())
        return ;
    std::istringstream iss(line);
    switch(level)
    {
        case 1:     cmdKick(s, client, line);       break;
        case 2:     cmdInvite(s, client, line);     break;
        case 3:     cmdTopic(s, client, line);      break;
        case 4:     cmdMode(s, client, line);       break;
        case 5:     cmdJoin(s, client, line);       break;
        case 6:     cmdPrivmsg(s, client, line);    break;
        case 7:     cmdPart(s, client, line);       break;
        case 8:     cmdPing(client, line);          break;
        case 9:     cmdNick(s, client, iss);        break;
        case 10:    cmdWho(s, client, line);        break;
        case 11:    cmdUser(iss, client);           break;
        case 12:    cmdPass(iss, client, "");       break;
        case 13:    cmdQuit(s, client, line);       break;
        case 14:    cmdNotice(s, client, line);     break;
        case 15:    cmdList(s, client, line);       break;
        case 16:    cmdNames(s, client, line);      break;
        default:
            client.sendMsg(ERR_UNKNOWNCOMMAND(client.getNickname(), client.getCliCmd()));
            break;
    }
}

void commandParse(const std::string& line, Client& client, std::string pass, Server &s)
{
    if (line.empty())
        return ;
    std::istringstream iss(line);
    std::string command;

    iss >> command;
    ft_toupper(command);
    if (command == "PASS")
        cmdPass(iss, client, pass);
    else if (command == "NICK" && client.getHasPass())
        cmdNick(s, client, iss);
    else if (command == "USER" && client.getHasPass())
        cmdUser(iss, client);
    else if (command == "CAP")
        cmdCap(iss, client);
}
