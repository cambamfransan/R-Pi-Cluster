#ifndef TCPSENDER_WEB_H
#define TCPSENDER_WEB_H

#include <qhostaddress.h>
#include <qtcpsocket.h>
#include <qtcpserver.h>
#include <string>
#include <memory>
#include <rapidjson/document.h>

class TCPSenderWeb : public QObject {
  Q_OBJECT

public:
  TCPSenderWeb();
  ~TCPSenderWeb();

  signals:
    void msgReceived(std::string, QHostAddress, qint16);
    void lostConnection();
    void newConnection();

public:
  qint64 send(const rapidjson::Document&);

  quint16 getServerPort();

private slots: 
  void connection();
  void disconnected();
  void emitMessage();

private:
  std::shared_ptr<QTcpServer> m_pServer;
  std::shared_ptr<QTcpSocket> m_pSocket;
};

#endif