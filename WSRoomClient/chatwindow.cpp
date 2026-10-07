#include "chatwindow.h"

ChatWindow::ChatWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("ChatRoom client");
    this->resize(800, 600);

    auto* central = new QWidget(this);
    this->setCentralWidget(central);
    // chat view
    this->m_chatView = new QTextEdit(this);
    this->m_chatView->setReadOnly(true); // Set Read-Only
    this->m_chatView->setStyleSheet(
        "background:#1e1e1e;"
        "color:#e0e0e0;"
        "font-size:16pt;"
        "border:none;"
    );

    // Input
    this->m_input = new QLineEdit(this);
    this->m_input->setPlaceholderText("输入消息，回车发送...");
    this->m_input->setStyleSheet("font-size:11pt; padding:6px;");

    // Send Button
    this->m_sendBtn = new QPushButton(this);
    this->m_sendBtn->setText("发送");
    this->m_sendBtn->setStyleSheet("font-size:11pt; padding:6px 16px;");

    // Layout
    auto* hbox = new QHBoxLayout(); // No Bind
    hbox->addWidget(this->m_input);
    hbox->addWidget(this->m_sendBtn);

    auto* vbox = new QVBoxLayout(central);
    vbox->addWidget(this->m_chatView);
    vbox->addLayout(hbox);

    // connect
    connect(this->m_sendBtn, &QPushButton::clicked,     this, &ChatWindow::OnSendClicked);
    connect(m_input,   &QLineEdit::returnPressed,       this, &ChatWindow::OnReturnPressed);

}

ChatWindow::~ChatWindow() = default;

void ChatWindow::OnSendClicked()
{
    QString text = this->m_input->text().trimmed();
    if(text.isEmpty()) return;

    //AppendLine("[" + this->m_nickname + "]：" + text, QColor("#f0f0f0"));
    this->m_ws->sendTextMessage(text);
    this->m_input->clear();
}

void ChatWindow::OnReturnPressed()
{
    OnSendClicked();
}

void ChatWindow::OnConnected()
{
    AppendLine("成功连接服务端!!!", Qt::green);
    this->m_ws->sendTextMessage(this->m_nickname); // Send nickname
}

void ChatWindow::OnDisconnected()
{
    AppendLine("与服务端断开连接!!!", Qt::red);
    m_input->setEnabled(false);
    m_sendBtn->setEnabled(false);
}

void ChatWindow::OnTextMessage(const QString& msg)
{
    if(msg == "DO_CLS")
    {
        this->m_chatView->clear();
        return;
    }
    this->ParseAndAppend(msg);
}

void ChatWindow::ConnectTo(const QString &url, const QString &nickname)
{
    this->m_nickname = nickname;
    // Create a new WebSocket
    this->m_ws = new QWebSocket(QString(), QWebSocketProtocol::VersionLatest, this);

    connect(this->m_ws, &QWebSocket::connected,             this, &ChatWindow::OnConnected);
    connect(this->m_ws, &QWebSocket::disconnected,          this, &ChatWindow::OnDisconnected);
    connect(this->m_ws, &QWebSocket::textMessageReceived,   this, &ChatWindow::OnTextMessage);

    this->AppendLine("连接 " + url + " 中...", Qt::yellow);
    this->m_ws->open(QUrl(url));
}

void ChatWindow::ParseAndAppend(const QString &raw)
{
    if(raw.startsWith("[G]"))
    {
        this->AppendLine(raw.mid(3), QColor("#4CAF50")); // Green
    }
    else if (raw.startsWith("[W]"))
    {
        AppendLine(raw.mid(3), QColor("#e0e0e0")); // White
    }
    else if (raw.startsWith("[R]"))
    {
        AppendLine(raw.mid(3), QColor("#F44336")); // Red
    }
    else
    {
        AppendLine(raw, QColor("#e0e0e0"));
    }
}


void ChatWindow::AppendLine(const QString& text, const QColor& color)
{
    QString time = QDateTime::currentDateTime().toString("[HH:mm:ss]");

    QString safeStr = text.toHtmlEscaped();
    safeStr.replace("\n", "<br>");

    QString html = QString(
       "<span style='color:%1;'>%2%3</span>"
       ).arg(color.name())
       .arg(time.toHtmlEscaped())
       .arg(safeStr);

    this->m_chatView->append(html);
    // Keep at the end of chatView
    this->m_chatView->moveCursor(QTextCursor::End);
}