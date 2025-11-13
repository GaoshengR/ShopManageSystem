#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <vector>
#include <string>
#include <algorithm>
#include "User.h"
#include "Product.h"
#include "Order.h"

/**
 * @brief 数据库管理类 - 内存数据库
 */
class DatabaseManager {
private:
    std::vector<User> users;
    std::vector<Product> products;
    std::vector<Order> orders;

public:
    DatabaseManager() {
        initializeSampleData();
    }

    void initializeSampleData() {
        // 初始化用户
        users.push_back(User("admin", "admin123", "admin", "admin@shop.com", "13800138000"));
        users.push_back(User("user1", "123456", "customer", "user1@email.com", "13900139000"));
        users.push_back(User("user2", "123456", "customer", "user2@email.com", "13900139001"));

        // 初始化商品（部分商品默认下架）
        products.push_back(Product("P001", "iPhone 15", "电子产品", 5999.00, 50, "最新款苹果手机", true));
        products.push_back(Product("P002", "华为Mate 60", "电子产品", 4999.00, 30, "华为旗舰手机", true));
        products.push_back(Product("P003", "牛奶", "食品", 5.50, 200, "纯牛奶250ml", true));
        products.push_back(Product("P004", "面包", "食品", 8.00, 150, "新鲜烘焙面包", false));  // 下架商品
        products.push_back(Product("P005", "T恤", "服装", 59.00, 100, "纯棉短袖T恤", true));
        products.push_back(Product("P006", "旧款手机", "电子产品", 1999.00, 10, "旧款清仓", false));  // 下架商品
    }

    // 用户管理（原有方法保持不变）
    bool addUser(const User& user) {
        if (getUser(user.getUsername()) != nullptr) {
            return false;
        }
        users.push_back(user);
        return true;
    }

    User* getUser(const std::string& username) {
        for (auto& user : users) {
            if (user.getUsername() == username) {
                return &user;
            }
        }
        return nullptr;
    }

    std::vector<User> getAllUsers() {
        return users;
    }

    bool userExists(const std::string& username) {
        return getUser(username) != nullptr;
    }

    bool updateUser(const User& user) {
        User* existingUser = getUser(user.getUsername());
        if (existingUser) {
            *existingUser = user;
            return true;
        }
        return false;
    }

    // 商品管理 - 新增状态相关方法
    bool addProduct(const Product& product) {
        if (getProduct(product.getId()) != nullptr) {
            return false;
        }
        products.push_back(product);
        return true;
    }

    Product* getProduct(const std::string& productId) {
        for (auto& product : products) {
            if (product.getId() == productId) {
                return &product;
            }
        }
        return nullptr;
    }

    // 获取所有商品（包括下架的）
    std::vector<Product> getAllProducts() {
        return products;
    }

    // 只获取上架的商品
    std::vector<Product> getActiveProducts() {
        std::vector<Product> result;
        for (const auto& product : products) {
            if (product.getIsActive()) {
                result.push_back(product);
            }
        }
        return result;
    }

    // 获取下架的商品
    std::vector<Product> getInactiveProducts() {
        std::vector<Product> result;
        for (const auto& product : products) {
            if (!product.getIsActive()) {
                result.push_back(product);
            }
        }
        return result;
    }

    std::vector<Product> getProductsByCategory(const std::string& category) {
        std::vector<Product> result;
        for (const auto& product : products) {
            if (product.getCategory() == category && product.getIsActive()) {
                result.push_back(product);
            }
        }
        return result;
    }

    std::vector<Product> searchProducts(const std::string& keyword) {
        std::vector<Product> result;
        for (const auto& product : products) {
            if (product.getIsActive() &&
                (product.getName().find(keyword) != std::string::npos ||
                    product.getDescription().find(keyword) != std::string::npos)) {
                result.push_back(product);
            }
        }
        return result;
    }

    bool updateProduct(const Product& product) {
        Product* existingProduct = getProduct(product.getId());
        if (existingProduct) {
            *existingProduct = product;
            return true;
        }
        return false;
    }

    // 上架商品
    bool activateProduct(const std::string& productId) {
        Product* product = getProduct(productId);
        if (product) {
            product->activate();
            return true;
        }
        return false;
    }

    // 下架商品
    bool deactivateProduct(const std::string& productId) {
        Product* product = getProduct(productId);
        if (product) {
            product->deactivate();
            return true;
        }
        return false;
    }

    bool deleteProduct(const std::string& productId) {
        auto it = std::remove_if(products.begin(), products.end(),
            [&](const Product& p) { return p.getId() == productId; });

        if (it != products.end()) {
            products.erase(it, products.end());
            return true;
        }
        return false;
    }

    // 订单管理（原有方法保持不变）
    bool addOrder(const Order& order) {
        orders.push_back(order);
        return true;
    }

    std::vector<Order> getOrdersByUser(const std::string& username) {
        std::vector<Order> result;
        for (const auto& order : orders) {
            if (order.getUsername() == username) {
                result.push_back(order);
            }
        }
        return result;
    }

    std::vector<Order> getAllOrders() {
        return orders;
    }

    Order* getOrder(const std::string& orderId) {
        for (auto& order : orders) {
            if (order.getOrderId() == orderId) {
                return &order;
            }
        }
        return nullptr;
    }

    bool updateOrder(const Order& order) {
        for (auto& ord : orders) {
            if (ord.getOrderId() == order.getOrderId()) {
                ord = order;
                return true;
            }
        }
        return false;
    }

    // 统计信息
    int getTotalUserCount() const { return users.size(); }
    int getTotalProductCount() const { return products.size(); }
    int getActiveProductCount() const {
        int count = 0;
        for (const auto& product : products) {
            if (product.getIsActive()) count++;
        }
        return count;
    }
    int getTotalOrderCount() const { return orders.size(); }

    double getTotalSales() const {
        double total = 0.0;
        for (const auto& order : orders) {
            if (order.getStatus() == "completed" || order.getStatus() == "shipped") {
                total += order.getTotalAmount();
            }
        }
        return total;
    }
};

#endif // DATABASEMANAGER_H