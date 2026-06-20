#include "Client.hpp"
#include "utils.hpp"

void cmdCap(std::istringstream &iss, Client &client)
{
    std::string subcommand;
    if (!(iss >> subcommand))
        return (client.sendMsg(ERR_NEEDMOREPARAMS(client.getNickname(), client.getCliCmd())));
    ft_toupper(subcommand);
    if (subcommand == "LS")
        client.sendMsg(my_serv_name" CAP * LS :");
    else if (subcommand == "REQ")
    {
        std::string resto;
        std::getline(iss, resto);
        client.sendMsg(my_serv_name" CAP * NAK " +  resto);
    }
    else if (subcommand == "END")
        client.setAuthenticated(true);
}
