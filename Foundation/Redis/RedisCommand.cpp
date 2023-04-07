#include "RedisCommand.h"

namespace Redis {
RedisCommand::RedisCommand(const std::string& command): Array()
{
    add(command);
}


RedisCommand::RedisCommand(const RedisCommand& copy): Array(copy)
{
}


RedisCommand::~RedisCommand()
{
}


RedisCommand RedisCommand::append(const std::string& key, const std::string& value)
{
    RedisCommand cmd("APPEND");

    cmd << key << value;

    return cmd;
}


RedisCommand RedisCommand::blpop(const StringVec& lists, qint64 timeout)
{
    RedisCommand cmd("BLPOP");

    cmd << lists << QString::number(timeout).toStdString();

    return cmd;
}


RedisCommand RedisCommand::brpop(const StringVec& lists, qint64 timeout)
{
    RedisCommand cmd("BRPOP");

    cmd << lists << QString::number(timeout).toStdString();

    return cmd;
}


RedisCommand RedisCommand::brpoplpush(const std::string& sourceList, const std::string& destinationList, qint64 timeout)
{
    RedisCommand cmd("BRPOPLPUSH");

    cmd << sourceList << destinationList << QString::number(timeout).toStdString();

    return cmd;
}


RedisCommand RedisCommand::decr(const std::string& key, qint64 by)
{
    RedisCommand cmd(by == 0 ? "DECR" : "DECRBY");

    cmd << key;
    if ( by > 0 ) cmd << QString::number(by).toStdString();

    return cmd;
}


RedisCommand RedisCommand::del(const std::string& key)
{
    RedisCommand cmd("DEL");

    cmd << key;

    return cmd;
}


RedisCommand RedisCommand::del(const StringVec& keys)
{
    RedisCommand cmd("DEL");

    cmd << keys;

    return cmd;
}


RedisCommand RedisCommand::get(const std::string& key)
{
    RedisCommand cmd("GET");

    cmd << key;

    return cmd;
}


RedisCommand RedisCommand::exists(const std::string& key)
{
    RedisCommand cmd("EXISTS");

    cmd << key;

    return cmd;
}


RedisCommand RedisCommand::hdel(const std::string& hash, const std::string& field)
{
    RedisCommand cmd("HDEL");

    cmd << hash << field;

    return cmd;
}


RedisCommand RedisCommand::hdel(const std::string& hash, const StringVec& fields)
{
    RedisCommand cmd("HDEL");

    cmd << hash << fields;

    return cmd;
}


RedisCommand RedisCommand::hexists(const std::string& hash, const std::string& field)
{
    RedisCommand cmd("HEXISTS");

    cmd << hash << field;

    return cmd;
}


RedisCommand RedisCommand::hget(const std::string& hash, const std::string& field)
{
    RedisCommand cmd("HGET");

    cmd << hash << field;

    return cmd;
}


RedisCommand RedisCommand::hgetall(const std::string& hash)
{
    RedisCommand cmd("HGETALL");

    cmd << hash;

    return cmd;
}


RedisCommand RedisCommand::hincrby(const std::string& hash, const std::string& field, qint64 by)
{
    RedisCommand cmd("HINCRBY");

    cmd << hash << field << QString::number(by).toStdString();

    return cmd;
}


RedisCommand RedisCommand::hkeys(const std::string& hash)
{
    RedisCommand cmd("HKEYS");

    cmd << hash;

    return cmd;
}


RedisCommand RedisCommand::hlen(const std::string& hash)
{
    RedisCommand cmd("HLEN");

    cmd << hash;

    return cmd;
}


RedisCommand RedisCommand::hmget(const std::string& hash, const StringVec& fields)
{
    RedisCommand cmd("HMGET");

    cmd << hash << fields;

    return cmd;
}


RedisCommand RedisCommand::hmset(const std::string& hash, std::map<std::string, std::string>& fields)
{
    RedisCommand cmd("HMSET");

    cmd << hash;
    for(std::map<std::string, std::string>::const_iterator it = fields.begin(); it != fields.end(); ++it)
    {
        cmd << it->first << it->second;
    }

    return cmd;
}


RedisCommand RedisCommand::hset(const std::string& hash, const std::string& field, const std::string& value, bool create)
{
    RedisCommand cmd(create ? "HSET" : "HSETNX");

    cmd << hash << field << value;

    return cmd;
}


RedisCommand RedisCommand::hset(const std::string& hash, const std::string& field, qint64 value, bool create)
{
    return hset(hash, field, QString::number(value).toStdString(), create);
}


RedisCommand RedisCommand::hstrlen(const std::string& hash, const std::string& field)
{
    RedisCommand cmd("HSTRLEN");

    cmd << hash << field;

    return cmd;
}


RedisCommand RedisCommand::hvals(const std::string& hash)
{
    RedisCommand cmd("HVALS");

    cmd << hash;

    return cmd;
}


RedisCommand RedisCommand::incr(const std::string& key, qint64 by)
{
    RedisCommand cmd(by == 0 ? "INCR" : "INCRBY");

    cmd << key;
    if ( by > 0 ) cmd << QString::number(by).toStdString();

    return cmd;
}


RedisCommand RedisCommand::lindex(const std::string& list, qint64 index)
{
    RedisCommand cmd("LINDEX");

    cmd << list << QString::number(index).toStdString();

    return cmd;
}


RedisCommand RedisCommand::linsert(const std::string& list, bool before, const std::string& reference, const std::string& value)
{
    RedisCommand cmd("LINSERT");

    cmd << list << (before ? "BEFORE" : "AFTER") << reference << value;
    return cmd;
}


RedisCommand RedisCommand::llen(const std::string& list)
{
    RedisCommand cmd("LLEN");

    cmd << list;

    return cmd;
}


RedisCommand RedisCommand::lpop(const std::string& list)
{
    RedisCommand cmd("LPOP");

    cmd << list;

    return cmd;
}


RedisCommand RedisCommand::lpush(const std::string& list, const std::string& value, bool create)
{
    RedisCommand cmd(create ? "LPUSH" : "LPUSHX");

    cmd << list << value;

    return cmd;
}


RedisCommand RedisCommand::lpush(const std::string& list, const StringVec& values, bool create)
{
    RedisCommand cmd(create ? "LPUSH" : "LPUSHX");

    cmd << list << values;

    return cmd;
}


RedisCommand RedisCommand::lrange(const std::string& list, qint64 start, qint64 stop)
{
    RedisCommand cmd("LRANGE");

    cmd << list << QString::number(start).toStdString() << QString::number(stop).toStdString();

    return cmd;
}


RedisCommand RedisCommand::lrem(const std::string& list, qint64 count, const std::string& value)
{
    RedisCommand cmd("LREM");

    cmd << list << QString::number(count).toStdString() << value;

    return cmd;
}


RedisCommand RedisCommand::lset(const std::string& list, qint64 index, const std::string& value)
{
    RedisCommand cmd("LSET");

    cmd << list << QString::number(index).toStdString() << value;

    return cmd;
}


RedisCommand RedisCommand::ltrim(const std::string& list, qint64 start, qint64 stop)
{
    RedisCommand cmd("LTRIM");

    cmd << list << QString::number(start).toStdString() << QString::number(stop).toStdString();

    return cmd;
}


RedisCommand RedisCommand::mget(const StringVec& keys)
{
    RedisCommand cmd("MGET");

    cmd << keys;

    return cmd;
}


RedisCommand RedisCommand::mset(const std::map<std::string, std::string>& keyvalues, bool create)
{
    RedisCommand cmd(create ? "MSET" : "MSETNX");

    for(std::map<std::string, std::string>::const_iterator it = keyvalues.begin(); it != keyvalues.end(); ++it)
    {
        cmd << it->first << it->second;
    }

    return cmd;
}


RedisCommand RedisCommand::sadd(const std::string& set, const std::string& value)
{
    RedisCommand cmd("SADD");

    cmd << set << value;

    return cmd;
}


RedisCommand RedisCommand::sadd(const std::string& set, const StringVec& values)
{
    RedisCommand cmd("SADD");

    cmd << set << values;

    return cmd;
}


RedisCommand RedisCommand::scard(const std::string& set)
{
    RedisCommand cmd("SCARD");

    cmd << set;

    return cmd;
}


RedisCommand RedisCommand::sdiff(const std::string& set1, const std::string& set2)
{
    RedisCommand cmd("SDIFF");

    cmd << set1 << set2;

    return cmd;
}


RedisCommand RedisCommand::sdiff(const std::string& set, const StringVec& sets)
{
    RedisCommand cmd("SDIFF");

    cmd << set << sets;

    return cmd;
}


RedisCommand RedisCommand::sdiffstore(const std::string& set, const std::string& set1, const std::string& set2)
{
    RedisCommand cmd("SDIFFSTORE");

    cmd << set << set1 << set2;

    return cmd;
}


RedisCommand RedisCommand::sdiffstore(const std::string& set, const StringVec& sets)
{
    RedisCommand cmd("SDIFFSTORE");

    cmd << set << sets;

    return cmd;
}


RedisCommand RedisCommand::set(const std::string& key, const std::string& value, bool overwrite, const qint64& expireTime, bool create)
{
    RedisCommand cmd("SET");

    cmd << key << value;
    if (! overwrite) cmd << "NX";
    if (! create) cmd << "XX";
    if (expireTime > 0) cmd << "PX" << QString::number(expireTime/1000).toStdString();

    return cmd;
}


RedisCommand RedisCommand::set(const std::string& key, qint64 value, bool overwrite, const qint64& expireTime, bool create)
{
    return set(key, QString::number(value).toStdString(), overwrite, expireTime, create);
}


RedisCommand RedisCommand::sinter(const std::string& set1, const std::string& set2)
{
    RedisCommand cmd("SINTER");

    cmd << set1 << set2;

    return cmd;
}


RedisCommand RedisCommand::sinter(const std::string& set, const StringVec& sets)
{
    RedisCommand cmd("SINTER");

    cmd << set << sets;

    return cmd;
}


RedisCommand RedisCommand::sinterstore(const std::string& set, const std::string& set1, const std::string& set2)
{
    RedisCommand cmd("SINTERSTORE");

    cmd << set << set1 << set2;

    return cmd;
}


RedisCommand RedisCommand::sinterstore(const std::string& set, const StringVec& sets)
{
    RedisCommand cmd("SINTERSTORE");

    cmd << set << sets;

    return cmd;
}


RedisCommand RedisCommand::sismember(const std::string& set, const std::string& member)
{
    RedisCommand cmd("SISMEMBER");

    cmd << set << member;

    return cmd;
}


RedisCommand RedisCommand::smembers(const std::string& set)
{
    RedisCommand cmd("SMEMBERS");

    cmd << set;

    return cmd;
}


RedisCommand RedisCommand::smove(const std::string& source, const std::string& destination, const std::string& member)
{
    RedisCommand cmd("SMOVE");

    cmd << source  << destination << member;

    return cmd;
}


RedisCommand RedisCommand::spop(const std::string& set, qint64 count)
{
    RedisCommand cmd("SPOP");

    cmd << set;
    if( count != 0 ) cmd << QString::number(count).toStdString();

    return cmd;
}


RedisCommand RedisCommand::srandmember(const std::string& set, qint64 count)
{
    RedisCommand cmd("SRANDMEMBER");

    cmd << set;
    if( count != 0 ) cmd << QString::number(count).toStdString();

    return cmd;
}


RedisCommand RedisCommand::srem(const std::string& set1, const std::string& member)
{
    RedisCommand cmd("SREM");

    cmd << set1 << member;

    return cmd;
}


RedisCommand RedisCommand::srem(const std::string& set, const StringVec& members)
{
    RedisCommand cmd("SREM");

    cmd << set << members;

    return cmd;
}


RedisCommand RedisCommand::sunion(const std::string& set1, const std::string& set2)
{
    RedisCommand cmd("SUNION");

    cmd << set1 << set2;

    return cmd;
}


RedisCommand RedisCommand::sunion(const std::string& set, const StringVec& sets)
{
    RedisCommand cmd("SUNION");

    cmd << set << sets;

    return cmd;
}


RedisCommand RedisCommand::sunionstore(const std::string& set, const std::string& set1, const std::string& set2)
{
    RedisCommand cmd("SUNIONSTORE");

    cmd << set << set1 << set2;

    return cmd;
}


RedisCommand RedisCommand::sunionstore(const std::string& set, const StringVec& sets)
{
    RedisCommand cmd("SUNIONSTORE");

    cmd << set << sets;

    return cmd;
}


RedisCommand RedisCommand::rename(const std::string& key, const std::string& newName, bool overwrite)
{
    RedisCommand cmd(overwrite ? "RENAME" : "RENAMENX");

    cmd << key << newName;

    return cmd;
}


RedisCommand RedisCommand::rpop(const std::string& list)
{
    RedisCommand cmd("RPOP");

    cmd << list;

    return cmd;
}


RedisCommand RedisCommand::rpoplpush(const std::string& sourceList, const std::string& destinationList)
{
    RedisCommand cmd("RPOPLPUSH");

    cmd << sourceList << destinationList;

    return cmd;
}


RedisCommand RedisCommand::rpush(const std::string& list, const std::string& value, bool create)
{
    RedisCommand cmd(create ? "RPUSH" : "RPUSHX");

    cmd << list << value;

    return cmd;
}


RedisCommand RedisCommand::rpush(const std::string& list, const StringVec& values, bool create)
{
    RedisCommand cmd(create ? "RPUSH" : "RPUSHX");

    cmd << list << values;

    return cmd;
}


RedisCommand RedisCommand::expire(const std::string& key, qint64 seconds)
{
    RedisCommand cmd("EXPIRE");

    cmd << key << QString::number(seconds).toStdString();

    return cmd;
}


RedisCommand RedisCommand::ping()
{
    RedisCommand cmd("PING");

    return cmd;
}


RedisCommand RedisCommand::multi()
{
    RedisCommand cmd("MULTI");

    return cmd;
}


RedisCommand RedisCommand::exec()
{
    RedisCommand cmd("EXEC");

    return cmd;
}


RedisCommand RedisCommand::discard()
{
    RedisCommand cmd("DISCARD");

    return cmd;
}
}
