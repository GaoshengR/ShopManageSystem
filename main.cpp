#include <iostream>
#include <cstdlib>
#include <ctime>

// 按依赖顺序包含头文件
#include "User.h"
#include "Product.h" 
#include "Order.h"
#include "DatabaseManager.h"
#include "ShopSystem.h"
#include "MenuSystem.h"

int main() {
   
    std::srand(std::time(nullptr));

    std::cout << "=== 商城管理系统启动 ===" << std::endl;
    std::cout << "系统初始化中..." << std::endl;
    try {
        MenuSystem menuSystem;
        std::cout << "系统初始化完成！" << std::endl;
        std::cout << "按回车键进入系统..." << std::endl;
        std::cin.get();

        menuSystem.run();
    }
    catch (const std::exception& e) {
        std::cerr << "系统错误: " << e.what() << std::endl;
        std::cout << "按回车键退出..." << std::endl;
        std::cin.get();
        return 1;
    }
    catch (...) {
        std::cerr << "未知错误发生！" << std::endl;
        std::cout << "按回车键退出..." << std::endl;
        std::cin.get();
        return 1;
    }

    return 0;
}