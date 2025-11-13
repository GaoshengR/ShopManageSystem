#ifndef PRODUCT_H
#define PRODUCT_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>

/**
 * @brief 商品类 - 管理商品信息
 */
class Product {
private:
    std::string id;
    std::string name;
    std::string category;
    double price;
    int stock;
    std::string description;
    bool isActive;  // 新增：商品状态，true表示上架，false表示下架

public:
    Product() : price(0.0), stock(0), isActive(true) {}  // 默认上架

    Product(const std::string& id, const std::string& name, const std::string& category,
        double price, int stock, const std::string& description = "", bool isActive = true)
        : id(id), name(name), category(category), price(price), stock(stock),
        description(description), isActive(isActive) {
    }

    // Getter方法
    std::string getId() const { return id; }
    std::string getName() const { return name; }
    std::string getCategory() const { return category; }
    double getPrice() const { return price; }
    int getStock() const { return stock; }
    std::string getDescription() const { return description; }
    bool getIsActive() const { return isActive; }  // 新增：获取商品状态

    // Setter方法
    void setName(const std::string& newName) { name = newName; }
    void setCategory(const std::string& newCategory) { category = newCategory; }
    void setPrice(double newPrice) { price = newPrice; }
    void setStock(int newStock) { stock = newStock; }
    void setDescription(const std::string& newDescription) { description = newDescription; }
    void setIsActive(bool active) { isActive = active; }  // 新增：设置商品状态

    // 业务方法
    void displayInfo() const {
        std::cout << "商品ID: " << id << std::endl;
        std::cout << "商品名称: " << name << std::endl;
        std::cout << "分类: " << category << std::endl;
        std::cout << "价格: Y" << std::fixed << std::setprecision(2) << price << std::endl;
        std::cout << "库存: " << stock << std::endl;
        std::cout << "状态: " << (isActive ? "上架" : "下架") << std::endl;  // 新增状态显示
        if (!description.empty()) {
            std::cout << "描述: " << description << std::endl;
        }
        std::cout << "------------------------" << std::endl;
    }

    // 简略显示，用于列表
    void displayBriefInfo() const {
        std::cout << id << " | " << name << " | Y" << std::fixed << std::setprecision(2) << price
            << " | 库存:" << stock << " | " << (isActive ? "上架" : "下架") << std::endl;
    }

    // 上架商品
    void activate() {
        isActive = true;
    }

    // 下架商品
    void deactivate() {
        isActive = false;
    }

    // 检查商品是否可用（上架且有库存）
    bool isAvailable() const {
        return isActive && stock > 0;
    }

    bool reduceStock(int quantity) {
        if (stock >= quantity) {
            stock -= quantity;
            return true;
        }
        return false;
    }

    void increaseStock(int quantity) {
        stock += quantity;
    }

    bool isInStock() const {
        return stock > 0;
    }

    bool hasEnoughStock(int quantity) const {
        return stock >= quantity;
    }

    // 序列化方法
    std::string toString() const {
        std::ostringstream oss;
        oss << id << "|" << name << "|" << category << "|"
            << price << "|" << stock << "|" << description << "|" << isActive;
        return oss.str();
    }

    static Product fromString(const std::string& data) {
        std::istringstream iss(data);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(iss, token, '|')) {
            tokens.push_back(token);
        }

        if (tokens.size() >= 6) {
            bool active = true;
            if (tokens.size() > 6) {
                active = (tokens[6] == "1" || tokens[6] == "true");
            }
            return Product(tokens[0], tokens[1], tokens[2],
                std::stod(tokens[3]), std::stoi(tokens[4]),
                tokens.size() > 5 ? tokens[5] : "", active);
        }
        return Product();
    }
};

#endif // PRODUCT_H