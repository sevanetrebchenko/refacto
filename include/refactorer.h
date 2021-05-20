
#ifndef REFACTO_REFACTORER_H
#define REFACTO_REFACTORER_H

#include <string>
#include <initializer_list>
#include <vector>
#include <unordered_map>


namespace Refacto {

    class Refactorer {
        public:
            Refactorer();
            ~Refactorer();

            void AddDirectory(const std::string& directory);

            // Mapping is case-sensitive.
            void AddIncludeGuardMapping(const std::string& from, const std::string& to);
            void AddNamespaceMapping(const std::string& from, const std::string& to);

            void Refactor() const;

        private:
            void FindAndReplace(const std::string& filepath, const std::initializer_list<std::string>& tokens, const std::pair<const std::string, std::string>& mapping) const;

            std::vector<std::string> _directories;
            std::unordered_map<std::string, std::string> _includeGuards;
            std::unordered_map<std::string, std::string> _namespaces;
    };

}

#endif //REFACTO_REFACTORER_H
