#include <cppurses/cppurses.hpp>

#include <ui/typer_app.hpp>

struct App : cppurses::Vertical_layout {
    App() {
        set_background_recursive(titlebar, cppurses::Color::Black);
        set_foreground_recursive(titlebar, cppurses::Color::White);
    }

    cppurses::Titlebar& titlebar{
        this->make_child<cppurses::Titlebar>("~T-y-p-e-r~")};
    typer::ui::Typer_app& app{this->make_child<typer::ui::Typer_app>()};
};

int main() {
    cppurses::System sys;

    App app;
    sys.set_head(&app);

    return sys.run();
}
