# QymChatRoom

A simple chat room using **websocketpp** (server) and **QWebSocket** (client).

一个简易聊天室，由 C++ 实现：

- 服务端：C++ / WebSocket++ / Asio / 控制台程序
- 客户端：C++ / Qt 6 / QWebSocket / Qt Widgets

服务端提供 Visual Studio 2022 解决方案，客户端使用 CMake 构建，支持 Windows / Linux 等平台按需编译。

---

## 项目结构

```text
QymChatRoom/
├─ WSRoom/                  # 服务端
│  ├─ ChatServer.h
│  ├─ ChatServer.cpp
│  ├─ server.cpp
│  └─ WSRoom.sln
├─ WSRoomClient/            # 客户端
│  ├─ chatwindow.h
│  ├─ chatwindow.cpp
│  ├─ main.cpp
│  └─ CMakeLists.txt
├─ third-party/             # 第三方依赖
│  ├─ websocketpp/
│  └─ asio/                 # 可选，使用 vcpkg 后可不保留
├─ THIRD-PARTY-NOTICES.md
└─ LICENSE
```

---

## 功能

- 昵称登录，服务端检查昵称是否重复
- 上线 / 下线广播
- 群聊广播
- 私聊：`tell [用户名] [消息]`
- 在线名单：`GetWho`
- 个人面板：`GetMe`
- 查询可用指令：`GetCmd` / `Command`
- 清屏：`cls` / `clear`
- 退出：`exit` / `quit`
- 客户端消息按颜色显示：
  - `[G]`：绿色
  - `[W]`：白色
  - `[R]`：红色
  - 无前缀：默认白色

> 当前服务端主要使用 `[G]` 和 `[R]`，`[W]` 由客户端保留支持。

---

## 消息协议

### 客户端 -> 服务端

| 消息 | 说明 |
|---|---|
| `昵称` | 未登录时发送，服务端将其作为昵称注册 |
| `普通文本` | 登录后发送，作为群聊消息广播 |
| `GetCmd` / `Command` | 查询可用指令 |
| `GetWho` | 查询在线人员 |
| `GetMe` | 查看个人面板 |
| `tell 用户名 消息` | 私聊指定用户 |
| `cls` / `clear` | 请求清屏 |
| `exit` / `quit` | 请求断开连接 |

### 服务端 -> 客户端

| 消息 | 说明 |
|---|---|
| `[G]文本` | 客户端绿色显示 |
| `[W]文本` | 客户端白色显示，当前服务端未主动使用 |
| `[R]文本` | 客户端红色显示 |
| `DO_CLS` | 客户端收到后清空聊天区 |
| 其他文本 | 客户端默认白色显示 |

### 示例

登录：

```text
C -> S: 张三
S -> C: [G]【系统】张三 已上线!
S -> C: [G]【系统】欢迎张三~当前共1人：张三
S -> C: [G]【系统】输入GetCmd或Command查看可用指令
```

群聊：

```text
C -> S: 大家好
S -> C: [张三]：大家好
```

私聊：

```text
C -> S: tell 李四 你好
S -> 李四: 【私聊】张三->你：你好
S -> 张三: 【私聊】你->李四：你好
```

清屏：

```text
C -> S: cls
S -> C: DO_CLS
```

---

## 使用

### 方式一：直接下载（推荐）

