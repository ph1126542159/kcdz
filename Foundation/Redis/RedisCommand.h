#ifndef REDISCOMMAND_H
#define REDISCOMMAND_H
#include "RedisArray.h"
#include <QDateTime>
#include <vector>
#include <map>
namespace Redis {
class RedisCommand: public Array
{
public:
    using StringVec = std::vector<std::string>;
    RedisCommand(const std::string& command);
    /// Creates a command.

    RedisCommand(const RedisCommand& copy);
    /// Creates a command by copying another one.

    virtual ~RedisCommand();
    /// Destroys the command.

    static RedisCommand append(const std::string& key, const std::string& value);
    /// Creates and returns an APPEND command.

    static RedisCommand blpop(const StringVec& lists, qint64 timeout = 0);
    /// Creates and returns a BLPOP command.

    static RedisCommand brpop(const StringVec& lists, qint64 timeout = 0);
    /// Creates and returns a BRPOP command.

    static RedisCommand brpoplpush(const std::string& sourceList, const std::string& destinationList, qint64 timeout = 0);
    /// Creates and returns a BRPOPLPUSH command.

    static RedisCommand decr(const std::string& key, qint64 by = 0);
    /// Creates and returns an DECR or DECRBY command. Calls DECR when by is omitted or zero.

    static RedisCommand del(const std::string& key);
    /// Creates and returns an DEL command.

    static RedisCommand del(const StringVec& keys);
    /// Creates and returns an DEL command.

    static RedisCommand get(const std::string& key);
    /// Creates and returns an GET command.

    static RedisCommand exists(const std::string& key);
    /// Creates and returns an EXISTS command.

    static RedisCommand hdel(const std::string& hash, const std::string& field);
    /// Creates and returns an HDEL command.

    static RedisCommand hdel(const std::string& hash, const StringVec& fields);
    /// Creates and returns an HDEL command.

    static RedisCommand hexists(const std::string& hash, const std::string& field);
    /// Creates and returns an HEXISTS command.

    static RedisCommand hget(const std::string& hash, const std::string& field);
    /// Creates and returns an HGET command.

    static RedisCommand hgetall(const std::string& hash);
    /// Creates and returns an HGETALL command.

    static RedisCommand hincrby(const std::string& hash, const std::string& field, qint64 by = 1);
    /// Creates and returns an HINCRBY command.

    static RedisCommand hkeys(const std::string& hash);
    /// Creates and returns an HKEYS command.

    static RedisCommand hlen(const std::string& hash);
    /// Creates and returns an HLEN command.

    static RedisCommand hmget(const std::string& hash, const StringVec& fields);
    /// Creates and returns an HMGET command.

    static RedisCommand hmset(const std::string& hash, std::map<std::string, std::string>& fields);
    /// Creates and returns a HMSET command.

    static RedisCommand hset(const std::string& hash, const std::string& field, const std::string& value, bool create = true);
    /// Creates and returns an HSET or HSETNX (when create is false) command.

    static RedisCommand hset(const std::string& hash, const std::string& field, qint64 value, bool create = true);
    /// Creates and returns an HSET or HSETNX (when create is false) command.

    static RedisCommand hstrlen(const std::string& hash, const std::string& field);
    /// Creates and returns an HSTRLEN command (Available for Redis 3.2).

    static RedisCommand hvals(const std::string& hash);
    /// Creates and returns an HVALS command.

    static RedisCommand incr(const std::string& key, qint64 by = 0);
    /// Creates and returns an INCR or INCRBY command. Calls INCR when by is omitted or zero.

    static RedisCommand lindex(const std::string& list, qint64 index = 0);
    /// Creates and returns a LINDEX command.

    static RedisCommand linsert(const std::string& list, bool before, const std::string& reference, const std::string& value);
    /// Creates and returns a LINSERT command.

    static RedisCommand llen(const std::string& list);
    /// Creates and returns a LLEN command.

    static RedisCommand lpop(const std::string& list);
    /// Creates and returns a LPOP command.

    static RedisCommand lpush(const std::string& list, const std::string& value, bool create = true);
    /// Creates and returns a LPUSH or LPUSHX (when create is false) command.

    static RedisCommand lpush(const std::string& list, const StringVec& value, bool create = true);
    /// Creates and returns a LPUSH or LPUSHX (when create is false) command.

    static RedisCommand lrange(const std::string& list, qint64 start = 0, qint64 stop = -1);
    /// Creates and returns a LRANGE command. When start and stop is omitted an LRANGE
    /// command will returned that returns all items of the list.

    static RedisCommand lrem(const std::string& list, qint64 count, const std::string& value);
    /// Creates and returns a LREM command.

    static RedisCommand lset(const std::string& list, qint64 index, const std::string& value);
    /// Creates and returns a LSET command.

