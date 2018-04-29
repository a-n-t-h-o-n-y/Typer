# Tests
add_executable(tests EXCLUDE_FROM_ALL 
    tests_main.cpp
    typer_logic_test.cpp
    )

target_compile_features(tests PUBLIC cxx_std_14)
target_compile_options(tests PUBLIC -Wall)
target_include_directories(tests PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(tests typer_lib)

add_test(logic_typer tests)
