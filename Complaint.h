#ifndef COMPLAINT_H
#define COMPLAINT_H

#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
#include <vector>
#include <iomanip>

/**
 * @brief 商品投诉类 - 管理用户对商品的投诉信息
 */
class Complaint {
private:
    std::string complaintId;     ///< 投诉ID
    std::string productId;       ///< 被投诉的商品ID
    std::string productName;     ///< 被投诉的商品名称
    std::string complainant;     ///< 投诉人用户名
    std::string complaintType;   ///< 投诉类型
    std::string title;           ///< 投诉标题
    std::string content;         ///< 投诉内容
    std::string complaintTime;   ///< 投诉时间
    std::string status;          ///< 投诉状态
    std::string response;        ///< 管理员回复
    std::string responseTime;    ///< 回复时间
    std::string adminUser;       ///< 处理投诉的管理员

public:
    /**
     * @brief 默认构造函数
     */
    Complaint() : status("pending") {}

    /**
     * @brief 参数化构造函数
     *
     * @param productId 商品ID
     * @param productName 商品名称
     * @param complainant 投诉人用户名
     * @param complaintType 投诉类型
     * @param title 投诉标题
     * @param content 投诉内容
     */
    Complaint(const std::string& productId, const std::string& productName,
        const std::string& complainant, const std::string& complaintType,
        const std::string& title, const std::string& content);

    // ==================== Getter 方法 ====================

    std::string getComplaintId() const { return complaintId; }
    std::string getProductId() const { return productId; }
    std::string getProductName() const { return productName; }
    std::string getComplainant() const { return complainant; }
    std::string getComplaintType() const { return complaintType; }
    std::string getTitle() const { return title; }
    std::string getContent() const { return content; }
    std::string getComplaintTime() const { return complaintTime; }
    std::string getStatus() const { return status; }
    std::string getResponse() const { return response; }
    std::string getResponseTime() const { return responseTime; }
    std::string getAdminUser() const { return adminUser; }

    // ==================== Setter 方法 ====================

    void setComplaintType(const std::string& newType) { complaintType = newType; }
    void setTitle(const std::string& newTitle) { title = newTitle; }
    void setContent(const std::string& newContent) { content = newContent; }
    void setStatus(const std::string& newStatus) { status = newStatus; }
    void setResponse(const std::string& newResponse) { response = newResponse; }
    void setAdminUser(const std::string& admin) { adminUser = admin; }

    // ==================== 业务逻辑方法 ====================

    /**
     * @brief 生成投诉ID
     */
    void generateComplaintId();

    /**
     * @brief 设置投诉时间为当前时间
     */
    void setCurrentTime();

    /**
     * @brief 设置回复时间为当前时间
     */
    void setResponseTime();

    /**
     * @brief 显示投诉详细信息
     */
    void displayInfo() const;

    /**
     * @brief 显示简要信息（用于列表显示）
     */
    void displayBriefInfo() const;

    /**
     * @brief 处理投诉（添加回复）
     * @param responseContent 回复内容
     * @param adminUsername 处理投诉的管理员用户名
     */
    void processComplaint(const std::string& responseContent, const std::string& adminUsername);

    /**
     * @brief 获取状态文本
     * @return 状态描述
     */
    std::string getStatusText() const;

    /**
     * @brief 检查投诉是否已处理
     * @return true-已处理，false-未处理
     */
    bool isProcessed() const;

    // ==================== 数据持久化方法 ====================

    std::string toString() const;
    static Complaint fromString(const std::string& data);
};

// ==================== 成员函数实现 ====================

inline Complaint::Complaint(const std::string& productId, const std::string& productName,
    const std::string& complainant, const std::string& complaintType,
    const std::string& title, const std::string& content)
    : productId(productId), productName(productName), complainant(complainant),
    complaintType(complaintType), title(title), content(content), status("pending") {
    generateComplaintId();
    setCurrentTime();
}

inline void Complaint::generateComplaintId() {
    std::ostringstream oss;
    oss << "CMP" << std::time(nullptr) << std::rand() % 1000;
    complaintId = oss.str();
}

inline void Complaint::setCurrentTime() {
    std::time_t now = std::time(nullptr);
    std::tm localTime;
    localtime_s(&localTime, &now);
    std::ostringstream oss;
    oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    complaintTime = oss.str();
}

inline void Complaint::setResponseTime() {
    std::time_t now = std::time(nullptr);
    std::tm localTime;
    localtime_s(&localTime, &now);
    std::ostringstream oss;
    oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    responseTime = oss.str();
}

inline void Complaint::displayInfo() const {
    std::cout << "投诉ID: " << complaintId << std::endl;
    std::cout << "商品ID: " << productId << std::endl;
    std::cout << "商品名称: " << productName << std::endl;
    std::cout << "投诉人: " << complainant << std::endl;
    std::cout << "投诉类型: " << complaintType << std::endl;
    std::cout << "标题: " << title << std::endl;
    std::cout << "内容: " << content << std::endl;
    std::cout << "投诉时间: " << complaintTime << std::endl;
    std::cout << "状态: " << getStatusText() << std::endl;

    if (!response.empty()) {
        std::cout << "管理员回复: " << response << std::endl;
        std::cout << "回复时间: " << responseTime << std::endl;
        std::cout << "处理管理员: " << adminUser << std::endl;
    }
    std::cout << "------------------------" << std::endl;
}

inline void Complaint::displayBriefInfo() const {
    std::cout << complaintId << " | " << productName << " | " << title
        << " | " << getStatusText() << " | " << complaintTime << std::endl;
}

inline void Complaint::processComplaint(const std::string& responseContent, const std::string& adminUsername) {
    response = responseContent;
    adminUser = adminUsername;
    status = "resolved";
    setResponseTime();
}

inline std::string Complaint::getStatusText() const {
    if (status == "pending") return "待处理";
    if (status == "processing") return "处理中";
    if (status == "resolved") return "已解决";
    if (status == "closed") return "已关闭";
    return "未知状态";
}

inline bool Complaint::isProcessed() const {
    return status == "resolved" || status == "closed";
}

inline std::string Complaint::toString() const {
    std::ostringstream oss;
    oss << complaintId << "|" << productId << "|" << productName << "|"
        << complainant << "|" << complaintType << "|" << title << "|"
        << content << "|" << complaintTime << "|" << status << "|"
        << response << "|" << responseTime << "|" << adminUser;
    return oss.str();
}

inline Complaint Complaint::fromString(const std::string& data) {
    std::istringstream iss(data);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(iss, token, '|')) {
        tokens.push_back(token);
    }

    if (tokens.size() >= 9) {
        Complaint complaint;
        complaint.complaintId = tokens[0];
        complaint.productId = tokens[1];
        complaint.productName = tokens[2];
        complaint.complainant = tokens[3];
        complaint.complaintType = tokens[4];
        complaint.title = tokens[5];
        complaint.content = tokens[6];
        complaint.complaintTime = tokens[7];
        complaint.status = tokens[8];

        if (tokens.size() > 9) {
            complaint.response = tokens[9];
        }
        if (tokens.size() > 10) {
            complaint.responseTime = tokens[10];
        }
        if (tokens.size() > 11) {
            complaint.adminUser = tokens[11];
        }
        return complaint;
    }
    return Complaint();
}

#endif // COMPLAINT_H
