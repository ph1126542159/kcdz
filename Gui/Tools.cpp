#include "Tools.h"
#include <QMetaMethod>
#include <QMetaObject>
#include <QCryptographicHash>
#include <QCoreApplication>
#include <QColor>
#include <QTimer>
#include <QFile>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QStringList>
#include <QTextDocument>
#include <QRegularExpression>
#if !defined(Q_OS_IOS) && !defined(Q_OS_WASM)
#include <QProcess>
#endif
#include <QTimerEvent>
#include <QUuid>
#include <QMimeDatabase>
#include <QImageReader>
#include <QJsonDocument>
#include <QThread>
#include <QImageWriter>

#ifndef Q_OS_WASM
#include <QRegularExpression>
#include <QRunnable>
#include <QThreadPool>
#endif
namespace kcdz {
class ToolsPrivate
{
public:
};

Tools::Tools(QObject *parent) :
    QObject(parent)
{
    p = new ToolsPrivate;
}

void Tools::debug(const QVariant &var)
{
    qDebug() << var;
}

QString Tools::localFilesPrePath()
{
#ifdef Q_OS_WIN
    return QStringLiteral("file:///");
#else
    return QStringLiteral("file://");
#endif
}

QDateTime Tools::currentDate()
{
    return QDateTime::currentDateTime();
}

QString Tools::dateToMSec(const QDateTime &dt)
{
    return QString::number(dt.toMSecsSinceEpoch());
}

QDateTime Tools::mSecToDate(const QString &ms)
{
    return QDateTime::fromMSecsSinceEpoch(ms.toLongLong());
}

qint64 Tools::dateToSec(const QDateTime &dt)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 8, 0))
    return dt.toSecsSinceEpoch();
#else
   return dt.toMSecsSinceEpoch()/1000;
#endif
}

QDateTime Tools::dateFromSec(qint64 date)
{
    return QDateTime::fromSecsSinceEpoch(date);
}

QString Tools::dateToString(const QDateTime &dt, const QString & format)
{
    if(format.isEmpty())
        return dt.toString();
    else
        return dt.toString(format);
}

QDateTime Tools::datefromString(const QString &dt, const QString &format)
{
    if(format.isEmpty())
        return QDateTime::fromString(dt);
    else
        return QDateTime::fromString(dt, format);
}

QString Tools::msecToTimeString(qint32 msec, const QString &format)
{
    return QTime::fromMSecsSinceStartOfDay(msec).toString(format);
}

QDate Tools::dateAddDays(const QDate &date, int days)
{
    return date.addDays(days);
}

QStringList Tools::toStringList(const QVariant &value)
{
    return value.toStringList();
}

QVariantMap Tools::toVariantMap(const QVariant &value)
{
    return value.toMap();
}

QVariantList Tools::toVariantList(const QVariant &value)
{
    return value.toList();
}

QString Tools::byteArrayToString(const QVariant &value)
{
    return QString::fromUtf8(value.toByteArray());
}

QByteArray Tools::compress(const QByteArray &data, int level)
{
    return qCompress(data, level);
}

QByteArray Tools::uncompress(const QByteArray &data)
{
    return qUncompress(data);
}

QVariant Tools::bytesToVariant(const QByteArray &_data)
{
    QVariant result;
    QByteArray data = _data;
    QDataStream stream(&data, QIODevice::ReadOnly);
    stream >> result;
    return result;
}

QByteArray Tools::variantToBytes(const QVariant &var)
{
    QByteArray result;
    QDataStream stream(&result, QIODevice::WriteOnly);
    stream << var;
    return result;
}
QString Tools::fileName(const QString &_path)
{
    QString path = _path;
    if(path.left(localFilesPrePath().size()) == localFilesPrePath())
        path = path.mid(QString("file:///").size());
    return QFileInfo(path).baseName();
}

QString Tools::fileSuffix(const QString &_path)
{
    QString path = _path;
    if(path.left(Tools::localFilesPrePath().size()) == Tools::localFilesPrePath())
        path = path.mid(Tools::localFilesPrePath().size());

    QString pathRight = path.right(10);
    QString result = pathRight.mid(pathRight.lastIndexOf(QStringLiteral("."))+1);
    if(!result.isEmpty())
        return result;

    result = QFileInfo(path).suffix().toLower();
    if(!result.isEmpty())
        return result;

    QMimeDatabase db;
    const QStringList &suffixes = db.mimeTypeForFile(path).suffixes();
    if(!suffixes.isEmpty())
        result = suffixes.first().toLower();

    return result;
}

