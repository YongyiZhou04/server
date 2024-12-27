#include "order.h"

std::string Order::getTicker()
{
    return ticker;
}

float Order::getPrice()
{
    return price;
}

unsigned long Order::getQuantity()
{
    return quantity;
}

void Order::setQuantity(unsigned long newQuantity)
{
    quantity = newQuantity;
}

long long Order::getTime()
{
    return time;
}

std::string Order::display()
{
    return ticker + ", " + std::to_string(price) + ", " + std::to_string(quantity) + ", " + std::to_string(time);
}