    static RedisCommand ltrim(const std::string& list, qint64 start = 0, qint64 stop = -1);
    /// Creates and returns a LTRIM command.

    static RedisCommand mget(const StringVec& keys);
    /// Creates and returns a MGET command.

    static RedisCommand mset(const std::map<std::string, std::string>& keyvalues, bool create = true);
    /// Creates and returns a MSET or MSETNX (when create is false) command.

    static RedisCommand sadd(const std::string& set, const std::string& value);
    /// Creates and returns a SADD command.

    static RedisCommand sadd(const std::string& set, const StringVec& values);
    /// Creates and returns a SADD command.

    static RedisCommand scard(const std::string& set);
    /// Creates and returns a SCARD command.

    static RedisCommand sdiff(const std::string& set1, const std::string& set2);
    /// Creates and returns a SDIFF command.Creates and returns

    static RedisCommand sdiff(const std::string& set, const StringVec& sets);
    /// Creates and returns a SDIFF command.

    static RedisCommand sdiffstore(const std::string& set, const std::string& set1, const std::string& set2);
    /// Creates and returns a SDIFFSTORE command.

    static RedisCommand sdiffstore(const std::string& set, const StringVec& sets);
    /// Creates and returns a SDIFFSTORE command.

    static RedisCommand set(const std::string& key, const std::string& value, bool overwrite = true, const qint64& expireTime = 0/*微秒*/, bool create = true);
    /// Creates and returns a SET command to set the key with a value.

    static RedisCommand set(const std::string& key, qint64 value, bool overwrite = true, const qint64& expireTime = 0/*微秒*/, bool create = true);
    /// Creates and returns a SET command to set the key with a value.

    static RedisCommand sinter(const std::string& set1, const std::string& set2);
    /// Creates and returns a SINTER command.

    static RedisCommand sinter(const std::string& set, const StringVec& sets);
    /// Creates and returns a SINTER command.

    static RedisCommand sinterstore(const std::string& set, const std::string& set1, const std::string& set2);
    /// Creates and returns a SINTERSTORE command.

    static RedisCommand sinterstore(const std::string& set, const StringVec& sets);
    /// Creates and returns a SINTERSTORE command.

    static RedisCommand sismember(const std::string& set, const std::string& member);
    /// Creates and returns a SISMEMBER command.

    static RedisCommand smembers(const std::string& set);
    /// Creates and returns a SMEMBERS command.

    static RedisCommand smove(const std::string& source, const std::string& destination, const std::string& member);
    /// Creates and returns a SMOVE command.

    static RedisCommand spop(const std::string& set, qint64 count = 0);
    /// Creates and returns a SPOP command.

    static RedisCommand srandmember(const std::string& set, qint64 count = 0);
    /// Creates and returns a SRANDMEMBER command.

    static RedisCommand srem(const std::string& set, const std::string& member);
    /// Creates and returns a SREM command.

    static RedisCommand srem(const std::string& set, const StringVec& members);
    /// Creates and returns a SREM command.

    static RedisCommand sunion(const std::string& set1, const std::string& set2);
    /// Creates and returns a SUNION command.

    static RedisCommand sunion(const std::string& set, const StringVec& sets);
    /// Creates and returns a SUNION command.

    static RedisCommand sunionstore(const std::string& set, const std::string& set1, const std::string& set2);
    /// Creates and returns a SUNIONSTORE command.

    static RedisCommand sunionstore(const std::string& set, const StringVec& sets);
    /// Creates and returns a SUNIONSTORE command.

    static RedisCommand rename(const std::string& key, const std::string& newName, bool overwrite = true);
    /// Creates and returns a RENAME or RENAMENX when overwrite is false.

    static RedisCommand rpop(const std::string& list);
    /// Creates and returns a RPOP command.

    static RedisCommand rpoplpush(const std::string& sourceList, const std::string& destinationList);
    /// Creates and returns a RPOPLPUSH command.

    static RedisCommand rpush(const std::string& list, const std::string& value, bool create = true);
    /// Creates and returns a RPUSH or RPUSHX (when create is false) command.

    static RedisCommand rpush(const std::string& list, const StringVec& value, bool create = true);
    /// Creates and returns a RPUSH or RPUSHX (when create is false) command.

    static RedisCommand expire(const std::string& key, qint64 seconds);
    /// Creates and returns an EXPIRE command.

    static RedisCommand ping();
    /// Creates and returns a PING command.

    static RedisCommand multi();
    /// Creates and returns a MULTI command.

    static RedisCommand exec();
    /// Creates and returns a EXEC command.

    static RedisCommand discard();
    /// Creates and returns a DISCARD command.
};
}


#endif // REDISCOMMAND_H
