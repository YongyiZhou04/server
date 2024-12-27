#pragma once
#include <atomic>
#include <iostream>
#include <cstring>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include <thread>
#include <algorithm>
#include <mutex>
#include <poll.h>
#include <format>

#include "priorityLinkedList.h"
#include "floor.h"
#include "order.h"

const int PORT = 8080;
std::atomic<bool> running{true};
