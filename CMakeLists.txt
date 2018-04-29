cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(Typer LANGUAGES CXX)
include(GNUInstallDirs)
enable_testing()
set(CMAKE_BUILD_TYPE Release)
# set(CMAKE_BUILD_TYPE Debug)

# Library
add_subdirectory(src)

# App
add_executable(typer src/main.cpp )
target_link_libraries(typer typer_lib)
target_compile_features(typer PUBLIC cxx_std_14)
target_compile_options(typer PUBLIC -Wall)

# Install
install(TARGETS typer
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})

# Unit Tests
add_subdirectory(tests)
