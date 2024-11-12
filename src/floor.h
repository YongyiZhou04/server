#pragma once

#include <map>
#include "linkedList.h"

class Floor
{
private:
    std::map<std::string, float> prices;
    std::map<std::string, unsigned long> quantity;
    std::map<std::string, LinkedList<int>> orders;

public:
    Floor() = default;

    Floor(std::map<std::string, float> prices,
          std::map<std::string, unsigned long> quantity,
          std::map<std::string, LinkedList<int>> orders);

    float getPrice(std::string ticker);

    static std::tuple<bool, std::string, unsigned long> parseOrder(char *content);

    void display();

    std::string process(char *order);
};