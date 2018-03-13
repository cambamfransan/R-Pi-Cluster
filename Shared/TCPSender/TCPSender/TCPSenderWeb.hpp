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
    void msgReceived(std::string);
    void lostConnection();
    void newConnection();

public:
  qint64 send(std::string toSend);

  quint16 getServerPort();
  int nextConvId();

private slots: 
  void connection();
  void disconnected();
  void emitMessage();

private:
  int m_convId;
  std::shared_ptr<QTcpServer> m_pServer;
  std::shared_ptr<QTcpSocket> m_pSocket;
};

#endif
