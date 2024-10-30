#include <map>

class Floor
{
private:
    std::map<char *, float> prices;
    std::map<char *, unsigned long> quantity;
    std::map<char *, unsigned long> quantity;

public:
    Floor(const std::map<char *, float> &prices = {}, const std::map<char *, unsigned long> &quantity = {})
        : prices(prices), quantity(quantity) {};
};