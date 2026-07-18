#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

namespace fs = std::filesystem;

struct Options
{
    bool modify_only = false;
    bool access_only = false;
};

void print_help()
{
    std::cout << "Usage: tch [OPTION]... FILE...\n"
              << "Update the access and modification times of each FILE to the current time.\n"
              << "Missing parent directories are created automatically.\n\n"
              << "  -a             change only the access time\n"
              << "  -m             change only the modification time\n"
              << "  --help         display this help and exit\n";
}

bool update_timestamps(const std::string &path_str, const Options &opts)
{

    struct timespec times[2];
    times[0].tv_nsec = UTIME_NOW;
    times[1].tv_nsec = UTIME_NOW;

    if (opts.modify_only && !opts.access_only)
    {
        times[0].tv_nsec = UTIME_OMIT;
    }
    else if (opts.access_only && !opts.modify_only)
    {
        times[1].tv_nsec = UTIME_OMIT;
    }

    if (utimensat(AT_FDCWD, path_str.c_str(), times, 0) != 0)
    {
        std::perror("tch: error updating timestamps");
        return false;
    }
    return true;
}

bool touch_path(const std::string &path_str, const Options &opts)
{
    try
    {
        fs::path target_path(path_str);

        if (target_path.has_parent_path())
        {
            fs::create_directories(target_path.parent_path());
        }

        std::ofstream file(target_path, std::ios::app);
        if (!file)
        {
            std::cerr << "tch: error: cannot open or create '" << path_str << "'\n";
            return false;
        }
        file.close();

        return update_timestamps(path_str, opts);
    }
    catch (const fs::filesystem_error &e)
    {
        std::cerr << "tch: filesystem error for '" << path_str << "': " << e.what() << "\n";
        return false;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        print_help();
        return 1;
    }

    Options opts;
    std::vector<std::string> file_paths;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];

        if (arg == "--help")
        {
            print_help();
            return 0;
        }
        else if (arg == "-m")
        {
            opts.modify_only = true;
        }
        else if (arg == "-a")
        {
            opts.access_only = true;
        }
        else if (arg == "-am" || arg == "-ma")
        {
            opts.access_only = true;
            opts.modify_only = true;
        }
        else if (arg[0] == '-' && arg.size() > 1)
        {
            std::cerr << "tch: invalid option -- '" << arg << "'\n"
                      << "Try 'tch --help' for more information.\n";
            return 1;
        }
        else
        {

            file_paths.push_back(arg);
        }
    }

    if (file_paths.empty())
    {
        std::cerr << "tch: missing file operand\n"
                  << "Try 'tch --help' for more information.\n";
        return 1;
    }

    bool success = true;
    for (const auto &path : file_paths)
    {
        if (!touch_path(path, opts))
        {
            success = false;
        }
    }

    return success ? 0 : 1;
}
