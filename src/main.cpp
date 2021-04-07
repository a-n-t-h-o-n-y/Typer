#include <termox/termox.hpp>

#include "ui/typer_app.hpp"

using namespace ox;

namespace {

struct Title : Titlebar {
    Title() : Titlebar{U"~T-y-p-e-r~"}
    {
        *this | pipe::children() | fg(Color::White) | bg(Color::Black);
    }
};

using App = VPair<Title, typer::ui::Typer_app>;

}  // namespace

int main() { return System{}.run<App>(); }
