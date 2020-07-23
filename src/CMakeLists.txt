add_executable(typer
    main.cpp
    ui/typer_app.cpp
    ui/typer_widget.cpp
)

target_compile_features(typer PRIVATE cxx_std_17)
target_compile_options(typer PRIVATE -Wall -Wextra)
target_link_libraries(typer PRIVATE cppurses)

# Install
install(TARGETS typer RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
