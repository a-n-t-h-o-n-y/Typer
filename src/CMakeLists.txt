# Typer Library
add_library(typer_lib
    typer_logic.cpp
    typer_app.cpp
    top_bar.cpp
    stats_box.cpp
    typer_widget.cpp
    set_text_widget.cpp
    typing_stack.cpp
    )

target_compile_features(typer_lib PUBLIC cxx_std_14)
target_compile_options(typer_lib PUBLIC -Wall)
target_include_directories(typer_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(typer_lib /usr/local/lib64/liblibcppurses.a ncurses)
