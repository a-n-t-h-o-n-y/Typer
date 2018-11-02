#ifndef TYPER_UI_TYPER_WIDGET_HPP
#define TYPER_UI_TYPER_WIDGET_HPP
#include <cppurses/painter/color.hpp>
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

    /// Filters text before sending onto cppurses::Text_display::set_text().
    void set_text(cppurses::Glyph_string text);

    // Signals
    sig::Signal<bool(char)> key_pressed;

    static const wchar_t carriage_return;

   private:
    using cppurses::Text_display::set_text;
    static const cppurses::Color correct_color{cppurses::Color::Light_gray};
    static const cppurses::Color incorrect_color{cppurses::Color::Red};
    void color_cursor_glyph(bool is_correct_key);
};

struct Typing_window : cppurses::Vertical_layout {
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
