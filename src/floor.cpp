#include <map>
#include <ctime>
#include <string>
#include <iostream>

#include "linkedList.h"
#include "floor.h"

/** Thinking from a high level, we need the floor to do a few things:
 * Like a real floor, the floor class should take orders from clients,
 * place them in a queue for the correct ticker, and fulfill orders.
 *
 * Now, here comes a design choice... there are different ways to
 * fulfill orders:
 *
 * 1.	Immediate or Cancel (IOC): This order type requires immediate execution
 *      for the amount available (even if partial) and cancels any remaining unfilled portion.
 *      For example, if there’s not enough buy interest, only part of the order is filled, and
 *      the rest is canceled.
 *
 * 2.   Fill or Kill (FOK): This type demands complete fulfillment immediately or cancellation.
 *      If the entire order can’t be filled instantly, it’s canceled in full. This is often used
 *      in larger orders where partial fulfillment isn’t ideal.
 *
 * 3.	Good ‘Til Canceled (GTC): This order remains active until it’s either fully filled or
 *      canceled by the trader. It allows partial fills over time, which means any unfilled
 *      portion stays open and can be filled whenever there is enough buy or sell interest to
 *      meet the quantity.
 *
 * 4.	All or None (AON): This requires the entire order to be filled in one go; otherwise,
 *      it remains unfilled until the market can accommodate the complete order.
 *
 * 5.	Market Orders: These are executed immediately at the current best available
 *      price. They can be partially filled if the quantity needed isn’t available at
 *      a single price level, with remaining portions filled at the next best prices.
 *
 * 6.	Limit Orders: These execute only at the specified price (or better). They may
 *      allow partial fills, leaving any unfilled portions on the order book until the
 *      price condition is met.
 *
 * We have to pick one and stick with it. Maybe we can specify what the order fulfillment
 * method is when instantiating? But also consider that when implementing, different
 * fulfillment methods may have very different algorithms for matching orders!
 *
 * After we have chosen the method for order fulfillment, we continue to consider how a trading
 * floor works:
 *
 * Orders constantly come in and are pushed to the queue. Simultaneosly, the floor is constantly
 * matching sell orders to buy orders.
 *
 * If an order is fulfilled/cancelled, then we should notify the client accordingly.
 */

float Floor::getPrice(std::string ticker)
{
    std::srand(std::time(0));
    float price = 100.0 + (std::rand() % RAND_MAX) * 100;
    return price;
};

std::string process(char *order_raw)
{
    std::tuple<bool, std::string, unsigned long> order;
    std::optional<std::tuple<bool, std::string, unsigned long>> parsed_order = Floor::parseOrder(order_raw);
    if (parsed_order)
    {
        order = parsed_order.value();
    }
    else
    {
        return "";
    }
    bool action = std::get<bool>(order);
    std::string ticker = std::get<std::string>(order);
    unsigned long quantity = std::get<unsigned long>(order);

    return std::string{};
    // TODO: must check if any of the things are null, if so, then the client has written an invalid order request.

    // TODO: add the order into the orders map.
}
// TODO: contoinuously process orders and send events to user.
//  void addOrder
static std::optional<std::tuple<bool, std::string, unsigned long>> parseOrder(char *content) // TODO: maybe consider throwing an exception?
{
    bool buy;
    std::string ticker;
    unsigned long quantity;

    int counter = 0;

    std::string result;

    for (int i = 0; i < std::strlen(content); i++)
    {
        if (content[i] == ' ')
        {
            if (counter == 0)
            {
                if (result == "buy")
                {
                    buy = true;
                }
                else if ((result == "sell"))
                {
                    buy = false;
                }
                else
                {
                    return {};
                }
                result = "";
                counter += 1;
            }
            else if (counter == 1)
            {
                ticker = result;
                counter += 1;
            }
            else
            {
                try
                {
                    quantity = std::stol(result);
                    return std::make_tuple(buy, ticker, quantity);
                }
                catch (const std::invalid_argument &e)
                {
                    std::cout << "Invalid argument: " << e.what() << "\n";
                    return {};
                }
                catch (const std::out_of_range &e)
                {
                    std::cout << "Out of range: " << e.what() << "\n";
                    return {};
                }
            }
        }
    }
}

void Floor::display()
{
    std::cout << "floor" << std::endl;
};