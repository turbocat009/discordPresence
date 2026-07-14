#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <functional>

#include "discordpp.h"
#include "Math.hpp"

class DiscordFunc {
    public:
        void initDiscordSDK();
        static void logIn(const uint64_t APPLICATION_ID, discordpp::Client *clientRef);
        static void logOut();
        static void setPresence(std::string &Name, std::string &Desc, std::string &Type, std::string &IMG);
        static std::shared_ptr<discordpp::Client> getClient();
        static std::string getToken();
        static bool isLogIngIn;
        static std::string getCurrentUsername();
<<<<<<< HEAD
        static long long getCurrentID();
=======
        static long getCurrentID();
        static int WhichErrorNow;
        static bool hasConnectionError() { return WhichErrorNow == 1; }
>>>>>>> e86b16e (Implemented PopUP)
    private:
        static std::shared_ptr<discordpp::Client> client;
        static std::string token;
        static std::function<void(std::string)> authErrorCallback;
    protected:
    public:
        static void setAuthErrorCallback(std::function<void(std::string)> callback);
};