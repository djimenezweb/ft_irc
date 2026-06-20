#include "utils.hpp"

std::string getMsg(std::istringstream &iss)
{
    std::string resto;

    std::getline(iss, resto);
    if (!resto.empty() && resto[0] == ' ')
        resto.erase(0, 1);
    if (!resto.empty() && resto[0] == ':')
        resto.erase(0, 1);
    while (!resto.empty() && resto[0] == ' ')
    {
        resto.erase(0, 1);
    }

    return (resto);
}

void ft_toupper(std::string &str)
{
    for (unsigned long  i = 0; i < str.length(); i++)
        str[i] = toupper(str[i]);
}

void ft_tolower(std::string &str)
{
    for (unsigned long  i = 0; i < str.length(); i++)
        str[i] = tolower(str[i]);
}

bool isValidChar(const std::string &line, const std::string &forbidden)
{
    for (size_t i = 0; i < line.length(); i++)
    {
        unsigned char c = line[i];
        if (c < 32 || c == 127)
            return false;
    }
    if (!forbidden.empty() && line.find_first_of(forbidden) != std::string::npos)
        return false;
    return true;
}
