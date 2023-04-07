#ifndef PURGESTRATEGY_H
#define PURGESTRATEGY_H
#include <QFileInfo>
#include <vector>
#include <QDateTime>

namespace PH {


class   PurgeStrategy
{
public:
    PurgeStrategy();
    virtual ~PurgeStrategy();

    virtual void purge(const QString& path) = 0;

protected:
    void list(const QString& path, std::vector<QFileInfo>& files);

private:
    PurgeStrategy(const PurgeStrategy&)=delete;
    PurgeStrategy& operator = (const PurgeStrategy&)=delete;
};


class  PurgeByAgeStrategy: public PurgeStrategy
{
public:
    PurgeByAgeStrategy(const qint64& age);
    ~PurgeByAgeStrategy();

    void purge(const QString& path);

private:
    qint64 _age;
};


class  PurgeByCountStrategy: public PurgeStrategy
{
public:
    PurgeByCountStrategy(std::size_t count);
    ~PurgeByCountStrategy();

    void purge(const QString& path);

private:
    std::size_t _count;
};


}

#endif // PURGESTRATEGY_H
