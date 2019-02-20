# Tests
add_executable(typer_tests EXCLUDE_FROM_ALL 
    tests_main.cpp
    typing_test_engine_test.cpp
    ../src/typing_test_engine.cpp
)

target_compile_features(typer_tests PUBLIC cxx_std_14)
target_compile_options(typer_tests PUBLIC -Wall -Wextra)
target_include_directories(typer_tests PRIVATE "${PROJECT_SOURCE_DIR}/include")

add_test(NAME typer_logic COMMAND typer_tests)
