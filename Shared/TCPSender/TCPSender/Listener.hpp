#include <qtcpserver.h>
#include <qtcpsocket.h>
#include <qdatastream.h>

class TCPListener : public QObject
{
  Q_OBJECT
    signals :
  void msgReceived(std::string, QHostAddress, qint16);

public:
  TCPListener(QHostAddress ip, qint16 port);
  ~TCPListener();

  void connection();

private:
  QTcpSocket* m_pSocket;
  QHash<QTcpSocket*, QByteArray*> m_buffers;

  private slots:
  void readStream();
};