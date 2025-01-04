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
    // std::srand(std::time(0));
    // float price = 100.0f + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * 100.0f;
    // return price;
    return 100.0f;
}

std::string Floor::process(int user_fd, std::vector<std::string> rawOrder)
{
    std::cout << "floor.cpp - process => process called" << std::endl;

    Order order;
    std::optional<std::tuple<bool, std::string, unsigned long>> parsed_order = Floor::parseOrder(rawOrder);
    bool buy;
    if (parsed_order)
    {
        std::string ticker = std::get<std::string>(parsed_order.value());
        float price = Floor::getPrice(ticker);
        unsigned long quantity = std::get<unsigned long>(parsed_order.value());

        buy = std::get<bool>(parsed_order.value());

        auto now = std::chrono::system_clock::now();
        long long time = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

        order = Order(ticker, price, quantity, time, user_fd);
        std::cout << "floor.cpp - process => order parsed successfully: " << order.display() << std::endl;
    }
    else
    {
        std::cout << "floor.cpp - process => failed to parse order" << std::endl;
        return std::string{};
    }
    // TODO: add the order into the orders map.

    std::unordered_map<std::string, std::unique_ptr<PriorityLinkedList<Order, long long>>> &orders = buy ? buyOrders : sellOrders;

    if (orders.find(order.getTicker()) == orders.end())
    {
        auto list = std::make_unique<PriorityLinkedList<Order, long long>>();
        orders[order.getTicker()] = std::move(list);
    }
    orders[order.getTicker()]->insert(order, order.getTime());

    if (tickerThreads.find(order.getTicker()) == tickerThreads.end())
    {
        startTickerThread(order.getTicker());
        std::cout << "floor.cpp - process => " + order.getTicker() << " thread started" << std::endl;
    }

    std::string orderType = buy ? "buy" : "sell";

    return "processing " + orderType + " order:\nTicker: " + order.getTicker() + "\nQuantity: " + std::to_string(order.getQuantity()) + "\nTime: " + std::to_string(order.getTime()) + "\n";
}
// TODO: contoinuously process orders and send events to user.
//  void addOrder
std::optional<std::tuple<bool, std::string, unsigned long>> Floor::parseOrder(std::vector<std::string> content) // TODO: maybe consider throwing an exception?
{
    std::cout << "floor.cpp - parseOrder => parseOrder called" << std::endl;

    bool buy;
    std::string ticker;
    unsigned long quantity;

    for (const std::string &str : content)
    {
        std::cout << "\"" << str << "\"" << std::endl;
    }

    if (content.size() != 3)
    {
        std::cout << "floor.cpp - split => content length is not correct" << std::endl;
        return {};
    }

    if (content[0] != "buy" && content[0] != "sell")
    {
        std::cout << "floor.cpp - split => first element is neither buy nor sell" << std::endl;
        return {};
    }
    else
    {
        buy = content[0] == "buy" ? true : false;
    }

    ticker = content[1];

    try
    {
        quantity = stol(content[2]);
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << "Invalid argument: " << e.what() << std::endl;
    }
    catch (const std::out_of_range &e)
    {
        std::cout << "Out of range: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "An unknown error occurred." << std::endl;
    }

    return std::make_tuple(buy, ticker, quantity);
}

void Floor::startTickerThread(const std::string ticker)
{
    // std::lock_guard<std::mutex> lock(mapMutex);
    if (tickerThreads.find(ticker) == tickerThreads.end())
    {
        tickerThreads[ticker] = std::make_pair(
            std::thread(&Floor::matchOrder, this, ticker, std::ref(tickerThreads[ticker].second)),
            true // Initial running flag
        );
    }
}

