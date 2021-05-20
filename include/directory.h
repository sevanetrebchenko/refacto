
#ifndef REFACTO_DIRECTORY_H
#define REFACTO_DIRECTORY_H

#include <string>
#include <vector>

namespace Refacto {

    [[nodiscard]] std::vector<std::string> GetFilesInDirectory(std::string directoryPath);

    [[nodiscard]] std::vector<std::string> GetAssetNames(const std::vector<std::string>& filePaths);
    [[nodiscard]] std::string GetAssetName(const std::string& assetPath);

    [[nodiscard]] std::string GetAssetExtension(const std::string& assetPath);

    [[nodiscard]] std::string ConvertToNativeSeparators(std::string path);

}

#endif //REFACTO_DIRECTORY_H
