#include <cppurses/cppurses.hpp>

#include <ui/typer_app.hpp>

int main() {
    cppurses::System sys;

    typer::ui::Typer_app app;
    sys.set_head(&app);

    return sys.run();
}
