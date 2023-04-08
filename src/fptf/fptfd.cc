// Fool proof temp file daemon

#include <cstdlib>
#include <iostream>
#include <string_view>
#include <charconv>
#include <filesystem>
#include <fstream>

#include <signal.h>
#include <sys/prctl.h>

#define FPTFD_CHECK_CALL(expr) \
    if (const auto ret = (expr); ret < 0) { \
        std::cerr << "fptfd: " << #expr << " failed with error code " << ret << std::endl; \
        std::exit(EXIT_FAILURE); \
    }

namespace fptf::daemon
{
    struct Arguments {
        int ppid;
        std::filesystem::path tempfile;
    };

    void printUsage(std::string_view progname)
    {
        std::cerr << "Usage: " << progname << " <ppid> <tempfile>" << std::endl;
    }

    Arguments parseArguments(int argc, const char** argv)
    {
        const std::string_view progname = argv[0];

        if (argc != 3) {
            printUsage(progname);
            std::exit(EXIT_FAILURE);
        }

        const std::string_view ppidStr = argv[1];
        const std::string_view tempfileStr = argv[2];

        int ppid = -1;
        const auto [ptr, ec] = std::from_chars(ppidStr.data(), ppidStr.data() + ppidStr.size(), ppid);
        if (ec != std::errc() || ppid <= 0 || ptr != ppidStr.data() + ppidStr.size()) {
            printUsage(progname);
            std::exit(EXIT_FAILURE);
        }

        return {ppid, tempfileStr};
    }

    void removeTempfile(const std::filesystem::path& tempfile)
    {
        std::filesystem::remove(tempfile);
    }

    int main(int argc, const char** argv)
    {
        const auto [ppid, tempfile] = parseArguments(argc, argv);

        // Create signal set for SIGUSR1
        sigset_t signalSet;
        FPTFD_CHECK_CALL(sigemptyset(&signalSet));
        FPTFD_CHECK_CALL(sigaddset(&signalSet, SIGUSR1));

        // Block SIGUSR1
        FPTFD_CHECK_CALL(sigprocmask(SIG_BLOCK, &signalSet, nullptr));

        // Register SIGUSR1 on parent exit
        ::prctl(PR_SET_PDEATHSIG, SIGUSR1);

        // Next up we will wait for SIGUSR1,
        // but only if the parent process is still alive
        if (::getppid() != ppid) {
            removeTempfile(tempfile);
            return EXIT_SUCCESS;
        }

        // Wait for SIGUSR1
        int sig;
        FPTFD_CHECK_CALL(sigwait(&signalSet, &sig));

        // Remove tempfile
        removeTempfile(tempfile);
        return EXIT_SUCCESS;
    }
} // namespace fptf::daemon


int main(int argc, const char** argv)
{
    return fptf::daemon::main(argc, argv);
}