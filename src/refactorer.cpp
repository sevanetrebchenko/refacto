
#include <refactorer.h>
#include <fstream>
#include <filesystem>
#include <iostream>

#include <directory.h>

namespace Refacto {

    Refactorer::Refactorer() {
    }

    Refactorer::~Refactorer() {
    }

    void Refactorer::AddDirectory(const std::string &directory) {
        _directories.emplace_back(directory);
    }

    void Refactorer::AddIncludeGuardMapping(const std::string &from, const std::string &to) {
        _includeGuards[from] = to;
    }

    void Refactorer::AddNamespaceMapping(const std::string &from, const std::string &to) {
        _namespaces[from] = to;
    }

    void Refactorer::Refactor() const {
        std::vector<std::string> files;
        std::fstream fileReader;

        std::string path = std::filesystem::current_path().string();

        // Get all files to process from directory.
        for (const std::string &directory : _directories) {
            try {
                files = std::move(GetFilesInDirectory(directory));
            }
            catch (std::runtime_error &exception) {
                std::cerr << exception.what() << std::endl;
                continue;
            }

            for (unsigned i = 0; i < files.size(); ++i) {
                const std::string& filename = files[i];
                std::string native = ConvertToNativeSeparators(filename);
                bool invalid = false;

                // Replace include guards.
                for (const std::pair<const std::string, std::string> &data : _includeGuards) {
                    try {
                        FindAndReplace(native, { "#ifndef", "#define", "#endif" }, data);
                    }
                    catch (std::runtime_error& exception) {
                        std::cerr << exception.what() << std::endl;
                        invalid = true;
                        break;
                    }
                }

                if (invalid) {
                    files.erase(files.begin() + i);

                    // Don't skip files.
                    if (i > 0) {
                        --i;
                    }

                    continue;
                }

                // Replace namespace names.
                for (const std::pair<const std::string, std::string> &data : _namespaces) {
                    try {
                        FindAndReplace(native, { "namespace" }, data);
                    }
                    catch (std::runtime_error& exception) {
                        std::cerr << exception.what() << std::endl;
                        invalid = true;
                        break;
                    }
                }

                if (invalid) {
                    files.erase(files.begin() + i);

                    // Don't skip files.
                    if (i > 0) {
                        --i;
                    }

                    continue;
                }
            }
        }
    }

    void Refactorer::FindAndReplace(const std::string &filepath, const std::initializer_list<std::string> &tokens, const std::pair<const std::string, std::string> &mapping) const {
        // Attempt to open the file.
        std::ifstream fileStream(filepath);
        if (!fileStream.is_open()) {
            throw std::runtime_error("File: " + filepath + " does not exist or cannot be opened. Removing from processing list.");
        }

        const std::string &from = mapping.first;
        const std::string &to = mapping.second;
        std::size_t length = from.size();

        std::string line;
        std::stringstream parser;
        std::stringstream file;

        // Process file.
        while (true) {
            std::getline(fileStream, line);
            if (fileStream.eof()) {
                break;
            }

            std::size_t tokenIndex;

            for (const std::string& tok : tokens) {
                tokenIndex = 0; // Reset for each token.

                while (true) {
                    // Get substring location.
                    tokenIndex = line.find(tok, tokenIndex);

                    if (tokenIndex == std::string::npos) {
                        break;
                    }

                    // Found token, search for mapping.
                    tokenIndex += tok.size();
                    std::size_t mappingIndex = tokenIndex; // Start searching at the end of the found token.

                    while (true) {
                        mappingIndex = line.find(from, mappingIndex);
                        if (mappingIndex == std::string::npos) {
                            break;
                        }

                        // Token exists, replace it.
                        line.replace(mappingIndex, length, to);
                        mappingIndex += length;
                        tokenIndex = mappingIndex; // Update next search location.
                    }
                }
            }

            file << line << std::endl;
        }

        fileStream.close();

        // Write file back.
        std::ofstream output(filepath, std::ios::trunc);
        output << file.str();
        output.close();
    }

}
