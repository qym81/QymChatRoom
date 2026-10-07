# 使用 Ubuntu 作为基础镜像，兼容性好且体积适中
FROM ubuntu:22.04

# 安装编译工具和必要的依赖
# libasio-dev 用于独立 Asio，libboost-system-dev 和 libboost-thread-dev 用于 websocketpp
RUN apt-get update && apt-get install -y \
    build-essential \
    libasio-dev \
    libboost-system-dev \
    libboost-thread-dev \
    && rm -rf /var/lib/apt/lists/*

# 设置工作目录
WORKDIR /app

# 将当前目录下的所有文件复制到容器中
COPY . .

# 编译服务端
# -IWSRoom 让编译器找到 ChatServer.h
# -Ithird-party 让编译器找到 websocketpp 和 asio
# -DASIO_STANDALONE 告诉 websocketpp 使用独立的 Asio 而非 Boost.Asio
RUN g++ -std=c++11 -O2 -I. -Ithird-party -DASIO_STANDALONE \
    -o chat_server ChatServer.cpp server.cpp \
    -lpthread

# 暴露端口（Back4App 要求必须有 EXPOSE）
EXPOSE 9527

# 启动服务端
CMD ["./chat_server"]