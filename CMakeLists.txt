cmake_minimum_required(VERSION 3.16)
project(colorize
    VERSION 1.0.0
    DESCRIPTION "Colorize text in the terminal (name / HEX / 256-index)"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-Wall -Wextra -Wpedantic)

add_library(colorize_core STATIC
    src/colorize_core.cpp
)
target_include_directories(colorize_core PUBLIC src)

add_executable(colorize src/main.cpp)
target_link_libraries(colorize PRIVATE colorize_core)

include(GNUInstallDirs)
install(TARGETS colorize
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# --- Tests -------------------------------------------------------------
enable_testing()
add_subdirectory(tests)