QString Tools::fileMime(const QString &path)
{
    QMimeDatabase db;
    return db.mimeTypeForFile(path).name();
}

QString Tools::fileParent(const QString &path)
{
    if(path.count() == 1 && path[0] == '/')
        return path;
#ifdef Q_OS_WIN
    if(path.count() == 3 && path[0].isLetter() && path[1] == ':')
        return path;
#endif
    QString res = path.mid(0, path.lastIndexOf(QStringLiteral("/")));
#ifndef Q_OS_WIN
    if(res.isEmpty())
        res += "/";
#endif
    return res;
}

QString Tools::fileSize(const QString &_path)
{
    QString path = _path;
    if(path.left(Tools::localFilesPrePath().size()) == Tools::localFilesPrePath())
        path = path.mid(Tools::localFilesPrePath().size());
    return QString::number(QFileInfo(path).size());
}

QString Tools::readText(const QString &path)
{
    QFile file(path);
    if( !file.open(QFile::ReadOnly) )
        return QString();

    QString res = QString::fromUtf8(file.readAll());
    return res;
}

bool Tools::writeText(const QString &path, const QString &text)
{
    QFile file(path);
    if( !file.open(QFile::WriteOnly) )
        return false;

    file.write(text.toUtf8());
    file.close();
    return true;
}

bool Tools::fileExists(const QString &_path)
{
    QString path = _path;
    if(path.left(Tools::localFilesPrePath().size()) == Tools::localFilesPrePath())
        path = path.mid(Tools::localFilesPrePath().size());

    return QFileInfo::exists(path);
}

QStringList Tools::filesOf(const QString &path)
{
    return QDir(path).entryList(QDir::Files);
}

QSize Tools::imageSize(const QString &_path)
{
    QString path = _path;
    if(path.left(Tools::localFilesPrePath().size()) == Tools::localFilesPrePath())
        path = path.mid(Tools::localFilesPrePath().size());

    QImageReader image(path);
    return image.size();
}

void Tools::imageResize(const QString &_path, const QSize &size, const QString &dest, QObject *base, std::function<void (bool)> callback)
{
#ifndef Q_OS_WASM
    QString path = _path;
    if(path.left(Tools::localFilesPrePath().size()) == Tools::localFilesPrePath())
        path = path.mid(Tools::localFilesPrePath().size());

    class ResizeRunnable: public QRunnable {
        void run() {
            QImageReader reader(_path);
            reader.setScaledSize(_size);

            QImage img = reader.read();
            QImageWriter writer(_dest);
            bool res = writer.write(img);

            if(!_base)
                return;

            QObject *obj = new QObject;

            auto callback = _callback;
            obj->connect(obj, &QObject::destroyed, _base, [callback, res](){
                callback(res);
            }, Qt::QueuedConnection);

            delete obj;
        }

    public:
        QString _path;
        QSize _size;
        QString _dest;
        QPointer<QObject> _base;
        std::function<void (bool)> _callback;
    };

    ResizeRunnable *run = new ResizeRunnable;
    run->setAutoDelete(true);
    run->_path = path;
    run->_size = size;
    run->_dest = dest;
    run->_base = base;
    run->_callback = callback;

    QThreadPool::globalInstance()->start(run);
#endif
}

bool Tools::writeFile(const QString &path, const QVariant &data, bool compress)
{
    QString dir = path.left(path.lastIndexOf(QStringLiteral("/"))+1);
    QDir().mkpath(dir);

    QFile file(path);
    if(!file.open(QFile::WriteOnly))
        return false;

    QByteArray bytes;
    if(data.type() == QVariant::ByteArray)
        bytes = data.toByteArray();
    else
    {
        QDataStream stream(&bytes, QIODevice::WriteOnly);
        stream << data;
    }

    if(compress)
        bytes = qCompress(bytes);

    file.write(bytes);
    file.close();
    return true;
}

QByteArray Tools::readFile(const QString &path, bool uncompress)
{
    QFile file(path);
    if(!file.open(QFile::ReadOnly))
        return QByteArray();

    QByteArray res = file.readAll();
    file.close();

    if(uncompress)
        res = qUncompress(res);

    return res;
}

