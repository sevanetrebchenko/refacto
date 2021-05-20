
set(CORE_SOURCE_FILES
        "${PROJECT_SOURCE_DIR}/src/main.cpp"
        "${PROJECT_SOURCE_DIR}/src/refactorer.cpp"
        "${PROJECT_SOURCE_DIR}/src/directory.cpp"
        )

add_executable(Refacto ${CORE_SOURCE_FILES})

target_include_directories(Refacto PUBLIC "${PROJECT_SOURCE_DIR}/include/")
