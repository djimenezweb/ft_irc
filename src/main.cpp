#include "Server.hpp"
#include "definitions.hpp"
#include "utils.hpp"

bool run_server = true;

int validate_args(char **argv)
{
    std::stringstream ss(argv[1]);
    int port;
    char leftover;

    if (!(ss >> port) || ss >> leftover)
        return 0;
    if (port < 0 || port > 65535)
        return 0;

    size_t index = 0;
    std::string pass(argv[2]);
    while (index < pass.length())
    {
        if (isspace(pass[index]) || !isValidChar(pass))
            return 0;
        index++;
    }
    return 1;
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return (1);
    }
    if (!validate_args(argv))
    {
        std::cerr << RED_BOLD << "error:" << RESET << " Invalid argument" << std::endl;
        return (1);
    }

    try
    {
        Server serv(argv[1], argv[2]);
        serv.init();
        serv.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return (1);
    }
    return (0);
}
