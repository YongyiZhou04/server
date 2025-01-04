#include "auth.h"

std::string Auth::encrypt(const std::string &input)
{
    CryptoPP::SHA256 sha256;

    // The output will be stored in a byte array
    CryptoPP::byte hash[CryptoPP::SHA256::DIGESTSIZE];

    // Compute the hash of the input
    sha256.CalculateDigest(hash, (const CryptoPP::byte *)input.data(), input.length());

    // Convert the byte array to a hex string
    std::string output;
    CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(output));
    encoder.Put(hash, sizeof(hash));
    encoder.MessageEnd();

    return output;
}

std::string Auth::authorize(const std::string username, const std::string password)
{

    if (userPW.find(username) == userPW.end() || userPW[username] != encrypt(password) || currentUsers.find(username) != currentUsers.end())
    {
        return "";
    }

    auto now = std::chrono::system_clock::now();
    long long time = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    if ((userToken.find(username) == userToken.end()) || ((userToken[username].getTime() - time) > 43200000))
    {
        std::string token = generateToken("1234567890-=qwertyuiopasdfghjklzxcvbnm,./;'_+!@#$%^&*(){}|:~`QWERTYUIOPASDFGHJKLZXCVBNM", 10);
        Token newToken = Token(token, time);

        while (tokenSet.find(newToken.getToken()) != tokenSet.end())
        {
            token = generateToken("1234567890-=qwertyuiopasdfghjklzxcvbnm,./;'_+!@#$%^&*(){}|:~`QWERTYUIOPASDFGHJKLZXCVBNM", 10);
            newToken = Token(token, time);
        }

        if ((userToken[username].getTime() - time) > 43200000)
        {
            tokenSet.erase(userToken[username].getToken());
        }

        tokenSet.insert(newToken.getToken());
        currentUsers.insert(username);
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

bool Auth::verify(std::string token)
{
    return (tokenSet.find(token) != tokenSet.end());
}

bool Auth::addUser(const std::string username, const std::string password)
{
    if (userPW.find(username) != userPW.end())
    {
        return false;
    }

    userPW[username] = encrypt(password);
    return true;
}