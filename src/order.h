#pragma once

#include <string>

class Order
{
private:
    std::string ticker;
    float price;
    unsigned long quantity;
    long long time;

public:
    Order(std::string ticker = "", float price = 0.0f, unsigned long quantity = 0, long long time = 0) : ticker(ticker), price(price), quantity(quantity), time(time) {};

    bool operator==(const Order &other) const
    {
        return (ticker == other.ticker && price == other.price && quantity == other.quantity && time == other.time);
    }

    std::string getTicker();

    float getPrice();

    unsigned long getQuantity();

    void setQuantity(unsigned long newQuantity);

    long long getTime();

    std::string display();
};