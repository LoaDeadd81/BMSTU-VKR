#ifndef SRC_ERROR_H
#define SRC_ERROR_H

#include <exception>
#include <string>
#include <utility>

using namespace std;

class Error: public exception {
private:
    string message_;
public:
    explicit Error(std::string  message);
    [[nodiscard]] const char* what() const noexcept override {
        return message_.c_str();
    }
};

Error::Error(std::string  message) : message_(std::move(message)) {

}

#endif //SRC_ERROR_H
