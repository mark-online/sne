#include "LauncherPCH.h"
#include "LauncherConfig.h"
#include <sne/server/app/Factory.h>
#include <sne/base/utility/LogManager.h>
#include <sne/core/utility/Win32Util.h>
#include <iostream>

using namespace sne;

int main(int argc, char* argv[])
{
#ifdef _WIN32
    sne::core::enableLowFragmentationHeap();
    sne::core::InitOle initOle;
#endif

    LauncherConfig config;
    if (!config.parseArgs(argc, argv)) {
        return -1;
    }

    base::LogManager logManager;
    logManager.enableConsoleLog();
    if (config.isVerbose()) {
        logManager.verbose();
    }

    try {
        auto creator = server::createAppFactory(config.getDllName());

        auto sharedObject = creator();
        if (sharedObject) {
            if (!sharedObject->initialize(config.getParameters())) {
                return EXIT_FAILURE;
            }

            // TODO: wait
            //sharedObject->finalize();
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
