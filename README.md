# Fool Proof Temp Files

Fool Proof Temp Files, or fptf, is a small library that ensures
proper clean up of temporary files, even after an abnormal exit,
such as segmentation fault. It accomplishes this by spawning a
daemon that waits for the parent process to terminate.

## Usage

### C++

```cpp
#include <fptf/fptf.hh>

void createTempFile() {
    std::filesystem::path tempFile = ...
    fptf::registerTempFile(tempFile); // May throw
}
```

### C

```c
#include <fptf/fptf.h>

void createTempFile() {
    const char* tempfile = ...
    if (fptf_register_temp_file(tempFile) != 0) {
        // handle error
    }
}
```