#include "chatwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    bool ok1 = false, ok2 = false;
    QString url = QInputDialog::getText(nullptr, "连接", "请输入服务端地址：",
                                        QLineEdit::Normal,
                                        "ws://10.94.136.32:9527", &ok1);
    if (!ok1 || url.isEmpty()) return 0;

    QString nickname = QInputDialog::getText(nullptr, "昵称", "请输入昵称：",
                                             QLineEdit::Normal, "张三", &ok2);
    if (!ok2 || nickname.isEmpty()) return 0;

    ChatWindow w;
    w.show();
    w.ConnectTo(url, nickname);
    return QApplication::exec();
}
