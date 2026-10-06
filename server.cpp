#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include "ChatServer.h"

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    ChatServer server;
    server.Run(9527);
    return 0;
}