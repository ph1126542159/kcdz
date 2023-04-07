#ifndef SERIALPORTHELPER_H
#define SERIALPORTHELPER_H

#include <QObject>
#include <QRunnable>
#include <QVariant>
#include <QtSerialPort/QSerialPort>

namespace kcdz {
class Handler;
class SerialPortHelper : public QObject,public QRunnable
{
    Q_OBJECT
    class Private;
    Q_PROPERTY(QString comName READ comName WRITE setComName)
    Q_PROPERTY(QVariant baudRate READ baudRate WRITE setBaudRate)
    Q_PROPERTY(QVariant  dataBits READ dataBits WRITE setDataBits)
    Q_PROPERTY(QVariant parity READ parity WRITE setParity)
    Q_PROPERTY(QVariant stopBits READ stopBits WRITE setStopBits)
    Q_PROPERTY(QVariant flowControl READ flowControl WRITE setFlowControl)
    Q_PROPERTY(Handler* hanler READ hanler WRITE setHanler)
public:
    Q_INVOKABLE void stop();
    Q_INVOKABLE void start();
    Q_INVOKABLE void restart();
    explicit SerialPortHelper(QObject *parent = nullptr);

    virtual ~SerialPortHelper();

    static QString byteArrayToHexString(QByteArray ascii);

    static QString byteArrToString(const char *data,int nLen);

    static QByteArray hexStringToByteArray(QString HexString);


    QString comName();
    void setComName(const QString& value);


    QVariant baudRate();
    void setBaudRate(const QVariant& value);


    QVariant dataBits();
    void setDataBits(const QVariant& value);


    QVariant  parity();
    void setParity(const QVariant & value);


    QVariant stopBits();
    void setStopBits(const QVariant& value);


    QVariant flowControl();
    void setFlowControl(const QVariant& value);

    void setHanler(Handler *);
    Handler *hanler() const;
Q_SIGNALS:
    void serialPortConnected();
    void serialPortDisConnected();
    void serialPortListChanged(const QVariant& listStr);
protected:
    void run() override;

private:
    Private* p;
};
}


#endif // SERIALPORTHELPER_H