QString Tools::className(QObject *obj)
{
    QString result;
    if(obj)
        result = QString::fromUtf8(obj->metaObject()->className());

    int idx = result.indexOf(QStringLiteral("_QMLTYPE"));
    if(idx >= 0)
        result = result.left(idx+8);
    return result;
}

QStringList Tools::stringLinks(const QString &str)
{
    QStringList links;
    QRegularExpression links_rxp(QStringLiteral("((?:(?:\\w\\S*\\/\\S*|\\/\\S+|\\:\\/)(?:\\/\\S*\\w|\\w))|(?:\\w+\\.(?:com|org|co|net)))"));
    auto i = links_rxp.globalMatch(str);
    while (i.hasNext())
    {
        auto m = i.next();
        QString link = m.captured(1);
        if(link.indexOf(QRegularExpression(QStringLiteral("\\w+\\:\\/\\/"))) == -1)
            link = QStringLiteral("http://") + link;

        links << link;
    }

    return links;
}

QUrl Tools::stringToUrl(const QString &path)
{
    return QUrl(path);
}

QString Tools::urlToLocalPath(const QUrl &url)
{
    QString res = url.toLocalFile();
    if(res.isEmpty() || url.toString().contains(QStringLiteral("#")))
        res = url.toString();

#ifdef Q_OS_WIN
    if (url.toLocalFile().left(2) == QStringLiteral("//"))
        return QStringLiteral("//") + res.mid(res.indexOf(QStringLiteral("//")) + 2);
#endif
    if(res.left(6) == QStringLiteral("file:/"))
        res = res.mid(6);

    int idx1 = res.indexOf(QStringLiteral(":/"));
    if(idx1 != -1)
    {
        int idx2 = res.lastIndexOf(QStringLiteral(":/"));
        if(idx2 != -1)
            res.remove(idx1, idx2-idx1);
    }

    if(res.left(Tools::localFilesPrePath().size()) == Tools::localFilesPrePath())
        res = res.mid(Tools::localFilesPrePath().size());
    if(res.left(5) == QStringLiteral("qrc:/"))
        res = res.mid(3);

#ifdef Q_OS_WIN
    while(res.count() && res[0] == '/')
#else
    while(res.count() > 1 && res[0] == '/' && res[1] == '/')
#endif
          res = res.mid(1);

    return res;
}

QString Tools::qtVersion()
{
    return QString::fromUtf8(qVersion());
}

QString Tools::aboutAseman()
{
    return tr("guiFrameWork");
}

void Tools::deleteItemDelay(QObject *o, int ms)
{
    QTimer::singleShot( ms, o, SLOT(deleteLater()) );
}

qreal Tools::colorHue(const QColor &clr)
{
    return clr.hue()/359.0;
}

qreal Tools::colorLightness(const QColor &clr)
{
    return clr.lightness()/255.0;
}

qreal Tools::colorSaturation(const QColor &clr)
{
    return clr.saturation()/255.0;
}

void Tools::mkDir(const QString &pt)
{
    QString path = pt;
    if(path.left(Tools::localFilesPrePath().size()) == Tools::localFilesPrePath())
        path = path.mid(Tools::localFilesPrePath().size());

    QDir().mkpath(path);
}

QVariantMap Tools::colorHsl(const QColor &clr)
{
    QVariantMap res;
    res[QStringLiteral("hue")] = colorHue(clr);
    res[QStringLiteral("lightness")] = colorLightness(clr);
    res[QStringLiteral("saturation")] = colorSaturation(clr);

    return res;
}

bool Tools::createVideoThumbnail(const QString &video, const QString &output, QString ffmpegPath)
{
#if defined(Q_OS_IOS) || defined(Q_OS_WASM)
    Q_UNUSED(video)
    Q_UNUSED(output)
    Q_UNUSED(ffmpegPath)
    return false;
#else
    if(ffmpegPath.isEmpty())
#ifdef Q_OS_WIN
        ffmpegPath = QCoreApplication::applicationDirPath() + "/ffmpeg.exe";
#else
#ifdef Q_OS_MAC
        ffmpegPath = QCoreApplication::applicationDirPath() + "/ffmpeg";
#else
    {
        if(QFileInfo::exists(QStringLiteral("/usr/bin/avconv")))
            ffmpegPath = QStringLiteral("/usr/bin/avconv");
        else
            ffmpegPath = QStringLiteral("ffmpeg");
    }
#endif
#endif

    QStringList args;
    args << QStringLiteral("-itsoffset");
    args << QStringLiteral("-4");
    args << QStringLiteral("-i");
    args << video;
    args << QStringLiteral("-vcodec");
    args << QStringLiteral("mjpeg");
    args << QStringLiteral("-vframes");
    args << QStringLiteral("1");
    args << QStringLiteral("-an");
    args << QStringLiteral("-f");
    args << QStringLiteral("rawvideo");
    args << output;
    args << QStringLiteral("-y");

    QProcess prc;
    prc.start(ffmpegPath, args);
    prc.waitForStarted();
    prc.waitForFinished();

    return prc.exitCode() == 0;
#endif
}

