
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
#include <sys/mman.h>
#include <linux/limits.h>
#include <cstring>

namespace fptf::detail
{
    // unsigned char fptfd[];
    // unsigned int fptfd_len;
    #include "fptfd_binary.hh"

    int loadDaemon()
    {
        // Create a memory file descriptor.
        const int memfd = memfd_create("fptfd", MFD_CLOEXEC);
        if (memfd < 0)
        {
            return -1;
        }

        // Write fptfd to the memory file descriptor.
        if (write(memfd, fptfd, fptfd_len) != fptfd_len)
        {
            return -1;
        }

        return memfd;
    }

    struct DaemonArgs
    {
        char procname[6] = "fptfd";
        char ppid[32];
        char filepath[PATH_MAX];

        DaemonArgs(int ppid, const char* filepath)
        {
            snprintf(this->ppid, sizeof(this->ppid), "%d", ppid);
            strncpy(this->filepath, filepath, sizeof(this->filepath));
        }

    };

    void registerTempFile(const char* filepath)
    {
        // Need to perform allocating operations before forking as
        // it is not safe to call non-reentrant functions in child.
        // const std::filesystem::path& daemonPath = getDaemonPath();
        const int ppid = getpid();
        DaemonArgs args{ppid, filepath};

        const int pid = fork();
        if (pid == 0)
        {
            // Child process

            static int daemonFd = loadDaemon();
            char* const argv[] = { args.procname, args.ppid, args.filepath, nullptr };
            char* const envp[] = { nullptr };
            fexecve(daemonFd, argv, envp);

            // fexecve does not return on success.
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




