#include "order_book.h"
#include <algorithm>
#include <chrono>

OrderBook::OrderBook() : nextOrderId_(1) {}

void OrderBook::addLimitOrder(const std::string& symbol,
                              Side side,
                              double price,
                              int quantity) {
    Order o;
    o.id = nextOrderId_++;
    o.symbol = symbol;
    o.side = side;
    o.price = price;
    o.quantity = quantity;
    o.timestamp = std::chrono::steady_clock::now();

    BookQueues& book = books_[symbol];

    if (side == Side::BUY) {
        book.bids.push(o);
    } else {
        book.asks.push(o);
    }

    match(symbol);
}

void OrderBook::match(const std::string& symbol) {
    BookQueues& book = books_[symbol];

    while (!book.bids.empty() && !book.asks.empty()) {
        Order bestBuy = book.bids.top();
        Order bestSell = book.asks.top();

        if (bestBuy.price < bestSell.price) {
            // No hay cruce de precios
            break;
        }

        book.bids.pop();
        book.asks.pop();

        int tradedQty = std::min(bestBuy.quantity, bestSell.quantity);
        double tradePrice = bestSell.price; // o bestBuy.price, es el rango cruzado

        // Registrar trade
        Trade t;
        t.buyId = (bestBuy.side == Side::BUY) ? bestBuy.id : bestSell.id;
        t.sellId = (bestBuy.side == Side::SELL) ? bestBuy.id : bestSell.id;
        t.symbol = symbol;
        t.price = tradePrice;
        t.quantity = tradedQty;
        t.timestamp = std::chrono::steady_clock::now();
        trades_.push_back(t);

        bestBuy.quantity -= tradedQty;
        bestSell.quantity -= tradedQty;

        // Reinsertar si quedan cantidades
        if (bestBuy.quantity > 0) {
            book.bids.push(bestBuy);
        }
        if (bestSell.quantity > 0) {
            book.asks.push(bestSell);
        }
    }
}

std::vector<Order> OrderBook::getTopBuys(const std::string& symbol, int n) const {
    std::vector<Order> result;
    auto it = books_.find(symbol);
    if (it == books_.end()) return result;

    auto bidsCopy = it->second.bids;
    while (!bidsCopy.empty() && (int)result.size() < n) {
        result.push_back(bidsCopy.top());
        bidsCopy.pop();
    }
    return result;
}

std::vector<Order> OrderBook::getTopSells(const std::string& symbol, int n) const {
    std::vector<Order> result;
    auto it = books_.find(symbol);
    if (it == books_.end()) return result;

    auto asksCopy = it->second.asks;
    while (!asksCopy.empty() && (int)result.size() < n) {
        result.push_back(asksCopy.top());
        asksCopy.pop();
    }
    return result;
}

std::vector<Trade> OrderBook::getLastTrades(int n) const {
    std::vector<Trade> result;
    if ((int)trades_.size() <= n) {
        return trades_;
    }
    result.insert(result.end(),
                  trades_.end() - n,
                  trades_.end());
    return result;
}
