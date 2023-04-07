#ifndef MIMEDATA_H
#define MIMEDATA_H

#include <QObject>
#include <QUrl>
#include <QVariantMap>


namespace kcdz{
class MimeDataPrivate;
class  MimeData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QString html READ html WRITE setHtml NOTIFY htmlChanged)
    Q_PROPERTY(QList<QUrl> urls READ urls WRITE setUrls NOTIFY urlsChanged)
    Q_PROPERTY(QVariantMap dataMap READ dataMap WRITE setDataMap NOTIFY dataMapChanged)
    Q_PROPERTY(QStringList formats READ formats NOTIFY dataMapChanged)

public:
    MimeData(QObject *parent = Q_NULLPTR);
    virtual ~MimeData();

    void setText(const QString &txt);
    QString text() const;

    void setHtml(const QString &html);
    QString html() const;

    void setUrls(const QList<QUrl> &urls);
    QList<QUrl> urls() const;

    void setDataMap(const QVariantMap &map);
    QVariantMap dataMap() const;
    QStringList formats() const;

public Q_SLOTS:
    void fetchClipboard();
    QString getDataAsString(const QString &type);

Q_SIGNALS:
    void textChanged();
    void htmlChanged();
    void urlsChanged();
    void dataMapChanged();

private:
    MimeDataPrivate *p;
};

}

Q_DECLARE_METATYPE(kcdz::MimeData*)

#endif // MIMEDATA_H
