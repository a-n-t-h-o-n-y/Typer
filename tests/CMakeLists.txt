cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

# Tests
add_executable(typer_tests EXCLUDE_FROM_ALL
    tests.main.cpp
    typing_test_engine_test.cpp
)

target_compile_features(typer_tests PUBLIC cxx_std_17)
target_compile_options(typer_tests PUBLIC -Wall -Wextra)
target_include_directories(typer_tests PRIVATE "${PROJECT_SOURCE_DIR}/src")

add_test(NAME typer_logic COMMAND typer_tests)
