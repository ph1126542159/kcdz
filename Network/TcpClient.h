#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QRunnable>
namespace kcdz {
class Handler;
class TcpClient : public QObject,public QRunnable
{
    Q_OBJECT
    class Private;
    Q_PROPERTY(Handler* hanler READ hanler WRITE setHanler)
    Q_PROPERTY(QString ip READ ip WRITE setIp)
    Q_PROPERTY(qint16 port READ port WRITE setPort)
public:
    Q_INVOKABLE void stop();
    Q_INVOKABLE void start();
    Q_INVOKABLE void restart();
    explicit TcpClient(QObject *parent = nullptr);
    virtual ~TcpClient();
    void run()override;
    void setHanler(Handler *);
    Handler *hanler() const;
    void setIp(const QString& value);
    QString ip()const;
    qint16 port( );
    void setPort(qint16 value)const;
signals:
    void connected();
    void disConnected();
private:
    Private* p;
};
}


#endif // TCPCLIENT_H
