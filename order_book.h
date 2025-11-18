#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include "order.h"
#include <queue>
#include <vector>
#include <unordered_map>
#include <string>

class OrderBook {
public:
    OrderBook();

    void addLimitOrder(const std::string& symbol,
                       Side side,
                       double price,
                       int quantity);

    std::vector<Order> getTopBuys(const std::string& symbol, int n) const;
    std::vector<Order> getTopSells(const std::string& symbol, int n) const;

    std::vector<Trade> getLastTrades(int n) const;

private:
    struct BuyCompare {
        bool operator()(const Order& a, const Order& b) const {
            if (a.price == b.price) {
                // Más antiguo tiene mayor prioridad → si a es más nuevo, "es menor"
                return a.timestamp > b.timestamp;
            }
            // Queremos precio más alto primero
            return a.price < b.price;
        }
    };

    struct SellCompare {
        bool operator()(const Order& a, const Order& b) const {
            if (a.price == b.price) {
                // Más antiguo tiene mayor prioridad → si a es más nuevo, "es menor"
                return a.timestamp > b.timestamp;
            }
            // Queremos precio más bajo primero
            return a.price > b.price;
        }
    };

    struct BookQueues {
        std::priority_queue<Order, std::vector<Order>, BuyCompare> bids;
        std::priority_queue<Order, std::vector<Order>, SellCompare> asks;
    };

    std::unordered_map<std::string, BookQueues> books_;
    std::vector<Trade> trades_;

    int nextOrderId_;

    void match(const std::string& symbol);
};

#endif // ORDER_BOOK_H
