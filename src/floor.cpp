#include <map>

#include "linkedList.h"

class Floor
{
private:
    std::map<char *, float> prices;
    std::map<char *, unsigned long> quantity;
    std::map<char *, LinkedList<int>> orders;

public:
    Floor(std::map<char *, float> prices = {},
          std::map<char *, unsigned long> quantity = {},
          std::map<char *, LinkedList<int>> orders = {})
        : prices(prices), quantity(quantity), orders(orders) {};
};