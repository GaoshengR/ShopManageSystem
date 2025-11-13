#ifndef SHOPSYSTEM_H
#define SHOPSYSTEM_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include "DatabaseManager.h"
#include "User.h"
#include "Product.h"
#include "Order.h"

/**
 * @brief 商城系统核心类
 */
class ShopSystem {
private:
    DatabaseManager db;
    User currentUser;
    bool isLoggedIn;
    std::vector<OrderItem> tempCart;

public:
    ShopSystem() : isLoggedIn(false) {}

    // ==================== 用户认证 ====================
    bool registerUser(const std::string& username, const std::string& password,
        const std::string& userType = "customer",
        const std::string& email = "", const std::string& phone = "") {
        if (username.empty() || password.empty()) {
            std::cout << "用户名和密码不能为空！" << std::endl;
            return false;
        }

        if (username.length() < 3) {
            std::cout << "用户名长度不能少于3个字符！" << std::endl;
            return false;
        }

        if (password.length() < 6) {
            std::cout << "密码长度不能少于6个字符！" << std::endl;
            return false;
        }

        // 新增：手机号验证
        if (phone.empty()) {
            std::cout << "手机号不能为空！" << std::endl;
            return false;
        }

        User tempUser("", "", "", "", phone);
        if (!tempUser.isValidPhone()) {
            std::cout << "手机号格式不正确！请输入11位有效手机号" << std::endl;
            return false;
        }

        if (db.userExists(username)) {
            std::cout << "用户名已存在！" << std::endl;
            return false;
        }

        User newUser(username, password, userType, email, phone);
        bool success = db.addUser(newUser);
        if (success) {
            std::cout << "注册成功！" << std::endl;
        }
        return success;
    }

    bool login(const std::string& username, const std::string& password) {
        if (username.empty() || password.empty()) {
            std::cout << "用户名和密码不能为空！" << std::endl;
            return false;
        }

        User* user = db.getUser(username);
        if (user && user->getPassword() == password) {
            currentUser = *user;
            isLoggedIn = true;
            tempCart.clear();
            std::cout << "登录成功！欢迎 " << username << std::endl;
            return true;
        }
        else {
            std::cout << "用户名或密码错误！" << std::endl;
            return false;
        }
    }

    void logout() {
        currentUser = User();
        isLoggedIn = false;
        tempCart.clear();
        std::cout << "已退出登录！" << std::endl;
    }

    bool isUserLoggedIn() const { return isLoggedIn; }
    User getCurrentUser() const { return currentUser; }

    std::string getLoginStatus() const {
        if (!isLoggedIn) return "未登录";
        return "已登录: " + currentUser.getUsername() +
            " (" + (currentUser.isAdmin() ? "管理员" : "普通用户") + ")";
    }

    // ==================== 商品管理 ====================
    // 用户上架商品
    bool addProduct(const std::string& id, const std::string& name,
        const std::string& category, double price, int stock,
        const std::string& description = "") {
        if (!isLoggedIn) {
            std::cout << "请先登录！" << std::endl;
            return false;
        }

        if (id.empty() || name.empty() || category.empty()) {
            std::cout << "商品ID、名称和分类不能为空！" << std::endl;
            return false;
        }

        if (price <= 0) {
            std::cout << "价格必须大于0！" << std::endl;
            return false;
        }

        if (stock < 0) {
            std::cout << "库存不能为负数！" << std::endl;
            return false;
        }

        // 创建商品，包含卖家信息
        Product product(id, name, category, price, stock, description,
            true, currentUser.getUsername(), currentUser.getPhone());

        bool success = db.addProduct(product);
        if (success) {
            std::cout << "商品上架成功！" << std::endl;
        }
        else {
            std::cout << "商品上架失败，商品ID已存在！" << std::endl;
        }
        return success;
    }

    // 用户下架自己的商品
    bool deactivateMyProduct(const std::string& productId) {
        if (!isLoggedIn) {
            std::cout << "请先登录！" << std::endl;
            return false;
        }

        Product* product = db.getProduct(productId);
        if (!product) {
            std::cout << "商品不存在！" << std::endl;
            return false;
        }

        // 检查是否是商品所有者
        if (product->getSellerUsername() != currentUser.getUsername() && !currentUser.isAdmin()) {
            std::cout << "无权操作此商品！" << std::endl;
            return false;
        }

        product->deactivate();
        bool success = db.updateProduct(*product);
        if (success) {
            std::cout << "商品下架成功！" << std::endl;
        }
        return success;
    }

