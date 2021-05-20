
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

        // Get all files to process from directory.
        for (const std::string &directory : _directories) {
            try {
                files = std::move(GetFilesInDirectory(directory));
            }
            catch (std::runtime_error &exception) {
                std::cerr << exception.what() << std::endl;
                continue;
            }

            for (const std::string &filename : files) {
                std::string native = ConvertToNativeSeparators(filename);

                for (const std::pair<const std::string, std::string> &data : _includeGuards) {
                    try {
                        FindAndReplace(native, {"#ifndef", "#define", "#endif"}, data); // Replace include guards.
                        FindAndReplace(native, {"namespace"}, data); // Replace namespace.
                    }
                    catch (std::runtime_error &exception) {
                        std::cerr << exception.what() << std::endl;
                        break;
                    }
                }
            }
        }
    }

    void Refactorer::FindAndReplace(const std::string &filepath, const std::initializer_list<std::string> &tokens,
                                    const std::pair<const std::string, std::string> &mapping) const {
        // Attempt to open the file.
        std::fstream fileStream(filepath);
        if (!fileStream.is_open()) {
            throw std::runtime_error("File: " + GetAssetName(filepath) + " does not exist.");
        }
        const std::string &from = mapping.first;
        const std::string &to = mapping.second;

        std::string line;
        std::string token;
        std::stringstream parser;
        std::stringstream file;

        // Process file.
        while (!fileStream.eof()) {
            std::getline(fileStream, line);

            // Reset parser (eof bit).
            parser.clear();
            parser.str(line);

            while (parser >> token) {
                bool found = false;
                for (const std::string &test : tokens) {
                    if (token == test) {
                        // Found include guard.
                        found = true;
                        break;
                    }
                }

                if (found) {
                    file << token << std::endl;

                    parser >> token; // Get include guard name.

                    std::size_t index = 0;
                    while (true) {
                        // Get substring location.
                        index = token.find(from, index);
                        if (index == std::string::npos) {
                            // Substring not found in this token.
                            break;
                        }

                        // Make replacement.
                        std::size_t length = from.size();

                        token.replace(index, length, to);
                        index += length;
                    }
                }

                file << token << std::endl;
            }
        }

        fileStream.close();

        // Write file back.
        std::ofstream output(filepath, std::ios::trunc);
        output << file.str();
        output.close();
    }

}
