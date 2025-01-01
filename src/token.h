class Token
{
private:
    std::string token;
    long long time;

public:
    Token(std::string token, long long time) : token(token), time(time) {};

    long long getTime()
    {
        return time;
    }

    std::string getToken()
    {
        return token;
    }

    bool operator==(const Token &other) const { return (this->time == other.time) && (this->token == other.token); }
};