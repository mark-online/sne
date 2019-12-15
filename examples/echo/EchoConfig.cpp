#include "EchoTestPCH.h"
#include "EchoConfig.h"
#include <boost/program_options.hpp>
#include <iostream>

enum {
    defaultPort = 34567
};

EchoConfig::EchoConfig() :
    mode_("both"),
    connectHost_("127.0.0.1"),
    port_(defaultPort),
    threads_(1),
    connections_(10),
    echoCount_(100),
    blockSize_(128),
    useRpc_(false),
    verbose_(false)
{
}


bool EchoConfig::parseArgs(int argc, char* argv[])
{
    namespace opt = boost::program_options;

    opt::options_description generic("Generic options");
    generic.add_options()
        (",m", opt::value<std::string>(&mode_)->default_value("both"),
            "[client|server|both]")
        (",p", opt::value<uint16_t>(&port_)->default_value(defaultPort),
            "port to connect/listen")
        (",t", opt::value<uint16_t>(&threads_)->default_value(1),
            "number of threads in thread pool")
        ("rpc,r", "use RPC (default: off)")
        ("verbose,v", "verbose (default: off)")
        ("help,h", "produce help message");

    opt::options_description client("Client options");
    client.add_options()
        (",a", opt::value<std::string>(&connectHost_)->default_value("127.0.0.1"),
            "host to connect")
        (",c", opt::value<uint16_t>(&connections_)->default_value(10),
            "connection count")
        (",e", opt::value<uint32_t>(&echoCount_)->default_value(100),
            "echo count")
        (",k", opt::value<uint16_t>(&blockSize_)->default_value(128),
            "data block size");

    opt::options_description cmdline_options;
    cmdline_options.add(generic).add(client);

    try {
        opt::variables_map vm;
        opt::store(opt::parse_command_line(argc, argv, cmdline_options), vm);
        opt::notify(vm);

        if (vm.count("help")) {
            std::cout << "SNE version: v" << SNE_VERSION_STRING << "\n";
            std::cout << cmdline_options << "\n";
            std::cout << "Usage:\n" <<
                "  server only: EchoTest -m server\n" <<
                "  client only: EchoTest -m client -n 2\n" <<
                "  server/client: EchoTest -e 2000 -k 512\n";
            return false;
        }
        else if (vm.count("verbose")) {
            verbose_ = true;
        }
        else if (vm.count("rpc")) {
            useRpc_ = true;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Failed to parse command line(" << e.what() << ")!\n";
        return false;
    }

    bool isOk = true;
    if ((! isServer()) && (! isClient())) {
        isOk = false;
    }

    return isOk;
}
