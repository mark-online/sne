#include "EchoTestPCH.h"
#include "EchoConfig.h"
#include "EchoServer.h"
#include "EchoClient.h"
#include "sne/server/utility/Profiler.h"
#include <sne/base/io_context/IoContextAcceptor.h>
#include <sne/base/io_context/IoContextConnector.h>
#include <sne/base/io_context/IoContextTask.h>
#include <sne/base/io_context/IoContext.h>
#include <sne/base/session/manager/CachedSessionManager.h>
#include <sne/base/utility/SystemUtil.h>
#include <sne/base/utility/Logger.h>
#include <sne/core/utility/SystemUtil.h>
#include <sne/base/utility/LogManager.h>
#include <iostream>
#include <chrono>

using namespace sne;

void printArgs(int argc, char* argv[])
{
    std::cout << "Command line: ";
    for (int i = 1; i < argc; ++i) {
        std::cout << argv[i] << " ";
    }
    std::cout << "\n";
}


// Let's go!
void run(const EchoConfig& config)
{
    base::IoContextTask ioContextTask("echo(io_context)");
    if (! ioContextTask.start(config.getThreads())) {
        SNE_LOG_ERROR("Failed to create IO service task.");
        return;
    }

    size_t poolSize = 5;
    if (config.getConnections() > poolSize) {
        poolSize = config.getConnections();
    }

    // server setup
    ServerSessionFactory serverSessionFactory(config, ioContextTask);
    base::CachedSessionManager serverSessionManager("ServerSessionManager", poolSize,
            serverSessionFactory);
    serverSessionManager.initialize();

    base::IoContextAcceptor acceptor(ioContextTask,
        serverSessionManager, serverSessionManager);

    // client setup
    ClientSessionFactory clientSessionFactory(config, ioContextTask);
    base::CachedSessionManager clientSessionManager("ClientSessionManager", poolSize,
            clientSessionFactory);
    clientSessionManager.initialize();

    base::IoContextConnector connector(ioContextTask,
        clientSessionManager, clientSessionManager);

    if (config.isServer()) {
        if (!acceptor.open(base::InetAddress(config.getPort()))) {
            return;
        }
        std::cout << "Running Server at " << "*:" <<
            config.getPort() << "...\n";
    }

    if (config.isClient()) {
        const base::InetAddress addr(config.getPort(), config.getConnectHost());
        connector.open(addr, config.getConnections());

        // 모든 클라이언트가 접속할 때까지 대기한다
        using namespace std::chrono;
        const msec_t timeout = config.getConnections() * 100;
        const auto startTime = std::chrono::steady_clock::now();
        while (clientSessionManager.getSessionCount() < config.getConnections()) {
            if (sne::core::getElapsedTime(startTime) >= timeout) {
                break;
            }
            base::pause(1);
        }
    }

    // running
    if (config.isClient()) {
        const size_t sessionCount = clientSessionManager.getSessionCount();
        if (config.getConnections() == sessionCount) {
            SNE_LOG_INFO("All client are connected(%d)",
                config.getConnections());
        }
        else {
            SNE_LOG_ERROR("Failed to connect to server(%d != %d)",
                config.getConnections(), sessionCount);
        }

        sne::server::BaseProfiler<0> profiler("Echo");
        clientSessionManager.waitForDone();
    }
    else if (config.isServer()) { // server only
        std::cout << "Press Ctrl-C to exit...\n";
        for (;;) {
            base::pause(10);
        }
    }

    // finishing
    connector.close();

    acceptor.close();

    serverSessionManager.cancel();
    serverSessionManager.waitForDone();

    ioContextTask.stop();
    ioContextTask.waitForDone();
}


int main(int argc, char* argv[])
{
    printArgs(argc, argv);

    EchoConfig config;
    if (! config.parseArgs(argc, argv)) {
        return -1;
    }

    base::LogManager logManager;
    logManager.enableConsoleLog();
    if (config.isVerbose()) {
        logManager.verbose();
    }
    else {
        logManager.silent();
        //logManager.noDebug();
    }
    //logManager.enableFileLog("log/EchoTest_%5N.log", 1024);

    run(config);

    return 0;
}