QString Tools::translateNumbers(QString input)
{
    input.replace(QStringLiteral("0"), Tools::tr("0"));
    input.replace(QStringLiteral("1"), Tools::tr("1"));
    input.replace(QStringLiteral("2"), Tools::tr("2"));
    input.replace(QStringLiteral("3"), Tools::tr("3"));
    input.replace(QStringLiteral("4"), Tools::tr("4"));
    input.replace(QStringLiteral("5"), Tools::tr("5"));
    input.replace(QStringLiteral("6"), Tools::tr("6"));
    input.replace(QStringLiteral("7"), Tools::tr("7"));
    input.replace(QStringLiteral("8"), Tools::tr("8"));
    input.replace(QStringLiteral("9"), Tools::tr("9"));
    return input;
}

QString Tools::trNums(QString input)
{
    return Tools::translateNumbers(input);
}

QString Tools::passToMd5(const QString &pass)
{
    if( pass.isEmpty() )
        return QString();

    return md5(pass);
}

QString Tools::md5(const QString &str)
{
    return hash(str, Md5);
}

QString Tools::passToHash(const QString &pass, int method)
{
    if( pass.isEmpty() )
        return QString();

    return hash(pass, method);
}

QString Tools::hash(const QString &str, int method)
{
    QCryptographicHash::Algorithm alg = QCryptographicHash::Md5;
    switch (method)
    {
    case Md5:
        alg = QCryptographicHash::Md5;
        break;
    case Sha224:
        alg = QCryptographicHash::Sha224;
        break;
    case Sha256:
        alg = QCryptographicHash::Sha256;
        break;
    case Sha384:
        alg = QCryptographicHash::Sha384;
        break;
    case Sha512:
        alg = QCryptographicHash::Sha512;
        break;
    case Sha3_224:
        alg = QCryptographicHash::Sha3_224;
        break;
    case Sha3_256:
        alg = QCryptographicHash::Sha3_256;
        break;
    case Sha3_384:
        alg = QCryptographicHash::Sha3_384;
        break;
    case Sha3_512:
        alg = QCryptographicHash::Sha3_512;
        break;
    }

    return QString::fromUtf8(QCryptographicHash::hash(str.toUtf8(), alg).toHex());
}

QString Tools::createUuid()
{
    return QUuid::createUuid().toString();
}

QString Tools::stringRemove(QString str, const QString &text, bool regExp)
{
    if(regExp)
        return str.remove( QRegularExpression(text) );
    else
        return str.remove(text);
}

QString Tools::stringReplace(QString str, const QString &text, const QString &replace, bool regExp)
{
    if(regExp)
        return str.replace(QRegularExpression(text), replace);
    else
        return str.replace(text, replace);
}

QVariantList Tools::stringRegExp(QString str, const QString &regExp, bool)
{
    QRegularExpression rx(regExp);

    QVariantList res;
    auto i = rx.globalMatch(str);
    while (i.hasNext()) {
        auto m = i.next();
        res << m.capturedTexts();
    }

    return res;
}

QString Tools::htmlToPlaintText(const QString &html)
{
    QTextDocument doc;
    doc.setHtml(html);
    return doc.toPlainText();
}

void Tools::copyDirectory(const QString &_src, const QString &_dst)
{
    QString src = _src;
    if(src.left(Tools::localFilesPrePath().size()) == Tools::localFilesPrePath())
        src = src.mid(Tools::localFilesPrePath().size());
    QString dst = _dst;
    if(dst.left(Tools::localFilesPrePath().size()) == Tools::localFilesPrePath())
        dst = dst.mid(Tools::localFilesPrePath().size());

    QDir().mkpath(dst);

    const QStringList & dirs = QDir(src).entryList(QDir::Dirs|QDir::NoDotAndDotDot);
    for(const QString &d: dirs)
        copyDirectory(src+"/"+d, dst+"/"+d);

    const QStringList & files = QDir(src).entryList(QDir::Files);
    for(const QString &f: files)
        QFile::copy(src+"/"+f, dst+"/"+f);
}

