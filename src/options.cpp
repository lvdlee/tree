#include "options.hpp"

#include <iostream>
#include <string>

bool tree::options::show_help = false;
bool tree::options::all_files = false;
bool tree::options::directories_only = false;
std::vector<std::string> tree::options::directories = {"."};

void tree::options::parse(int argc, char* argv[]) {
    std::string arg1 = argv[1];

    int i = 1;

    if (arg1.substr(0, 2) == "--" && arg1 == "--help") {
        tree::options::show_help = true;
        i++;
    }

    if (arg1.substr(0, 2) != "--" && arg1.substr(0, 1) == "-") {
        if (arg1.find('a') != std::string::npos) {
            tree::options::all_files = true;
        }

        if (arg1.find('d') != std::string::npos) {
            tree::options::directories_only = true;
        }

        i++;
    }

    while (i < argc) {
        std::string arg2 = argv[i];

        if (arg2.substr(0, 2) == "--") {
        }

        if (arg2.substr(0, 2) != "--" && arg2.substr(0, 1) == "-") {
        }

        if (arg2.substr(0, 2) != "--" && arg2.substr(0, 1) != "-") {
            break;
        }

        i++;
    }

    if (i < argc) {
        tree::options::directories = {};
    }

    while (i < argc) {
        tree::options::directories.push_back(argv[i]);

        i++;
    }
}