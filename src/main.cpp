#include <cppurses/painter/color.hpp>
#include <cppurses/system/system.hpp>
#include <cppurses/widget/widgets/titlebar.hpp>

#include <ui/typer_app.hpp>

using namespace cppurses;

struct App : layout::Vertical {
    App() {
        for (const auto& child : titlebar.children.get()) {
            child->brush.set_background(Color::Black);
            child->brush.set_foreground(Color::White);
        }
    }
    Titlebar& titlebar{this->make_child<Titlebar>("~T-y-p-e-r~")};
    typer::ui::Typer_app& app{this->make_child<typer::ui::Typer_app>()};
};

int main() {
    System sys;
    return sys.run<App>();
}
