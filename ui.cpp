#include "ui.h"
#include <iostream>
#include <iomanip>

static const std::string RED   = "\033[31m";
static const std::string GREEN = "\033[32m";
static const std::string CYAN  = "\033[36m";
static const std::string RESET = "\033[0m";

TerminalUI::TerminalUI(OrderBook& book) : book_(book) {}

void TerminalUI::clearScreen() {
    std::cout << "\033[2J\033[H";
}

void TerminalUI::render(const std::string& symbol) {
    clearScreen();

    auto bids = book_.getTopBuys(symbol, 10);
    auto asks = book_.getTopSells(symbol, 10);
    auto trades = book_.getLastTrades(5);

    std::cout << CYAN << "=== Continuous Limit Order Book (CLOB) ===" << RESET << "\n";
    std::cout << "Symbol: " << symbol << "\n\n";

    std::cout << GREEN << "  BUY ORDERS" << RESET
              << "                       "
              << RED << "SELL ORDERS" << RESET << "\n";

    std::cout << std::left
              << std::setw(10) << "Qty"
              << std::setw(10) << "Price"
              << " | "
              << std::setw(10) << "Price"
              << std::setw(10) << "Qty"
              << "\n";

    std::cout << "---------------------------------------------\n";

    size_t rows = std::max(bids.size(), asks.size());
    for (size_t i = 0; i < rows; ++i) {
        if (i < bids.size()) {
            std::cout << GREEN
                      << std::setw(10) << bids[i].quantity
                      << std::setw(10) << std::fixed << std::setprecision(2) << bids[i].price
                      << RESET;
        } else {
            std::cout << std::setw(10) << " "
                      << std::setw(10) << " ";
        }

        std::cout << " | ";

        if (i < asks.size()) {
            std::cout << RED
                      << std::setw(10) << std::fixed << std::setprecision(2) << asks[i].price
                      << std::setw(10) << asks[i].quantity
                      << RESET;
        }

        std::cout << "\n";
    }

    std::cout << "\n" << CYAN << "Last trades:" << RESET << "\n";
    for (const auto& t : trades) {
        std::cout << "Trade " << t.symbol
                  << " qty=" << t.quantity
                  << " price=" << std::fixed << std::setprecision(2) << t.price
                  << " (buyId=" << t.buyId
                  << ", sellId=" << t.sellId << ")\n";
    }

    std::cout.flush();
}
