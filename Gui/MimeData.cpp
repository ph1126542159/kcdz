#include "MimeData.h"
#include <QClipboard>
#include <QDataStream>
#include <QGuiApplication>
#include <QIODevice>
#include <QMimeData>
namespace kcdz {
class MimeDataPrivate
{
public:
    QString text;
    QString html;
    QList<QUrl> urls;
    QVariantMap dataMap;
};

MimeData::MimeData(QObject *parent) :
    QObject(parent)
{
    p = new MimeDataPrivate;
}

void MimeData::setText(const QString &txt)
{
    if(p->text == txt)
        return;

    p->text = txt;
    Q_EMIT textChanged();
}

QString MimeData::text() const
{
    return p->text;
}

void MimeData::setHtml(const QString &html)
{
    if(p->html == html)
        return;

    p->html = html;
    Q_EMIT htmlChanged();
}

QString MimeData::html() const
{
    return p->html;
}

void MimeData::setUrls(const QList<QUrl> &urls)
{
    if(p->urls == urls)
        return;

    p->urls = urls;
    Q_EMIT urlsChanged();
}

QList<QUrl> MimeData::urls() const
{
    return p->urls;
}

void MimeData::setDataMap(const QVariantMap &map)
{
    if(p->dataMap == map)
        return;

    p->dataMap = map;
    Q_EMIT dataMapChanged();
}

QVariantMap MimeData::dataMap() const
{
    return p->dataMap;
}

QStringList MimeData::formats() const
{
    return p->dataMap.keys();
}

void MimeData::fetchClipboard()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    if(!clipboard) return;
    const QMimeData *mime = clipboard->mimeData();
    if(!mime) return;

    setUrls(mime->urls());
    setText(mime->text());
    setHtml(mime->html());

    QVariantMap data;
    for(const QString &format: mime->formats())
    {
        QVariant var;
        QByteArray bytes = mime->data(format);
        QDataStream stream(&bytes, QIODevice::ReadOnly);
        stream >> var;

        data[format] = var;
    }
    setDataMap(data);
}

QString MimeData::getDataAsString(const QString &type)
{
    return p->dataMap.value(type).toString();
}

MimeData::~MimeData()
{
    delete p;
}
}

