#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <chrono>

enum class Side {
    BUY,
    SELL
};

struct Order {
    int id;
    std::string symbol;
    Side side;
    double price;
    int quantity;
    std::chrono::steady_clock::time_point timestamp;
};

struct Trade {
    int buyId;
    int sellId;
    std::string symbol;
    double price;
    int quantity;
    std::chrono::steady_clock::time_point timestamp;
};

#endif // ORDER_H
