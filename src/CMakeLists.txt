add_executable(typer
    main.cpp
    typing_test_engine.cpp
    typer_app.cpp
    top_bar.cpp
    stats_box.cpp
    typer_widget.cpp
    set_text_widget.cpp
    typing_stack.cpp
)

target_compile_features(typer PRIVATE cxx_std_14)
target_compile_options(typer PRIVATE -Wall -Wextra)
target_include_directories(typer PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(typer PRIVATE cppurses)

# Install
install(TARGETS typer RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# CLANG TIDY
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_custom_target(compile_commands ALL
    COMMAND [ -f ${CMAKE_SOURCE_DIR}/build/compile_commands.json ] && mv
    	${CMAKE_SOURCE_DIR}/build/release/compile_commands.json
    	${CMAKE_SOURCE_DIR}/compile_commands.json || :)
