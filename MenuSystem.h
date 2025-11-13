#pragma once
#ifndef MENUSYSTEM_H
#define MENUSYSTEM_H

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "ShopSystem.h"

/**
 * @brief 菜单系统类 - 用户界面交互
 */
class MenuSystem {
private:
    ShopSystem shopSystem;

public:
    MenuSystem() {}

    void run() {
        while (true) {
            if (!shopSystem.isUserLoggedIn()) {
                showMainMenu();
            }
            else {
                if (shopSystem.getCurrentUser().isAdmin()) {
                    showAdminMenu();
                }
                else {
                    showCustomerMenu();
                }
            }
        }
    }

private:
    // 主菜单
    void showMainMenu() {
        clearScreen();
        printHeader("商城管理系统");
        std::cout << "1. 用户登录" << std::endl;
        std::cout << "2. 用户注册" << std::endl;
        std::cout << "3. 退出系统" << std::endl;
        std::cout << "请选择操作: ";

        int choice = getIntInput("");
        switch (choice) {
        case 1:
            showLoginMenu();
            break;
        case 2:
            showRegisterMenu();
            break;
        case 3:
            std::cout << "感谢使用商城管理系统，再见！" << std::endl;
            exit(0);
        default:
            std::cout << "无效选择，请重新输入！" << std::endl;
            pause();
        }
    }

    void showLoginMenu() {
        clearScreen();
        printHeader("用户登录");

        int attempts = 0;
        const int maxAttempts = 3;

        while (attempts < maxAttempts) {
            std::string username = getStringInput("用户名: ");
            std::string password = getStringInput("密码: ");

            if (shopSystem.login(username, password)) {
                return;
            }

            attempts++;
            if (attempts < maxAttempts) {
                std::cout << "登录失败，还有 " << (maxAttempts - attempts) << " 次尝试机会" << std::endl;
            }
            else {
                std::cout << "登录失败次数过多，返回主菜单" << std::endl;
                pause();
            }
        }
    }

    void showRegisterMenu() {
        clearScreen();
        printHeader("用户注册");

        std::string username = getStringInput("用户名: ");
        std::string password = getStringInput("密码: ");
        std::string confirmPassword = getStringInput("确认密码: ");

        if (password != confirmPassword) {
            std::cout << "两次输入的密码不一致！" << std::endl;
            pause();
            return;
        }

        std::string email = getStringInput("邮箱(可选): ");
        std::string phone = getStringInput("电话(可选): ");

        if (shopSystem.registerUser(username, password, "customer", email, phone)) {
            std::cout << "注册成功！是否立即登录？(y/n): ";
            std::string choice = getStringInput("");
            if (choice == "y" || choice == "Y") {
                if (shopSystem.login(username, password)) {
                    std::cout << "自动登录成功！" << std::endl;
                }
            }
        }
        pause();
    }

    // 客户菜单
    void showCustomerMenu() {
        clearScreen();
        printHeader("用户菜单 - " + shopSystem.getLoginStatus());
        std::cout << "1. 浏览商品" << std::endl;
        std::cout << "2. 搜索商品" << std::endl;
        std::cout << "3. 查看商品详情" << std::endl;
        std::cout << "4. 购物车管理" << std::endl;
        std::cout << "5. 我的订单" << std::endl;
        std::cout << "6. 退出登录" << std::endl;
        std::cout << "请选择操作: ";

        int choice = getIntInput("");
        switch (choice) {
        case 1:
            browseProducts();
            break;
        case 2:
            searchProducts();
            break;
        case 3:
            viewProductDetails();
            break;
        case 4:
            showCartMenu();
            break;
        case 5:
            showOrderMenu();
            break;
        case 6:
            shopSystem.logout();
            pause();
            break;
        default:
            std::cout << "无效选择！" << std::endl;
            pause();
        }
    }

