
// Implementation forward declaration
namespace fptf::detail
{
    void registerTempFile(const char* filepath);
} // namespace fptf::detail

// C API
#include <fptf/fptf.h>

int fptf_register_signal_guard(const char* filepath)
{
    if (filepath == nullptr)
    {
        return 1;
    }
    try {
        fptf::detail::registerTempFile(filepath);
    } catch (...) {
        return 1;
    }
    return 0;
}

// C++ API
#include <fptf/fptf.hh>

namespace fptf
{
    void registerTempFile(const std::filesystem::path& filepath)
    {
        detail::registerTempFile(filepath.c_str());
    }

    void registerTempFile(const std::string& filepath)
    {
        detail::registerTempFile(filepath.c_str());
    }

    void registerTempFile(const char* filepath)
    {
        detail::registerTempFile(filepath);
    }
} // namespace fptf

// Implementation

#include <filesystem>
#include <string>
#include <unistd.h>
#include <dlfcn.h>

namespace fptf::detail
{
    std::string
    getLibraryDirectoryPath()
    {
        Dl_info info;
        if (dladdr(reinterpret_cast<void*>(fptf_register_signal_guard), &info) == 0)
        {
            throw std::runtime_error("fptf::registerSignalGuard failed to get library path");
        }
        return std::filesystem::path(info.dli_fname).parent_path();
    }

    const std::filesystem::path&
    getDaemonPath()
    {
        const static std::filesystem::path daemonPath = [] {
            Dl_info info;
            if (dladdr(reinterpret_cast<void*>(fptf_register_signal_guard), &info) == 0)
            {
                throw std::runtime_error("fptf::registerSignalGuard failed to get library path");
            }
            const std::filesystem::path libDir = std::filesystem::path(info.dli_fname).parent_path();
            const std::filesystem::path daemonPath = libDir / "fptfd";
            if (!std::filesystem::exists(daemonPath))
            {
                throw std::runtime_error("fptf::registerSignalGuard failed to find daemon");
            }
            return daemonPath;
        }();
        return daemonPath;
    }

    void registerTempFile(const char* filepath)
    {
        // Need to perform allocating operations before forking as
        // it is not safe to call non-reentrant functions in child.
        const std::filesystem::path& daemonPath = getDaemonPath();
        const int ppid = getpid();
        const std::string ppidString = std::to_string(ppid);

        const int pid = fork();
        if (pid == 0)
        {
            // Child process
            execl(daemonPath.c_str(), daemonPath.c_str(), ppidString.c_str(), filepath, nullptr);

            // execl does not return on success.
            // It is not safe to use std::cerr here, so we use perror instead.
            perror("fptf::registerSignalGuard failed to launch daemon");

            // Use _exit to avoid calling atexit handlers from parrent process.
            _exit(EXIT_FAILURE);
        }
        else if (pid < 0)
        {
            throw std::runtime_error("fptf::registerSignalGuard failed to fork");
        }
    }
} // namespace fptf::detail




