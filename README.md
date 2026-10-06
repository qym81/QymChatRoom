# QymChatRoom

A simple chat room using **websocketpp** (server) and **QWebSocket** (client).

## 项目结构

- `WSRoom/`        - 服务端（C++ / WebSocket++ / Asio）
- `WSRoomClient/`  - 客户端（C++ / Qt 6 / QWebSocket）
- `third-party/`   - 依赖库（websocketpp、asio）

## 功能

- 昵称登录（重名检查）
- 上线 / 下线广播
- 群聊广播
- 私聊（`tell` 指令）
- 在线名单（`GetWho`）
- 个人面板（`GetMe`）
- 查询可用指令（`GetCmd` / `Command`）
- 清屏（`cls` / `clear`）
- 退出（`exit` / `quit`）

## 使用

### 方式一：直接下载（推荐）

前往 [Releases](https://github.com/qym81/QymChatRoom/releases) 页面，
下载对应的 zip 压缩包，解压后直接运行：

- 服务端：运行 `WSRoom.exe`，默认端口 `9527`
- 客户端：运行 `WSRoomClient.exe`，输入服务端地址和昵称

### 方式二：从源码编译

#### 服务端
用 Visual Studio 2022 打开 `WSRoom/WSRoom.sln`，直接编译。

#### 客户端
用 Qt Creator 打开 `WSRoomClient/CMakeLists.txt`，
选择 **Qt 6.x + MSVC 2022 64bit**，编译运行。

## 快速开始

1. 启动服务端（默认端口 `9527`）
2. 启动客户端，输入服务端地址和昵称
3. 开始聊天

## 依赖

| 库 | 版本 | 许可证 |
|----|------|--------|
| Qt | 6.x | LGPLv3 |
| websocketpp | 0.8.x | BSD-3-Clause |
| Asio | 1.28.x | Boost Software License 1.0 |

详见 [THIRD-PARTY-NOTICES.md](THIRD-PARTY-NOTICES.md)。

## License

[MIT](LICENSE)