#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <sstream>

std::string getMsg(std::istringstream &iss);
bool isValidChar(const std::string& line, const std::string& forbidden  = "");
void ft_toupper(std::string &str);
void ft_tolower(std::string &str);

#endif
