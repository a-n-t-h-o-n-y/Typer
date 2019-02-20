#ifndef TYPER_UI_TYPER_WIDGET_HPP
#define TYPER_UI_TYPER_WIDGET_HPP
#include <cppurses/painter/color.hpp>
#include <cppurses/system/events/key.hpp>
#include <cppurses/widget/layouts/vertical.hpp>
#include <cppurses/widget/widgets/detail/textbox_base.hpp>
#include <cppurses/widget/widgets/fixed_height.hpp>
#include <cppurses/widget/widgets/open_file.hpp>
#include <signals/signal.hpp>

namespace typer {
namespace ui {

struct Typer_widget : cppurses::detail::Textbox_base {
    Typer_widget();

    /// Override scroll down function to scroll height amount instead of one.
    void scroll_down(std::size_t) override;

    /// Filters text before sending onto cppurses::Text_display::set_contents().
    void set_contents(cppurses::Glyph_string text);

    // Signals
    sig::Signal<bool(char)> key_pressed;

    static const wchar_t carriage_return;

   protected:
    /// Override keyboard input to move cursor.
    bool key_press_event(const cppurses::Key::State& keyboard) override;

   private:
    using cppurses::Text_display::set_contents;
    static const cppurses::Color correct_color{cppurses::Color::Light_gray};
    static const cppurses::Color incorrect_color{cppurses::Color::Red};
    void color_cursor_glyph(bool is_correct_key);
    bool cursor_over_text();
    bool cursor_glyph_is(cppurses::Color c);
    void set_cursor_glyph(cppurses::Color c);
};

struct Typing_window : cppurses::layout::Vertical {
    cppurses::Fixed_height& space_1{
        this->make_child<cppurses::Fixed_height>(1)};
    Typer_widget& typer_widget{this->make_child<Typer_widget>()};
    cppurses::Fixed_height& space_2{
        this->make_child<cppurses::Fixed_height>(1)};
    cppurses::Open_file<>& load_file{this->make_child<cppurses::Open_file<>>()};
};

}  // namespace ui
}  // namespace typer
#endif  // TYPER_UI_TYPER_WIDGET_HPP
