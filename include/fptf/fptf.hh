#pragma once

#include <filesystem>
#include <string>

namespace fptf
{
    /**
     * @brief Register a temporary file to be deleted on exit.
     * 
     * @param filepath The path to the temporary file.
     */

    void registerTempFile(const std::filesystem::path& filepath);
    /**
     * @brief Register a temporary file to be deleted on exit.
     * 
     * @param filepath The path to the temporary file.
     */
    void registerTempFile(const std::string& filepath);

    /**
     * @brief Register a temporary file to be deleted on exit.
     * 
     * @param filepath The path to the temporary file.
     */
    void registerTempFile(const char* filepath);
} // namespace fptf
