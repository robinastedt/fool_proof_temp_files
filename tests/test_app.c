// Mocked application used by tests.
// Registers a tempfile with fptf C API and exits.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <fptf/fptf.h>

int main(int argc, const char** argv)
{
    if (argc != 3)
    {
        printf("Usage: %s <tempfile> <quick exit>", argv[0]);
        return EXIT_FAILURE;
    }

    const char* const tempfile = argv[1];
    const char* const quickexit = argv[2];

    if (fptf_register_temp_file(tempfile) != 0) {
        printf("Failed to register temp file %s", tempfile);
        return EXIT_FAILURE;
    }

    printf("Registered temp file %s", tempfile);

    if (strcmp(quickexit, "true") == 0)
    {
        printf("Quick exit");
    }
    else
    {
        printf("Slow exit");
        usleep(100000);
    }

    return EXIT_SUCCESS;
}