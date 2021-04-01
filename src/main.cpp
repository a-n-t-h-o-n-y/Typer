#include <termox/termox.hpp>

#include "ui/typer_app.hpp"

class App : public ox::VPair<ox::Titlebar, typer::ui::Typer_app> {
   public:
    ox::Titlebar& titlebar = this->first;

   public:
    App()
    {
        using namespace ox::pipe;
        titlebar | children() | fg(ox::Color::White) | bg(ox::Color::Black);
        titlebar.title.set_text(U"~T-y-p-e-r~");
    }
};

int main() { return ox::System{}.run<App>(); }
