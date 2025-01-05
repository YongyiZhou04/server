#pragma once

#include <algorithm>
#include <ctime>
#include <string>
#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <unordered_map>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>

#include "priorityLinkedList.h"
#include "order.h"
#include "util.h"

class Floor
{
private:
    std::unordered_map<std::string, float> prices;
    std::unordered_map<std::string, unsigned long> quantity;
    std::unordered_map<std::string, std::unique_ptr<PriorityLinkedList<Order, long long>>> buyOrders;
    std::unordered_map<std::string, std::unique_ptr<PriorityLinkedList<Order, long long>>> sellOrders;
    std::unordered_map<std::string, std::pair<std::thread, std::atomic<bool>>> tickerThreads;
    std::mutex mapMutex;
    std::condition_variable cv;

public:
    Floor(std::unordered_map<std::string, float> prices = {},
          std::unordered_map<std::string, unsigned long> quantity = {}) : prices(prices), quantity(quantity)
    {
        buyOrders = std::unordered_map<std::string, std::unique_ptr<PriorityLinkedList<Order, long long>>>{};
        ;
        sellOrders = std::unordered_map<std::string, std::unique_ptr<PriorityLinkedList<Order, long long>>>{};
        ;
    }

    // ~Floor()
    // {
    //     stopTickerThread(""); // Stop all threads
    // }

    // // Prevent copying
    // Floor(const Floor &) = delete;
    // Floor &operator=(const Floor &) = delete;

    // // Allow moving if needed
    // Floor(Floor &&) = default;
    // Floor &operator=(Floor &&) = default;

    float getPrice(std::string ticker);

    static std::optional<std::tuple<bool, std::string, unsigned long>> parseOrder(std::vector<std::string> content);

    std::string process(int user_fd, std::vector<std::string> rawOrder);

    void startTickerThread(const std::string ticker);

    void stopTickerThread(const std::string ticker = "");

    void matchOrder(const std::string ticker, std::atomic<bool> &running);

    void display();
};