bool Tools::copy(const QString &_src, const QString &_dst)
{
    QString src = _src;
    if(src.left(Tools::localFilesPrePath().size()) == Tools::localFilesPrePath())
        src = src.mid(Tools::localFilesPrePath().size());
    QString dst = _dst;
    if(dst.left(Tools::localFilesPrePath().size()) == Tools::localFilesPrePath())
        dst = dst.mid(Tools::localFilesPrePath().size());

    return QFile::copy(src, dst);
}

bool Tools::rename(const QString &_src, const QString &_dst)
{
    QString src = _src;
    if(src.left(Tools::localFilesPrePath().size()) == Tools::localFilesPrePath())
        src = src.mid(Tools::localFilesPrePath().size());
    QString dst = _dst;
    if(dst.left(Tools::localFilesPrePath().size()) == Tools::localFilesPrePath())
        dst = dst.mid(Tools::localFilesPrePath().size());

    return QFile::rename(src, dst);
}

void Tools::deleteFile(const QString &pt)
{
    QString path = pt;
    if(path.left(Tools::localFilesPrePath().size()) == Tools::localFilesPrePath())
        path = path.mid(Tools::localFilesPrePath().size());

    QFile::remove(path);
}

void Tools::clearDirectory(const QString &pt)
{
    QString path = pt;
    if(path.left(Tools::localFilesPrePath().size()) == Tools::localFilesPrePath())
        path = path.mid(Tools::localFilesPrePath().size());

    const QStringList & files = QDir(path).entryList(QDir::Files);
    for(const QString &f: files)
        deleteFile(path+"/"+f);
}

QVariantMap Tools::decodeJwt(const QString &token)
{
    QVariantMap res;

    QStringList parts = token.split(".");
    if (parts.length() > 0) res["header"] = QJsonDocument::fromJson( QByteArray::fromBase64( parts[0].toUtf8() ) ).toVariant();
    if (parts.length() > 1) res["body"] = QJsonDocument::fromJson( QByteArray::fromBase64( parts[1].toUtf8() ) ).toVariant();

    return res;
}

void Tools::setProperty(QObject *obj, const QString &property, const QVariant &v)
{
    if( !obj || property.isEmpty() )
        return;

    obj->setProperty( property.toUtf8(), v );
}

QVariant Tools::property(QObject *obj, const QString &property)
{
    if( !obj || property.isEmpty() )
        return QVariant();

    return obj->property(property.toUtf8());
}

Qt::LayoutDirection Tools::directionOf(const QString &str)
{
    Qt::LayoutDirection res = Qt::LeftToRight;
    if( str.isEmpty() )
        return res;

    int ltr = 0;
    int rtl = 0;

    for(const QChar &ch: str)
    {
        QChar::Direction dir = ch.direction();
        switch( static_cast<int>(dir) )
        {
        case QChar::DirL:
        case QChar::DirLRE:
        case QChar::DirLRO:
        case QChar::DirEN:
            ltr++;
            break;

        case QChar::DirR:
        case QChar::DirRLE:
        case QChar::DirRLO:
        case QChar::DirAL:
            rtl++;
            break;
        }
    }

    if( ltr >= rtl )
        res = Qt::LeftToRight;
    else
        res = Qt::RightToLeft;

    return res;
}

