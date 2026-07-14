#pragma once
#include <functional>
#include <string>

class ErrorNotifier {
public:
    using Callback = std::function<void(const std::string &message, const std::string &title)>;
    static void setCallback(Callback cb) {
        getCallback() = cb;
    }

    static void notify(const std::string &message, const std::string &title = "Error") {
        auto &cb = getCallback();
        if (cb) cb(message, title);
    }

private:
    static Callback &getCallback() {
        static Callback g_callback = nullptr;
        return g_callback;
    }
};
