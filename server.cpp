#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#ifdef _WIN32
#include <Windows.h>
#else
#include <clocale>
#endif

#include "ChatServer.h"

static void SetupConsoleUtf8()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#else
    std::setlocale(LC_ALL, "");
#endif
}

int main()
{
    SetupConsoleUtf8();

    const char* port_env = std::getenv("PORT");
    std::uint16_t port = 9527;
    if (port_env != nullptr) {
        port = static_cast<std::uint16_t>(std::stoi(port_env));
    }

    ChatServer server;
    server.Run(port);
    return 0;
}