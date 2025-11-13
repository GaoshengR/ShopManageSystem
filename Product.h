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
    bool isActive;
    std::string sellerUsername;  // 新增：卖家用户名
    std::string sellerPhone;     // 新增：卖家手机号

public:
    Product() : price(0.0), stock(0), isActive(true) {}

    Product(const std::string& id, const std::string& name, const std::string& category,
        double price, int stock, const std::string& description = "",
        bool isActive = true, const std::string& sellerUsername = "",
        const std::string& sellerPhone = "")
        : id(id), name(name), category(category), price(price), stock(stock),
        description(description), isActive(isActive), sellerUsername(sellerUsername),
        sellerPhone(sellerPhone) {
    }

    // Getter方法
    std::string getId() const { return id; }
    std::string getName() const { return name; }
    std::string getCategory() const { return category; }
    double getPrice() const { return price; }
    int getStock() const { return stock; }
    std::string getDescription() const { return description; }
    bool getIsActive() const { return isActive; }
    std::string getSellerUsername() const { return sellerUsername; }  // 新增
    std::string getSellerPhone() const { return sellerPhone; }        // 新增

    // Setter方法
    void setName(const std::string& newName) { name = newName; }
    void setCategory(const std::string& newCategory) { category = newCategory; }
    void setPrice(double newPrice) { price = newPrice; }
    void setStock(int newStock) { stock = newStock; }
    void setDescription(const std::string& newDescription) { description = newDescription; }
    void setIsActive(bool active) { isActive = active; }
    void setSellerUsername(const std::string& username) { sellerUsername = username; }  // 新增
    void setSellerPhone(const std::string& phone) { sellerPhone = phone; }              // 新增

    // 业务方法
    void displayInfo() const {
        std::cout << "商品ID: " << id << std::endl;
        std::cout << "商品名称: " << name << std::endl;
        std::cout << "分类: " << category << std::endl;
        std::cout << "价格: Y" << std::fixed << std::setprecision(2) << price << std::endl;
        std::cout << "库存: " << stock << std::endl;
        std::cout << "状态: " << (isActive ? "上架" : "下架") << std::endl;
        std::cout << "卖家: " << sellerUsername << std::endl;        // 新增
        std::cout << "卖家手机: " << sellerPhone << std::endl;       // 新增
        if (!description.empty()) {
            std::cout << "描述: " << description << std::endl;
        }
        std::cout << "------------------------" << std::endl;
    }

    // 简略显示，用于列表
    void displayBriefInfo() const {
        std::cout << id << " | " << name << " | Y" << std::fixed << std::setprecision(2) << price
            << " | 库存:" << stock << " | " << (isActive ? "上架" : "下架")
            << " | 卖家:" << sellerUsername << std::endl;
    }

    // 显示给买家的信息（隐藏卖家联系方式）
    void displayInfoForBuyer() const {
        std::cout << "商品ID: " << id << std::endl;
        std::cout << "商品名称: " << name << std::endl;
        std::cout << "分类: " << category << std::endl;
        std::cout << "价格: Y" << std::fixed << std::setprecision(2) << price << std::endl;
        std::cout << "库存: " << stock << std::endl;
        std::cout << "卖家: " << sellerUsername << std::endl;
        if (!description.empty()) {
            std::cout << "描述: " << description << std::endl;
        }
        std::cout << "------------------------" << std::endl;
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
            << price << "|" << stock << "|" << description << "|"
            << isActive << "|" << sellerUsername << "|" << sellerPhone;
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
            std::string sellerUser = "";
            std::string sellerPhone = "";

            if (tokens.size() > 6) {
                active = (tokens[6] == "1" || tokens[6] == "true");
            }
            if (tokens.size() > 7) {
                sellerUser = tokens[7];
            }
            if (tokens.size() > 8) {
                sellerPhone = tokens[8];
            }

            return Product(tokens[0], tokens[1], tokens[2],
                std::stod(tokens[3]), std::stoi(tokens[4]),
                tokens.size() > 5 ? tokens[5] : "", active, sellerUser, sellerPhone);
        }
        return Product();
    }
};

#endif // PRODUCT_H