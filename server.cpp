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

    ChatServer server;
    server.Run(9527);
    return 0;
}