前往 [Releases](https://github.com/qym81/QymChatRoom/releases) 页面，下载对应 zip 压缩包，解压后直接运行：

- 服务端：运行 `WSRoom.exe`，默认监听端口 `9527`
- 客户端：运行 `WSRoomClient.exe`，输入服务端地址和昵称

### 方式二：从源码编译

#### 服务端

使用 vcpkg 管理 Asio 依赖：

```bash
vcpkg install boost-asio
vcpkg integrate install
```

然后用 Visual Studio 2022 打开 `WSRoom/WSRoom.sln`，编译运行即可。

服务端默认端口在 `server.cpp` 中：

```cpp
server.Run(9527);
```

如需修改端口，直接修改该处并重新编译。

#### 客户端

1. 安装 Qt 6.x，确保安装时勾选 **Qt WebSockets** 模块
2. 使用 Qt Creator 打开 `WSRoomClient/CMakeLists.txt`
3. 选择 **Qt 6.x + MSVC 2022 64bit** 或其他可用 Kit
4. 配置、编译并运行
5. 若 CMake 提示找不到 WebSockets，请确认 `CMakeLists.txt` 中已链接 `Qt6::WebSockets`

客户端默认服务端地址在 `main.cpp` 中：

```cpp
"ws://10.94.136.32:9527"
```

本地测试时可改为：

```text
ws://127.0.0.1:9527
```

---

## 快速开始

1. 启动服务端：

   ```text
   WSRoom.exe
   ```

   控制台显示：

   ```text
   监听 9527 成功
   ```

2. 启动客户端，输入服务端地址：

   ```text
   ws://127.0.0.1:9527
   ```

3. 输入昵称，例如 `张三`

4. 连接成功后，客户端显示：

   ```text
   成功连接服务端!!!
   ```

5. 输入消息或指令开始聊天：

   ```text
   GetCmd
   GetWho
   GetMe
   tell 李四 你好
   cls
   exit
   ```

---

## 指令说明

| 指令 | 说明 | 示例 |
|---|---|---|
| `GetCmd` / `Command` | 查看所有可用指令 | `GetCmd` |
| `GetWho` | 查看当前在线人员 | `GetWho` |
| `GetMe` | 查看个人昵称和在线时长 | `GetMe` |
| `tell 用户名 消息` | 给指定用户发送私聊 | `tell 李四 你好` |
| `cls` / `clear` | 清空聊天窗口 | `cls` |
| `exit` / `quit` | 退出聊天室 | `exit` |

---

## 依赖

| 库 | 版本 | 许可证 |
|---|---|---|
| Qt | 6.x | LGPLv3 |
| websocketpp | 0.8.x | BSD-3-Clause |
| Asio | 1.28.x 或由 vcpkg `boost-asio` 提供 | Boost Software License 1.0 |

详见 [THIRD-PARTY-NOTICES.md](THIRD-PARTY-NOTICES.md)。

---

## 常见问题

### 1. 客户端连接失败

请检查：

- 服务端是否已启动
- 地址和端口是否正确，例如 `ws://127.0.0.1:9527`
- 防火墙是否放行 `9527` 端口
- 局域网连接时是否使用服务端机器的局域网 IP

### 2. 中文乱码

服务端已在 Windows 下设置控制台 UTF-8：

```cpp
SetConsoleOutputCP(CP_UTF8);
```

请确保源码文件保存为 UTF-8，并使用支持中文的终端字体。

### 3. 昵称重复

服务端会拒绝重复昵称，并返回红色错误提示。重新输入新的昵称发送即可。

### 4. 私聊失败

如果提示用户不存在或未在线，请使用 `GetWho` 查看当前在线名单，确认昵称完全一致。

### 5. 客户端没有显示连接错误详情

当前客户端主要处理连接成功、断开和文本消息。若需要更完整的错误提示，可连接 `QWebSocket::errorOccurred` 信号并显示错误信息。

---

## 已知限制

- 当前仅支持 `ws://`，未启用 TLS，不支持 `wss://`
- 无密码认证，昵称即身份，存在冒用可能
- 无历史消息、离线消息、图片、文件传输
- 服务端数据保存在内存中，重启后丢失
- 广播时持锁发送，适合小型聊天室，不适合大规模并发
- 客户端默认服务端地址硬编码在 `main.cpp` 中

---

## 可扩展方向

- 支持 `wss://` / TLS
- 增加房间、频道、管理员权限
- 使用 SQLite 保存历史消息
- 增加心跳、超时断开、自动重连
- 将协议改为 JSON，便于扩展字段
- 增加客户端连接错误提示和重连按钮
- 将端口、地址、默认昵称改为配置文件或命令行参数

---

## License

[MIT](LICENSE)