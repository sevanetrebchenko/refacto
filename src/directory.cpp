
#include <directory.h>
#include <filesystem>
#include <stdexcept>

namespace Refacto {

    std::vector<std::string> GetFilesInDirectory(std::string directoryPath) {
        directoryPath = ConvertToNativeSeparators(directoryPath);

        std::filesystem::path path { directoryPath };
        if (std::filesystem::is_directory(path)) {
            std::vector<std::string> filesInDirectory;

            for (auto& file : std::filesystem::directory_iterator(path)) {
                filesInDirectory.push_back(file.path().string());
            }

            return filesInDirectory;
        }

        throw std::runtime_error("Provided directory does not exist.");
    }

    std::vector<std::string> GetAssetNames(const std::vector<std::string> &filePaths) {
        std::vector<std::string> assets;

        assets.reserve(filePaths.size());
        for (auto& filePath : filePaths) {
            assets.emplace_back(GetAssetName(filePath));
        }

        return assets;
    }

    std::string GetAssetName(const std::string &assetPath) {
        std::string nativeAssetPath = ConvertToNativeSeparators(assetPath);
        std::size_t slashPosition;

#ifdef _WIN32
        slashPosition = nativeAssetPath.find_last_of('\\');
#else
        slashPosition = nativeAssetPath.find_last_of('/');
#endif

        std::size_t periodPosition = nativeAssetPath.find_first_of('.');

        // If there exists a slash.
        if (slashPosition != std::string::npos) {
            return nativeAssetPath.substr(slashPosition + 1, periodPosition);
        }

        return nativeAssetPath.substr(0, periodPosition);
    }

    std::string GetAssetExtension(const std::string &assetPath) {
        return assetPath.substr(assetPath.find_last_of('.') + 1);
    }

    std::string ConvertToNativeSeparators(std::string path) {
#ifdef _WIN32
        const char separator = '/';
        const char native = '\\';
#else
        const char separator = '\\';
            const char native = '/';
#endif
        std::replace(path.begin(), path.end(), separator, native);
        return path;
    }

}

