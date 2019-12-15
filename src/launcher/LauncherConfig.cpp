#include "LauncherPCH.h"
#include "LauncherConfig.h"
#include <boost/program_options.hpp>
#include <iostream>

LauncherConfig::LauncherConfig() :
    verbose_(false)
{
}


bool LauncherConfig::parseArgs(int argc, char* argv[])
{
    namespace opt = boost::program_options;

    opt::options_description generic("Generic options");
    generic.add_options()
        (",p", opt::value<std::string>(&parameters_), "Paramters to DLL module")
        ("verbose,v", "verbose (default: off)")
        ("help,h", "produce help message");

    opt::options_description hidden("Hidden options");
    hidden.add_options()
        ("input-file", opt::value<std::string>(), "input file");

    opt::options_description cmdlineOptions;
    cmdlineOptions.add(generic).add(hidden);

    opt::options_description visibleOptions;
    visibleOptions.add(generic);

    opt::positional_options_description positional;
    positional.add("input-file", -1);

    try {
        opt::variables_map vm;
        store(opt::command_line_parser(argc, argv).
            options(cmdlineOptions).positional(positional).run(), vm);
        opt::notify(vm);

        if (vm.count("verbose")) {
            verbose_ = true;
        }

        if (vm.count("input-file")) {
            dllName_ = vm["input-file"].as<std::string>();
        }

        if (vm.count("help") || dllName_.empty()) {
            std::cout << "SNE version: v" << SNE_VERSION_STRING << "\n";
            std::cout << visibleOptions << "\n";
            std::cout << "Usage:\n" <<
                "  sne.launcher <DLL name>\n";
            return false;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to parse command line(" << e.what() << ")!\n";
        return false;
    }

    return true;
}
