#include "RedisClient.h"

namespace Redis {

RedisClient::RedisClient():
    _port(0),
    _address(),
    _socket(),
    _input(nullptr),
    _output(nullptr),
    _isConnected(false)
{

}

bool RedisClient::connect(int msecs)
{
    if(nullptr!=_input){
        disconnect();
    }
    _socket.connectToHost(_address,_port);
    _isConnected=_socket.waitForConnected(msecs);
    if(_isConnected){
        _input = new RedisInputStream(_socket);
        _output = new RedisOutputStream(_socket);
    }
    return _isConnected;
}

void RedisClient::writeCommand(const Array &command, bool flush)
{
    assert(nullptr!=_output);

    std::string commandStr = command.toString();

    _output->write(commandStr.c_str(), commandStr.length());
    if (flush) _output->flush();
}

RedisClient::~RedisClient()
{
    disconnect();
}

bool RedisClient::connect(const std::string &host, unsigned short port,int msecs)
{
    return connect(QHostAddress(host.c_str()),port,msecs);
}

bool RedisClient::connect(const QHostAddress &addrs, unsigned short port,int msecs)
{
    _address=addrs;
    _port=port;
    return connect(msecs);
}

void RedisClient::disconnect()
{
    if(_input){
        delete _input;
        _input = nullptr;
    }

    if(_output){
        delete _output;
        _output = nullptr;
    }
    _socket.abort();
    _socket.close();
    _isConnected=false;
}

bool RedisClient::isConnected() const
{
    return _isConnected;
}

RedisType::Ptr RedisClient::sendCommand(const Array &command)
{
    writeCommand(command, true);
    return readReply();
}

RedisType::Ptr RedisClient::readReply()
{
    assert(nullptr!=_input);

    int c = _input->get();
    if (c == -1)
    {
        disconnect();
        throw Exception("Lost connection to Redis server");
    }
    RedisType::Ptr result = RedisType::createRedisType(c);
    if (nullptr==result.get())
    {
        throw Exception("Invalid Redis type returned");
    }

    result->read(*_input);

    return result;
}

Array RedisClient::sendCommands(const std::vector<Array> &commands)
{
    Array results;

    for (std::vector<Array>::const_iterator it = commands.begin(); it != commands.end(); ++it)
    {
        writeCommand(*it, false);
    }
    _output->flush();

    for (int i = 0; i < commands.size(); ++i)
    {
        results.addRedisType(readReply());
    }

    return results;
}

}
