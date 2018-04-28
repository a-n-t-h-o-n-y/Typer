#ifndef TYPER_UI_TYPER_WIDGET_HPP
#define TYPER_UI_TYPER_WIDGET_HPP
#include <cppurses/widget/widgets/textbox_base.hpp>
#include <signals/signal.hpp>

namespace typer {
namespace ui {

struct Typer_widget : cppurses::Textbox_base {
    Typer_widget();

    /// Override keyboard input to move cursor
    bool key_press_event(cppurses::Key key, char symbol) override;

    /// Override scroll down function to scroll height amount instead of one.
    void scroll_down(std::size_t n = 1) override;

    // Signals
    sig::Signal<bool(char)> key_pressed;

   private:
    bool last_missed_{false};
};

}  // namespace ui
}  // namespace typer
#endif  // TYPER_UI_TYPER_WIDGET_HPP
