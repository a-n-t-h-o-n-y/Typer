cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(Typer LANGUAGES CXX)
include(GNUInstallDirs)
enable_testing()

add_subdirectory(external/TermOx)
add_subdirectory(src)
add_subdirectory(tests)
