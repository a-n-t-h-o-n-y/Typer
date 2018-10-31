#ifndef TYPER_UI_TYPER_WIDGET_HPP
#define TYPER_UI_TYPER_WIDGET_HPP
#include <cppurses/system/keyboard_data.hpp>
#include <cppurses/widget/layouts/vertical_layout.hpp>
#include <cppurses/widget/widgets/blank_height.hpp>
#include <cppurses/widget/widgets/open_file.hpp>
#include <cppurses/widget/widgets/textbox_base.hpp>
#include <signals/signal.hpp>

namespace typer {
namespace ui {

struct Typer_widget : cppurses::Textbox_base {
    Typer_widget();

    /// Override keyboard input to move cursor.
    bool key_press_event(const cppurses::Keyboard_data& keyboard) override;

    /// Override scroll down function to scroll height amount instead of one.
    void scroll_down(std::size_t n = 1) override;

    // Signals
    sig::Signal<bool(char)> key_pressed;

   private:
    bool last_missed_{false};
};

struct Main_window : cppurses::Vertical_layout {
    cppurses::Blank_height& space_1{
        this->make_child<cppurses::Blank_height>(1)};
    Typer_widget& typer_widget{this->make_child<Typer_widget>()};
    cppurses::Blank_height& space_2{
        this->make_child<cppurses::Blank_height>(1)};
    cppurses::Open_file<>& load_file{this->make_child<cppurses::Open_file<>>()};
};

}  // namespace ui
}  // namespace typer
#endif  // TYPER_UI_TYPER_WIDGET_HPP
