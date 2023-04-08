// Mocked application used by tests.
// Registers a tempfile with fptf C++ API and exits.

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <thread>

#include <fptf/fptf.hh>

int main(int argc, const char** argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <tempfile> <quick exit>" << std::endl;
        return EXIT_FAILURE;
    }

    const std::string tempfile = argv[1];
    const std::string quickexit = argv[2];

    fptf::registerTempFile(tempfile);
    std::cout << "Registered temp file " << tempfile << std::endl;

    if (quickexit == "true")
    {
        std::cout << "Quick exit" << std::endl;
    }
    else
    {
        std::cout << "Slow exit" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return EXIT_SUCCESS;
}