    // 管理员菜单
    void showAdminMenu() {
        clearScreen();
        printHeader("管理员菜单 - " + shopSystem.getLoginStatus());
        std::cout << "1. 商品管理" << std::endl;
        std::cout << "2. 用户管理" << std::endl;
        std::cout << "3. 订单管理" << std::endl;
        std::cout << "4. 数据统计" << std::endl;
        std::cout << "5. 退出登录" << std::endl;
        std::cout << "请选择操作: ";

        int choice = getIntInput("");
        switch (choice) {
        case 1:
            showProductManagementMenu();
            break;
        case 2:
            showUserManagementMenu();
            break;
        case 3:
            showOrderManagementMenu();
            break;
        case 4:
            showStatisticsMenu();
            break;
        case 5:
            shopSystem.logout();
            pause();
            break;
        default:
            std::cout << "无效选择！" << std::endl;
            pause();
        }
    }

    // 商品浏览功能
    void browseProducts() {
        clearScreen();
        printHeader("浏览商品");

        auto products = shopSystem.browseProducts();
        if (products.empty()) {
            std::cout << "暂无商品！" << std::endl;
        }
        else {
            for (const auto& product : products) {
                product.displayInfo();
            }
        }
        pause();
    }

    void searchProducts() {
        clearScreen();
        printHeader("搜索商品");

        std::string keyword = getStringInput("请输入搜索关键词: ");
        auto products = shopSystem.searchProducts(keyword);

        if (products.empty()) {
            std::cout << "未找到相关商品！" << std::endl;
        }
        else {
            std::cout << "找到 " << products.size() << " 个相关商品:" << std::endl;
            for (const auto& product : products) {
                product.displayInfo();
            }
        }
        pause();
    }

    void viewProductDetails() {
        clearScreen();
        printHeader("商品详情");

        std::string productId = getStringInput("请输入商品ID: ");
        auto product = shopSystem.getProduct(productId);

        if (product) {
            product->displayInfo();

            std::cout << "\n1. 加入购物车" << std::endl;
            std::cout << "2. 返回" << std::endl;
            std::cout << "请选择操作: ";

            int choice = getIntInput("");
            if (choice == 1) {
                int quantity = getIntInput("请输入数量: ");
                shopSystem.addToCart(productId, quantity);
            }
        }
        else {
            std::cout << "商品不存在！" << std::endl;
        }
        pause();
    }

    // 购物车菜单
    void showCartMenu() {
        clearScreen();
        printHeader("购物车管理");

        shopSystem.displayCart();

        if (!shopSystem.getCartItems().empty()) {
            std::cout << "\n1. 继续购物" << std::endl;
            std::cout << "2. 生成订单" << std::endl;
            std::cout << "3. 清空购物车" << std::endl;
            std::cout << "4. 返回" << std::endl;
            std::cout << "请选择操作: ";

            int choice = getIntInput("");
            switch (choice) {
            case 1:
                // 继续购物，直接返回
                break;
            case 2:
                createOrder();
                break;
            case 3:
                // 清空购物车功能需要实现
                std::cout << "清空购物车功能待实现" << std::endl;
                pause();
                break;
            case 4:
                return;
            default:
                std::cout << "无效选择！" << std::endl;
                pause();
            }
        }
        else {
            pause();
        }
    }

    void createOrder() {
        clearScreen();
        printHeader("生成订单");

        std::string address = getStringInput("请输入配送地址: ");
        std::string payment = getStringInput("请输入支付方式: ");

        Order order = shopSystem.createOrder(address, payment);
        if (order.getOrderId() != "") {
            std::cout << "\n订单详情:" << std::endl;
            order.displayOrderDetails();
        }
        pause();
    }

    // 订单菜单
    void showOrderMenu() {
        clearScreen();
        printHeader("我的订单");

        auto orders = shopSystem.getUserOrders();
        if (orders.empty()) {
            std::cout << "您还没有订单！" << std::endl;
        }
        else {
            for (const auto& order : orders) {
                order.displayBriefInfo();
            }

            std::cout << "\n1. 查看订单详情" << std::endl;
            std::cout << "2. 取消订单" << std::endl;
            std::cout << "3. 返回" << std::endl;
            std::cout << "请选择操作: ";

            int choice = getIntInput("");
            switch (choice) {
            case 1: {
                std::string orderId = getStringInput("请输入订单ID: ");
                // 查看订单详情功能需要实现
                std::cout << "查看订单详情功能待实现" << std::endl;
                break;
            }
            case 2: {
                std::string orderId = getStringInput("请输入要取消的订单ID: ");
                shopSystem.cancelOrder(orderId);
                break;
            }
            case 3:
                return;
            default:
                std::cout << "无效选择！" << std::endl;
            }
        }
        pause();
    }

