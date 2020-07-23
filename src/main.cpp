#include <cppurses/painter/color.hpp>
#include <cppurses/system/system.hpp>
#include <cppurses/widget/pipe.hpp>
#include <cppurses/widget/widgets/titlebar.hpp>

#include "ui/typer_app.hpp"

using namespace cppurses;

class App : public layout::Vertical<> {
   public:
    Titlebar& titlebar        = this->make_child<Titlebar>("~T-y-p-e-r~");
    typer::ui::Typer_app& app = this->make_child<typer::ui::Typer_app>();

   public:
    App()
    {
        using namespace cppurses::pipe;
        titlebar | children() | fg(Color::White) | bg(Color::Black);
    }
};

int main() { return System{}.run<App>(); }
