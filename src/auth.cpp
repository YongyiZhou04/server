#include "auth.h"

std::string Auth::encrypt(std::string password)
{
    char encrypted_str[crypto_pwhash_STRBYTES];

    if (crypto_pwhash_str(
            encrypted_str, password.c_str(),
            password.length(),
            crypto_pwhash_OPSLIMIT_MODERATE,
            crypto_pwhash_MEMLIMIT_MODERATE) != 0)
    {
        // TODO: might need to replace this with error handling
        std::cout << "error occured during password encryption" << std::endl;
    }
    return std::string(encrypted_str);
}

std::string Auth::authorize(std::string username, std::string password)
{

    if (userPW.find(username) == userPW.end())
    {
        userPW[username] = Auth::encrypt(password);
    }
    else if (crypto_pwhash_str_verify(
                 userPW[username].c_str(),
                 password.c_str(),
                 password.length()) != 0)
    {
        // TODO: might want to deal with this another way
        return "";
    }

    auto now = std::chrono::system_clock::now();
    long long time = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    if ((userToken.find(username) == userToken.end()) || ((userToken[username].getTime() - time) > 43200000))
    {
        std::string token = generateToken("1234567890-=qwertyuiopasdfghjklzxcvbnm,./;'_+!@#$%^&*(){}|:~`QWERTYUIOPASDFGHJKLZXCVBNM", 10);
        Token newToken = Token(token, time);

        while (tokenSet.find(newToken) != tokenSet.end())
        {
            token = generateToken("1234567890-=qwertyuiopasdfghjklzxcvbnm,./;'_+!@#$%^&*(){}|:~`QWERTYUIOPASDFGHJKLZXCVBNM", 10);
            newToken = Token(token, time);
        }

        if ((userToken[username].getTime() - time) > 43200000)
        {
            tokenSet.erase(userToken[username]);
        }

        tokenSet.insert(newToken);
        userToken[username] = newToken;
    }
    return userToken[username].getToken();
}

std::string Auth::generateToken(std::string characters, int length)
{
    std::string token = "";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, characters.length() - 1);

    for (int i = 0; i < length; i++)
    {
        token += characters[dist(gen)];
    }

    return token;
}

bool Auth::verify(Token token)
{
    auto now = std::chrono::system_clock::now();
    long long time = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    return (tokenSet.find(token) != tokenSet.end() && time - token.getTime() < 43200000);
}