    // 用户重新上架自己的商品
    bool activateMyProduct(const std::string& productId) {
        if (!isLoggedIn) {
            std::cout << "请先登录！" << std::endl;
            return false;
        }

        Product* product = db.getProduct(productId);
        if (!product) {
            std::cout << "商品不存在！" << std::endl;
            return false;
        }

        // 检查是否是商品所有者
        if (product->getSellerUsername() != currentUser.getUsername() && !currentUser.isAdmin()) {
            std::cout << "无权操作此商品！" << std::endl;
            return false;
        }

        product->activate();
        bool success = db.updateProduct(*product);
        if (success) {
            std::cout << "商品上架成功！" << std::endl;
        }
        return success;
    }

    // 获取用户自己的商品
    std::vector<Product> getMyProducts() {
        if (!isLoggedIn) return std::vector<Product>();

        std::vector<Product> allProducts = db.getAllProducts();
        std::vector<Product> myProducts;

        for (const auto& product : allProducts) {
            if (product.getSellerUsername() == currentUser.getUsername()) {
                myProducts.push_back(product);
            }
        }
        return myProducts;
    }

    // 管理员商品状态管理
    bool activateProduct(const std::string& productId) {
        if (!checkAdminPermission()) return false;

        bool success = db.activateProduct(productId);
        if (success) {
            std::cout << "商品上架成功！" << std::endl;
        }
        else {
            std::cout << "商品上架失败，商品不存在！" << std::endl;
        }
        return success;
    }

    bool deactivateProduct(const std::string& productId) {
        if (!checkAdminPermission()) return false;

        bool success = db.deactivateProduct(productId);
        if (success) {
            std::cout << "商品下架成功！" << std::endl;
        }
        else {
            std::cout << "商品下架失败，商品不存在！" << std::endl;
        }
        return success;
    }

    std::vector<Product> browseProducts() {
        return db.getActiveProducts();
    }

    std::vector<Product> searchProducts(const std::string& keyword) {
        return db.searchProducts(keyword);
    }

    Product* getProduct(const std::string& productId) {
        return db.getProduct(productId);
    }

    // 获取所有商品（管理员用，包括下架的）
    std::vector<Product> getAllProductsForAdmin() {
        if (!checkAdminPermission()) return std::vector<Product>();
        return db.getAllProducts();
    }

    // 获取上架商品（客户用）
    std::vector<Product> getActiveProducts() {
        return db.getActiveProducts();
    }

    // 获取下架商品
    std::vector<Product> getInactiveProducts() {
        if (!checkAdminPermission()) return std::vector<Product>();
        return db.getInactiveProducts();
    }

    // ==================== 购物车操作 ====================
    bool addToCart(const std::string& productId, int quantity) {
        if (!isLoggedIn) {
            std::cout << "请先登录！" << std::endl;
            return false;
        }

        Product* product = db.getProduct(productId);
        if (!product) {
            std::cout << "商品不存在！" << std::endl;
            return false;
        }

        // 检查商品是否上架
        if (!product->getIsActive()) {
            std::cout << "该商品已下架，无法购买！" << std::endl;
            return false;
        }

        // 不能购买自己的商品
        if (product->getSellerUsername() == currentUser.getUsername()) {
            std::cout << "不能购买自己上架的商品！" << std::endl;
            return false;
        }

        if (quantity <= 0) {
            std::cout << "数量必须大于0！" << std::endl;
            return false;
        }

        if (!product->hasEnoughStock(quantity)) {
            std::cout << "库存不足！当前库存: " << product->getStock() << std::endl;
            return false;
        }

        // 检查是否已在购物车中
        for (auto& item : tempCart) {
            if (item.getProductId() == productId) {
                item.setQuantity(item.getQuantity() + quantity);
                std::cout << "已更新购物车中的商品数量" << std::endl;
                return true;
            }
        }

        // 添加到购物车，包含卖家信息
        tempCart.push_back(OrderItem(productId, product->getName(), quantity,
            product->getPrice(), product->getSellerUsername(),
            product->getSellerPhone()));
        std::cout << "商品已添加到购物车" << std::endl;
        return true;
    }

