#include <sodium.h>
#include <map>
#include <string>
#include <iostream>
#include <chrono>
#include <random>
#include <set>

#include "token.h"

class Auth
{
private:
    std::unordered_map<std::string, std::string> userPW;
    std::unordered_map<std::string, Token> userToken;
    std::set<Token> tokenSet;

    static std::string encrypt(std::string str);
    static std::string generateToken(std::string characters, int length);

public:
    Auth() = default;

    std::string authorize(std::string username, std::string password);

    bool verify(Token token);

    void deleteToken(std::string token);
};