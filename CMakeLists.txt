cmake_minimum_required(VERSION 3.2)

# set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")

# Logic Library
add_library(typer_logic src/typer_logic.cpp)
target_include_directories(typer_logic PUBLIC include)

# App Executable
add_executable(typer src/main.cpp src/typer_app.cpp src/top_bar.cpp
    src/stats_box.cpp src/typer_widget.cpp src/set_text_widget.cpp
    src/typing_stack.cpp)
target_link_libraries(typer PRIVATE typer_logic
    /usr/local/lib64/liblibcppurses.a ncurses)

# Unit Tests
enable_testing()
add_executable(tests EXCLUDE_FROM_ALL tests/typer_logic_test.cpp)
target_link_libraries(tests typer_logic)
add_test(logic_typer tests)
