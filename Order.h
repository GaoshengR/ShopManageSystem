#pragma once
#ifndef ORDER_H
#define ORDER_H

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include "Product.h"

/**
 * @brief 订单项类
 */
class OrderItem {
private:
    std::string productId;
    std::string productName;
    int quantity;
    double price;

public:
    OrderItem() : quantity(0), price(0.0) {}

    OrderItem(const std::string& productId, const std::string& productName,
        int quantity, double price)
        : productId(productId), productName(productName), quantity(quantity), price(price) {
    }

    // Getter方法
    std::string getProductId() const { return productId; }
    std::string getProductName() const { return productName; }
    int getQuantity() const { return quantity; }
    double getPrice() const { return price; }
    double getTotalPrice() const { return price * quantity; }

    void setQuantity(int newQuantity) { quantity = newQuantity; }

    void displayInfo() const {
        std::cout << productName << " x " << quantity
            << " @ ￥" << std::fixed << std::setprecision(2) << price
            << " = ￥" << getTotalPrice() << std::endl;
    }

    std::string toString() const {
        std::ostringstream oss;
        oss << productId << "|" << productName << "|" << quantity << "|" << price;
        return oss.str();
    }

    static OrderItem fromString(const std::string& data) {
        std::istringstream iss(data);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(iss, token, '|')) {
            tokens.push_back(token);
        }

        if (tokens.size() >= 4) {
            return OrderItem(tokens[0], tokens[1], std::stoi(tokens[2]), std::stod(tokens[3]));
        }
        return OrderItem();
    }
};

/**
 * @brief 订单类
 */
class Order {
private:
    std::string orderId;
    std::string username;
    std::vector<OrderItem> items;
    double totalAmount;
    std::string orderTime;
    std::string status;
    std::string shippingAddress;
    std::string paymentMethod;

public:
    Order() : totalAmount(0.0), status("pending") {}

    Order(const std::string& username, const std::vector<OrderItem>& items,
        const std::string& address, const std::string& payment)
        : username(username), items(items), shippingAddress(address),
        paymentMethod(payment), status("pending") {
        generateOrderId();
        setOrderTime();
        calculateTotalAmount();
    }

    // Getter方法
    std::string getOrderId() const { return orderId; }
    std::string getUsername() const { return username; }
    std::vector<OrderItem> getItems() const { return items; }
    double getTotalAmount() const { return totalAmount; }
    std::string getOrderTime() const { return orderTime; }
    std::string getStatus() const { return status; }
    std::string getShippingAddress() const { return shippingAddress; }
    std::string getPaymentMethod() const { return paymentMethod; }

    // 状态管理
    void setStatus(const std::string& newStatus) { status = newStatus; }

    void pay() {
        if (status == "pending") {
            status = "paid";
        }
    }

    void ship() {
        if (status == "paid") {
            status = "shipped";
        }
    }

    void complete() {
        if (status == "shipped") {
            status = "completed";
        }
    }

    void cancel() {
        if (status == "pending" || status == "paid") {
            status = "cancelled";
        }
    }

    bool canCancel() const {
        return status == "pending" || status == "paid";
    }

    void displayOrderDetails() const {
        std::cout << "订单ID: " << orderId << std::endl;
        std::cout << "用户名: " << username << std::endl;
        std::cout << "下单时间: " << orderTime << std::endl;
        std::cout << "状态: " << getStatusText() << std::endl;
        std::cout << "配送地址: " << shippingAddress << std::endl;
        std::cout << "支付方式: " << paymentMethod << std::endl;
        std::cout << "商品列表:" << std::endl;

        for (const auto& item : items) {
            std::cout << "  ";
            item.displayInfo();
        }

        std::cout << "总金额: ￥" << std::fixed << std::setprecision(2) << totalAmount << std::endl;
        std::cout << "------------------------" << std::endl;
    }

    void displayBriefInfo() const {
        std::cout << orderId << " | " << getStatusText() << " | ￥"
            << std::fixed << std::setprecision(2) << totalAmount
            << " | " << orderTime << std::endl;
    }

    std::string getStatusText() const {
        if (status == "pending") return "待支付";
        if (status == "paid") return "已支付";
        if (status == "shipped") return "已发货";
        if (status == "completed") return "已完成";
        if (status == "cancelled") return "已取消";
        return "未知状态";
    }

    std::string toString() const {
        std::ostringstream oss;
        oss << orderId << "|" << username << "|" << totalAmount << "|"
            << orderTime << "|" << status << "|" << shippingAddress << "|"
            << paymentMethod << "|";

        for (size_t i = 0; i < items.size(); ++i) {
            if (i > 0) oss << ";";
            oss << items[i].toString();
        }

        return oss.str();
    }

    static Order fromString(const std::string& data) {
        std::istringstream iss(data);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(iss, token, '|')) {
            tokens.push_back(token);
        }

        if (tokens.size() >= 7) {
            Order order;
            order.orderId = tokens[0];
            order.username = tokens[1];
            order.totalAmount = std::stod(tokens[2]);
            order.orderTime = tokens[3];
            order.status = tokens[4];
            order.shippingAddress = tokens[5];
            order.paymentMethod = tokens[6];

            if (tokens.size() > 7 && !tokens[7].empty()) {
                std::istringstream itemsStream(tokens[7]);
                std::string itemToken;
                while (std::getline(itemsStream, itemToken, ';')) {
                    order.items.push_back(OrderItem::fromString(itemToken));
                }
            }
            return order;
        }
        return Order();
    }

private:
    void generateOrderId() {
        std::ostringstream oss;
        oss << "ORD" << std::time(nullptr) << std::rand() % 1000;
        orderId = oss.str();
    }

    void setOrderTime() {
        std::time_t now = std::time(nullptr);
        std::tm localTime;
        localtime_s(&localTime, &now); // 使用安全版本 localtime_s
        std::ostringstream oss;
        oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
        orderTime = oss.str();
    }

    void calculateTotalAmount() {
        totalAmount = 0.0;
        for (const auto& item : items) {
            totalAmount += item.getTotalPrice();
        }
    }
};

#endif // ORDER_H