    std::vector<OrderItem> getCartItems() const {
        return tempCart;
    }

    double getCartTotal() const {
        double total = 0.0;
        for (const auto& item : tempCart) {
            total += item.getTotalPrice();
        }
        return total;
    }

    void displayCart() const {
        if (tempCart.empty()) {
            std::cout << "购物车为空" << std::endl;
            return;
        }

        std::cout << "=== 购物车 ===" << std::endl;
        for (const auto& item : tempCart) {
            item.displayInfo();
        }
        std::cout << "总计: Y" << std::fixed << std::setprecision(2) << getCartTotal() << std::endl;
    }

    // ==================== 订单管理 ====================
    Order createOrder(const std::string& address, const std::string& payment) {
        if (!isLoggedIn) {
            std::cout << "请先登录！" << std::endl;
            return Order();
        }

        if (tempCart.empty()) {
            std::cout << "购物车为空！" << std::endl;
            return Order();
        }

        // 检查库存
        for (const auto& item : tempCart) {
            Product* product = db.getProduct(item.getProductId());
            if (!product || !product->hasEnoughStock(item.getQuantity())) {
                std::cout << "商品 " << item.getProductName() << " 库存不足！" << std::endl;
                return Order();
            }
        }

        // 创建订单，包含买家手机号
        Order order(currentUser.getUsername(), tempCart, address, payment, currentUser.getPhone());

        // 减少库存
        for (const auto& item : tempCart) {
            Product* product = db.getProduct(item.getProductId());
            if (product) {
                product->reduceStock(item.getQuantity());
                db.updateProduct(*product);
            }
        }

        // 保存订单
        if (db.addOrder(order)) {
            tempCart.clear();
            std::cout << "订单创建成功！订单ID: " << order.getOrderId() << std::endl;
            return order;
        }

        return Order();
    }

    std::vector<Order> getUserOrders() {
        if (!isLoggedIn) {
            std::cout << "请先登录！" << std::endl;
            return std::vector<Order>();
        }
        return db.getOrdersByUser(currentUser.getUsername());
    }

    bool cancelOrder(const std::string& orderId) {
        if (!isLoggedIn) {
            std::cout << "请先登录！" << std::endl;
            return false;
        }

        Order* order = db.getOrder(orderId);
        if (!order) {
            std::cout << "订单不存在！" << std::endl;
            return false;
        }

        if (order->getUsername() != currentUser.getUsername()) {
            std::cout << "无权操作此订单！" << std::endl;
            return false;
        }

        if (order->canCancel()) {
            order->cancel();

            // 恢复库存
            for (const auto& item : order->getItems()) {
                Product* product = db.getProduct(item.getProductId());
                if (product) {
                    product->increaseStock(item.getQuantity());
                    db.updateProduct(*product);
                }
            }

            bool success = db.updateOrder(*order);
            if (success) {
                std::cout << "订单取消成功！" << std::endl;
            }
            return success;
        }
        else {
            std::cout << "订单无法取消！" << std::endl;
            return false;
        }
    }

    // ==================== 管理员统计功能 ====================
    void displayStatistics() const {
        if (!checkAdminPermission()) return;

        std::cout << "=== 系统统计 ===" << std::endl;
        std::cout << "用户总数: " << db.getTotalUserCount() << std::endl;
        std::cout << "商品总数: " << db.getTotalProductCount() << std::endl;
        std::cout << "上架商品: " << db.getActiveProductCount() << std::endl;
        std::cout << "订单总数: " << db.getTotalOrderCount() << std::endl;
        std::cout << "总销售额: Y" << std::fixed << std::setprecision(2) << db.getTotalSales() << std::endl;
    }

private:
    bool checkAdminPermission() const {
        if (!isLoggedIn) {
            std::cout << "请先登录！" << std::endl;
            return false;
        }
        if (!currentUser.isAdmin()) {
            std::cout << "权限不足！" << std::endl;
            return false;
        }
        return true;
    }
};

#endif // SHOPSYSTEM_H