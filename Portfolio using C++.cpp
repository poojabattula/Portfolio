#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

class Stock {
public:
    string symbol;
    int quantity;
    double price;

    Stock(string sym, int qty, double pr) : symbol(sym), quantity(qty), price(pr) {}

    void display() const {
        cout << "Symbol: " << symbol
             << ", Quantity: " << quantity
             << ", Price: $" << price
             << ", Total Value: $" << quantity * price << endl;
    }

    string to_string() const {
        return symbol + " " + std::to_string(quantity) + " " + std::to_string(price);
    }
};

class Portfolio {
private:
    vector<Stock> stocks;

public:
    void addStock(const Stock& stock) {
        stocks.push_back(stock);
        cout << "Stock added successfully.\n";
    }

    void showPortfolio() const {
        if (stocks.empty()) {
            cout << "Portfolio is empty.\n";
            return;
        }
        double total = 0;
        cout << "\n--- Your Portfolio ---\n";
        for (const auto& stock : stocks) {
            stock.display();
            total += stock.quantity * stock.price;
        }
        cout << "Total Portfolio Value: $" << total << "\n\n";
    }

    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (!file) {
            cerr << "Failed to open file for writing.\n";
            return;
        }
        for (const auto& stock : stocks) {
            file << stock.to_string() << endl;
        }
        cout << "Portfolio saved to " << filename << "\n";
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cerr << "Failed to open file for reading.\n";
            return;
        }
        stocks.clear();
        string symbol;
        int quantity;
        double price;
        while (file >> symbol >> quantity >> price) {
            stocks.emplace_back(symbol, quantity, price);
        }
        cout << "Portfolio loaded from " << filename << "\n";
    }
};

// Main menu
int main() {
    Portfolio myPortfolio;
    int choice;

    while (true) {
        cout << "\n--- Portfolio Tracker ---\n";
        cout << "1. Add Stock\n";
        cout << "2. View Portfolio\n";
        cout << "3. Save Portfolio\n";
        cout << "4. Load Portfolio\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            string symbol;
            int qty;
            double price;
            cout << "Enter stock symbol: ";
            cin >> symbol;
            cout << "Enter quantity: ";
            cin >> qty;
            cout << "Enter price: ";
            cin >> price;
            myPortfolio.addStock(Stock(symbol, qty, price));
        }
        else if (choice == 2) {
            myPortfolio.showPortfolio();
        }
        else if (choice == 3) {
            myPortfolio.saveToFile("portfolio.txt");
        }
        else if (choice == 4) {
            myPortfolio.loadFromFile("portfolio.txt");
        }
        else if (choice == 5) {
            cout << "Exiting...\n";
            break;
        }
        else {
            cout << "Invalid choice, try again.\n";
        }
    }

    return 0;
}
