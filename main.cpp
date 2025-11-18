#include "order_book.h"
#include "ui.h"

#include <random>
#include <thread>
#include <chrono>

int main() {
    OrderBook book;
    TerminalUI ui(book);

    std::string symbol = "MSFT";

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> priceDist(95.0, 105.0);
    std::uniform_int_distribution<int> qtyDist(10, 500);
    std::uniform_int_distribution<int> sideDist(0, 1);
    std::uniform_int_distribution<int> ordersPerTick(1, 3);

    while (true) {
        int nOrders = ordersPerTick(rng);
        for (int i = 0; i < nOrders; ++i) {
            Side side = (sideDist(rng) == 0) ? Side::BUY : Side::SELL;
            double price = priceDist(rng);
            int qty = qtyDist(rng);
            book.addLimitOrder(symbol, side, price, qty);
        }

        ui.render(symbol);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    return 0;
}
