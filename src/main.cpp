
#include <iostream>
#include <refactorer.h>

int main(int argc, char** argv) {

    Refacto::Refactorer refactorer;

    refactorer.AddDirectory("test");
    refactorer.AddIncludeGuardMapping("REFACTO", "OTCAFER");
    refactorer.AddNamespaceMapping("Testing1", "Renamed");
    refactorer.AddNamespaceMapping("Nested", "Renamed2");

    refactorer.Refactor();

    return 0;
}
