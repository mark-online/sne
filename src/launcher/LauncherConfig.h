#pragma once

#include <sne/Common.h>

using namespace sne;

/**
 * @class LauncherConfig
 * 컨픽.
 */
class LauncherConfig
{
public:
    LauncherConfig();

    bool parseArgs(int argc, char* argv[]);

    const std::string& getDllName() const {
        return dllName_;
    }

    const std::string& getParameters() const {
        return parameters_;
    }

    bool isVerbose() const {
        return verbose_;
    }

private:
    std::string dllName_;
    std::string parameters_;
    bool verbose_;
};
