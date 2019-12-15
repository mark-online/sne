#pragma once

#include <sne/Common.h>

using namespace sne;

/**
 * @class EchoConfig
 * 컨픽.
 */
class EchoConfig
{
public:
    EchoConfig();

    bool parseArgs(int argc, char* argv[]);

    bool isServer() const {
        return mode_ == "server" || mode_ == "both";
    }
    bool isClient() const {
        return mode_ == "client" || mode_ == "both";
    }
    bool isBoth() const {
        return mode_ == "both";
    }
    std::string getConnectHost() const {
        return connectHost_;
    }
    uint16_t getPort() const {
        return port_;
    }
    uint32_t getThreads() const {
        return threads_;
    }
    uint32_t getConnections() const {
        return connections_;
    }
    uint32_t getEchoCount() const {
        return echoCount_;
    }
    uint32_t getBlockSize() const {
        return blockSize_;
    }
    bool useRpc() const {
        return useRpc_;
    }
    std::string getCipher() const {
        return "none"; //"aes";
    }
    bool isVerbose() const {
        return verbose_;
    }

private:
    std::string mode_;
    std::string connectHost_;
    uint16_t port_;
    uint16_t threads_;
    uint16_t connections_;
    uint32_t echoCount_;
    uint16_t blockSize_;
    bool useRpc_;
    bool verbose_;
};
