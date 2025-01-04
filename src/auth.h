#include <cryptlib.h>
#include <sha.h>
#include <hex.h>
#include <filters.h>
#include <unordered_map>
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
    std::set<std::string> tokenSet;
    std::set<std::string> currentUsers;

    static std::string encrypt(const std::string &str);
    static std::string generateToken(std::string characters, int length);

public:
    Auth(std::unordered_map<std::string, std::string> userPW = {},
         std::unordered_map<std::string, Token> userToken = {},
         std::set<std::string> tokenSet = std::set<std::string>(),
         std::set<std::string> currentUsers = std::set<std::string>()) : userPW(userPW), userToken(userToken), tokenSet(tokenSet), currentUsers(currentUsers) {}

    std::string authorize(const std::string username, const std::string password);

    bool verify(std::string token);

    bool addUser(const std::string username, const std::string password);

    void deleteToken(std::string token);
};