QVariant Tools::call(QObject *obj, const QString &member, Qt::ConnectionType ctype, const QVariant &v0, const QVariant &v1, const QVariant &v2, const QVariant &v3, const QVariant &v4, const QVariant &v5, const QVariant &v6, const QVariant &v7, const QVariant &v8, const QVariant &v9)
{
    const QMetaObject *meta_obj = obj->metaObject();
    QMetaMethod meta_method;
    for( int i=0; i<meta_obj->methodCount(); i++ )
    {
        QMetaMethod mtd = meta_obj->method(i);
        if( mtd.name() == member.toUtf8() )
            meta_method = mtd;
    }
    if( !meta_method.isValid() )
        return QVariant();

    QList<QByteArray> param_types = meta_method.parameterTypes();
    QList<QByteArray> param_names = meta_method.parameterNames();

    QString ret_type = QString::fromUtf8(meta_method.typeName());
    QList< QPair<QString,QString> > m_args;
    for( int i=0 ; i<param_types.count() ; i++ )
        m_args << QPair<QString,QString>( QString::fromUtf8(param_types.at(i)) , QString::fromUtf8(param_names.at(i)) );

    QVariantList vals;
        vals << v0 << v1 << v2 << v3 << v4 << v5 << v6 << v7 << v8 << v9;

    QVariantList tr_vals;

    QList< QPair<QString,const void*> > args;
    for( int i=0 ; i<vals.count() ; i++ )
    {
        if( i<m_args.count() )
        {
            QString type = m_args.at(i).first;

            if( type != QString::fromUtf8(vals.at(i).typeName()) )
            {
                if( !vals[i].canConvert( QVariant::nameToType(type.toLatin1()) ) )
                    ;
                else
                    vals[i].convert( QVariant::nameToType(type.toLatin1()) );
            }

            args << QPair<QString,const void*>( type, vals.at(i).data() );
            tr_vals << vals[i];
        }
        else
        {
            args << QPair<QString,const void*>( QString::fromUtf8(vals.at(i).typeName()) , vals.at(i).data() );
        }
    }

    int type = QMetaType::type(ret_type.toLatin1());
    void *res = QMetaType::create( type );
    bool is_pointer = ret_type.contains('*');

    bool done;
    switch( static_cast<int>(ctype) )
    {
    case Qt::QueuedConnection:
        done = QMetaObject::invokeMethod( obj , member.toLatin1() , Qt::QueuedConnection ,
                                  QGenericArgument( args.at(0).first.toLatin1() , args.at(0).second ) ,
                                  QGenericArgument( args.at(1).first.toLatin1() , args.at(1).second ) ,
                                  QGenericArgument( args.at(2).first.toLatin1() , args.at(2).second ) ,
                                  QGenericArgument( args.at(3).first.toLatin1() , args.at(3).second ) ,
                                  QGenericArgument( args.at(4).first.toLatin1() , args.at(4).second ) ,
                                  QGenericArgument( args.at(5).first.toLatin1() , args.at(5).second ) ,
                                  QGenericArgument( args.at(6).first.toLatin1() , args.at(6).second ) ,
                                  QGenericArgument( args.at(7).first.toLatin1() , args.at(7).second ) ,
                                  QGenericArgument( args.at(8).first.toLatin1() , args.at(8).second ) ,
                                  QGenericArgument( args.at(9).first.toLatin1() , args.at(9).second ) );
        return QVariant();
        break;

    default:
        done = QMetaObject::invokeMethod( obj , member.toLatin1() , ctype, QGenericReturnArgument( ret_type.toLatin1() , (is_pointer)? &res : res ) ,
                                  QGenericArgument( args.at(0).first.toLatin1() , args.at(0).second ) ,
                                  QGenericArgument( args.at(1).first.toLatin1() , args.at(1).second ) ,
                                  QGenericArgument( args.at(2).first.toLatin1() , args.at(2).second ) ,
                                  QGenericArgument( args.at(3).first.toLatin1() , args.at(3).second ) ,
                                  QGenericArgument( args.at(4).first.toLatin1() , args.at(4).second ) ,
                                  QGenericArgument( args.at(5).first.toLatin1() , args.at(5).second ) ,
                                  QGenericArgument( args.at(6).first.toLatin1() , args.at(6).second ) ,
                                  QGenericArgument( args.at(7).first.toLatin1() , args.at(7).second ) ,
                                  QGenericArgument( args.at(8).first.toLatin1() , args.at(8).second ) ,
                                  QGenericArgument( args.at(9).first.toLatin1() , args.at(9).second ) );
        break;
    }

    QVariant result;
    if( !done )
    {
        QMetaType::destroy(type, res);
        return result;
    }

    if( type == QMetaType::Void )
        result = QVariant();
    else
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    if( is_pointer )
        result = QVariant( type , &res );
    else
        result = QVariant( type , res );
#else
    if( is_pointer )
        result = QVariant( QMetaType(type) , &res );
    else
        result = QVariant( QMetaType(type) , res );
#endif

    if( type == QMetaType::type("QVariant") )
        return result.value<QVariant>();
    else
        return result;
}

Tools::~Tools()
{
    delete p;
}

}
