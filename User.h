#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

/**
 * @brief 用户类 - 管理商城系统的用户信息
 */
class User {
private:
    std::string username;
    std::string password;
    std::string userType;
    std::string email;
    std::string phone;

public:
    User() : userType("customer") {}

    User(const std::string& username, const std::string& password,
        const std::string& userType = "customer", const std::string& email = "",
        const std::string& phone = "")
        : username(username), password(password), userType(userType), email(email), phone(phone) {
    }

    // Getter方法
    std::string getUsername() const { return username; }
    std::string getPassword() const { return password; }
    std::string getUserType() const { return userType; }
    std::string getEmail() const { return email; }
    std::string getPhone() const { return phone; }

    // Setter方法
    void setPassword(const std::string& newPassword) { password = newPassword; }
    void setEmail(const std::string& newEmail) { email = newEmail; }
    void setPhone(const std::string& newPhone) { phone = newPhone; }

    // 业务方法
    bool isAdmin() const {
        return userType == "admin";
    }

    void displayInfo() const {
        std::cout << "用户名: " << username << std::endl;
        std::cout << "用户类型: " << (isAdmin() ? "管理员" : "普通用户") << std::endl;
        std::cout << "手机号: " << phone << std::endl;  // 手机号现在必填
        if (!email.empty()) {
            std::cout << "邮箱: " << email << std::endl;
        }
        std::cout << "------------------------" << std::endl;
    }

    // 验证手机号格式
    bool isValidPhone() const {
        // 简单的手机号验证：11位数字，以1开头
        if (phone.length() != 11) return false;
        if (phone[0] != '1') return false;
        for (char c : phone) {
            if (!isdigit(c)) return false;
        }
        return true;
    }

    // 序列化方法
    std::string toString() const {
        std::ostringstream oss;
        oss << username << "|" << password << "|" << userType << "|" << email << "|" << phone;
        return oss.str();
    }

    static User fromString(const std::string& data) {
        std::istringstream iss(data);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(iss, token, '|')) {
            tokens.push_back(token);
        }

        if (tokens.size() >= 3) {
            return User(tokens[0], tokens[1], tokens[2],
                tokens.size() > 3 ? tokens[3] : "",
                tokens.size() > 4 ? tokens[4] : "");
        }
        return User();
    }
};

#endif // USER_H