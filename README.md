# QymChatRoom
A simple chat room using websocktpp and QWebSocket

## 项目结构

- `WSRoom/`        - 服务端（C++ / WebSocket++ / Asio）
- `WSRoomClient/`  - 客户端（C++ / Qt 6 / QWebSocket）
- `third-party/`   - 依赖库（websocketpp、asio）

## 功能

- 昵称登录（重名检查）
- 上线 / 下线广播
- 群聊广播
- 私聊（tell 指令）
- 在线名单（GetWho）
- 个人面板（GetMe）
- 清屏（cls / clear）
- 退出（exit / quit）

## 编译

### 服务端
用 Visual Studio 2022 打开 `WSRoom/WSRoom.sln`，直接编译。

### 客户端
用 Qt Creator 打开 `WSRoomClient/CMakeLists.txt`，
选择 **Qt 6.x + MSVC 2022 64bit**，编译运行。

## 使用

1. 启动服务端（默认端口 9527）
2. 启动客户端，输入服务端地址和昵称
3. 开始聊天

## 依赖

- Qt 6（LGPLv3）
- websocketpp（BSD-3-Clause）
- Asio（Boost Software License 1.0）

## License

MIT
