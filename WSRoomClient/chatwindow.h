#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include <QWebSocket>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QDateTime>
#include <QTextCursor>
#include <QColor>

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = nullptr);
    ~ChatWindow() override;

    // Functions
    void OnTextMessage(const QString& msg);
    void OnSendClicked();
    void OnReturnPressed();
    void OnConnected();
    void OnDisconnected();
    void ConnectTo(const QString& url, const QString& nickname);

private:
    void AppendLine(const QString& text, const QColor& color);
    void ParseAndAppend(const QString& raw);

private:
    QWebSocket* m_ws = nullptr;
    QTextEdit* m_chatView;
    QLineEdit* m_input;
    QPushButton* m_sendBtn;
    QString m_nickname;
};
#endif // CHATWINDOW_H