void Floor::stopTickerThread(const std::string ticker)
{
    std::vector<std::string> threads;
    {
        if (ticker.empty())
        {
            for (auto &pair : tickerThreads)
            {
                std::cout << "floor.cpp - stopTickerThread => ticker empty, adding ticker:  " << pair.first << std::endl;
                threads.push_back(pair.first);
            }
        }
        else
        {
            std::cout << "floor.cpp - stopTickerThread => adding ticker:  " << ticker << std::endl;
            threads.push_back(ticker);
        }
    }

    for (std::string &ticker : threads)
    {
        std::thread threadToJoin;
        {
            // std::lock_guard<std::mutex> lock(mapMutex);
            auto it = tickerThreads.find(ticker);
            if (it != tickerThreads.end())
            {
                it->second.second = false; // Signal thread to stop
                threadToJoin = std::move(it->second.first);
                tickerThreads.erase(it);
            }
            else
            {
                std::cout << "No thread exists for ticker: " << ticker << std::endl;
            }
        }
        if (threadToJoin.joinable())
        {
            std::cout << "floor.cpp - stopTickerThread => waiting thread to join" << std::endl;
            threadToJoin.join(); // Wait for thread to finish
        }
    }
}

void Floor::matchOrder(const std::string ticker, std::atomic<bool> &running)
{
    while (running)
    {

        if (!running)
            break; // Exit if stop signal is received

        std::shared_ptr<Node<Order, long long>> buyOrder = buyOrders.find(ticker) != buyOrders.end() ? buyOrders[ticker]->head : nullptr;
        std::shared_ptr<Node<Order, long long>> sellOrder = sellOrders.find(ticker) != sellOrders.end() ? sellOrders[ticker]->head : nullptr;

        std::string buyMessage;
        std::string sellMessage;

        while (sellOrder != nullptr && buyOrder != nullptr)
        {
            float sellPrice = sellOrder->val.getPrice();
            float buyPrice = buyOrder->val.getPrice();
            // TODO: you may wnat to not use the FD but actual userID in the check
            if (sellPrice <= buyPrice && sellOrder->val.getUserFD() != buyOrder->val.getUserFD())
            {
                unsigned long sellQuantity = sellOrder->val.getQuantity();
                unsigned long buyQuantity = buyOrder->val.getQuantity();
                if (sellQuantity == buyQuantity)
                {
                    buyOrders[ticker]->remove(buyOrder);
                    sellOrders[ticker]->remove(sellOrder);
                }
                else if (sellQuantity > buyQuantity)
                {
                    sellOrder->val.setQuantity(sellQuantity - buyQuantity);
                    buyOrders[ticker]->remove(buyOrder);
                }
                else
                {
                    buyOrder->val.setQuantity(buyQuantity - sellQuantity);
                    sellOrders[ticker]->remove(sellOrder);
                }

                buyMessage = std::to_string(std::min(sellQuantity, buyQuantity)) + " buy orders of " + ticker + " filled\n";
                sellMessage = std::to_string(std::min(sellQuantity, buyQuantity)) + " sell orders of " + ticker + " filled\n";

                std::cout << "successfully matched " << buyOrder->val.display() << " with " << sellOrder->val.display() << std::endl;
                break;
            }
            else
            {
                if (sellOrder->val.getTime() < buyOrder->val.getTime())
                {
                    std::cout << "floor.cpp - matchOrder => moving to next buy order" << std::endl;
                    buyOrder = buyOrder->next;
                    if (buyOrder == nullptr)
                    {
                        sellOrder = sellOrder->next;
                        buyOrder = buyOrders[ticker]->head;
                    }
                }
                else
                {
                    std::cout << "floor.cpp - matchOrder => moving to next sell order" << std::endl;
                    sellOrder = sellOrder->next;
                    if (sellOrder == nullptr)
                    {
                        buyOrder = buyOrder->next;
                        sellOrder = sellOrders[ticker]->head;
                    }
                }
            }
        }

        if (buyMessage.length() != 0 && sellMessage.length() != 0)
        {
            int buyUserFd = buyOrder->val.getUserFD();
            int sellUserFd = sellOrder->val.getUserFD();

            if (buyUserFd > 0 && sellUserFd > 0)
            {
                int buyvalsent = send(buyUserFd, buyMessage.c_str(), buyMessage.size(), 0);
                if (buyvalsent <= 0)
                {
                    std::cerr << "error in sending response to buyer";
                }
                int sellvalsent = send(sellUserFd, sellMessage.c_str(), sellMessage.size(), 0);
                if (sellvalsent <= 0)
                {
                    std::cerr << "error in sending response to seller";
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void Floor::display()
{
    std::cout << "floor" << std::endl;
};