    // 管理员功能菜单
    void showProductManagementMenu() {
        clearScreen();
        printHeader("商品管理");

        std::cout << "1. 添加商品" << std::endl;
        std::cout << "2. 查看所有商品" << std::endl;
        std::cout << "3. 上架商品" << std::endl;
        std::cout << "4. 下架商品" << std::endl;
        std::cout << "5. 查看下架商品" << std::endl;
        std::cout << "6. 返回" << std::endl;
        std::cout << "请选择操作: ";

        int choice = getIntInput("");

        switch (choice) {
        case 1:
            addProduct();
            break;
        case 2:
            showAllProductsForAdmin();
            break;
        case 3:
            activateProduct();
            break;
        case 4:
            deactivateProduct();
            break;
        case 5:
            showInactiveProducts();
            break;
        case 6:
            return;
        default:
            std::cout << "无效选择！" << std::endl;
            pause();
        }
    }
    void showAllProductsForAdmin() {
        clearScreen();
        printHeader("所有商品（管理员视图）");

        auto products = shopSystem.getAllProductsForAdmin();
        if (products.empty()) {
            std::cout << "暂无商品！" << std::endl;
        }
        else {
            for (const auto& product : products) {
                product.displayInfo();
            }
        }
        pause();
    }

    void activateProduct() {
        clearScreen();
        printHeader("上架商品");

        std::string productId = getStringInput("请输入要上架的商品ID: ");
        shopSystem.activateProduct(productId);
        pause();
    }

    void deactivateProduct() {
        clearScreen();
        printHeader("下架商品");

        std::string productId = getStringInput("请输入要下架的商品ID: ");
        shopSystem.deactivateProduct(productId);
        pause();
    }

    void showInactiveProducts() {
        clearScreen();
        printHeader("下架商品列表");

        auto products = shopSystem.getInactiveProducts();
        if (products.empty()) {
            std::cout << "暂无下架商品！" << std::endl;
        }
        else {
            for (const auto& product : products) {
                product.displayBriefInfo();
            }
        }
        pause();
    }
    void addProduct() {
        clearScreen();
        printHeader("添加商品");

        std::string id = getStringInput("商品ID: ");
        std::string name = getStringInput("商品名称: ");
        std::string category = getStringInput("商品分类: ");
        double price = getDoubleInput("价格: ");
        int stock = getIntInput("库存数量: ");
        std::string description = getStringInput("商品描述: ");

        // 这里需要调用ShopSystem的添加商品方法
        std::cout << "添加商品功能需要在ShopSystem中实现" << std::endl;
        pause();
    }

    void showUserManagementMenu() {
        clearScreen();
        printHeader("用户管理");

        // 这里需要显示所有用户
        std::cout << "用户管理功能待实现" << std::endl;
        pause();
    }

    void showOrderManagementMenu() {
        clearScreen();
        printHeader("订单管理");

        // 这里需要显示所有订单
        std::cout << "订单管理功能待实现" << std::endl;
        pause();
    }

    void showStatisticsMenu() {
        clearScreen();
        printHeader("数据统计");

        // 这里需要显示统计信息
        std::cout << "数据统计功能待实现" << std::endl;
        pause();
    }

    // 辅助方法
    void pause() {
        std::cout << "按回车键继续...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }

    void clearScreen() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    int getIntInput(const std::string& prompt) {
        int value;
        std::cout << prompt;
        while (!(std::cin >> value)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "输入无效，请输入数字: ";
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return value;
    }

    double getDoubleInput(const std::string& prompt) {
        double value;
        std::cout << prompt;
        while (!(std::cin >> value)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "输入无效，请输入数字: ";
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return value;
    }

    std::string getStringInput(const std::string& prompt) {
        std::string value;
        std::cout << prompt;
        std::getline(std::cin, value);
        return value;
    }

    void printHeader(const std::string& title) {
        std::cout << "==========================================" << std::endl;
        std::cout << "          " << title << std::endl;
        std::cout << "==========================================" << std::endl;
    }
};

#endif // MENUSYSTEM_H