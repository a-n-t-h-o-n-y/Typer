#ifndef TYPER_UI_TYPER_WIDGET_HPP
#define TYPER_UI_TYPER_WIDGET_HPP
#include <cstddef>

#include <cppurses/painter/color.hpp>
#include <cppurses/system/events/key.hpp>
#include <cppurses/widget/layouts/vertical.hpp>
#include <cppurses/widget/widgets/detail/textbox_base.hpp>
#include <cppurses/widget/widgets/fixed_height.hpp>
#include <cppurses/widget/widgets/open_file.hpp>

#include <signals/signal.hpp>

namespace typer::ui {

class Typer_widget : public cppurses::detail::Textbox_base {
   public:
    Typer_widget()
    {
        using namespace cppurses::pipe;
        *this | east_west_border() | strong_focus();
    }

    /// Override scroll down function to scroll height amount instead of one.
    void scroll_down(std::size_t) override
    {
        this->Textbox_base::scroll_down(this->Widget::height());
    }

    /// Filters text before sending onto cppurses::Text_display::set_contents().
    void set_contents(cppurses::Glyph_string text);

   public:
    sig::Signal<bool(char)> key_pressed;

    inline static wchar_t const carriage_return = L'↵';

   protected:
    /// Override keyboard input to move cursor.
    auto key_press_event(cppurses::Key::State const& keyboard) -> bool override;

   private:
    using cppurses::Text_display::set_contents;

   private:
    inline static auto const correct_color   = cppurses::Color::Light_gray;
    inline static auto const incorrect_color = cppurses::Color::Red;

   private:
    /// Set the foreground color of the Glyph under the cursor based on whether
    /// a key press was correct or not.
    void color_cursor_glyph(bool is_correct_key)
    {
        auto foreground = incorrect_color;
        if (is_correct_key && !this->cursor_glyph_is(incorrect_color))
            foreground = correct_color;
        this->set_cursor_glyph_color(foreground);
    }

    /// Return true if the cursor is over any part of the text contents.
    auto cursor_is_over_text() -> bool
    {
        return this->cursor_index() < this->contents().size();
    }

    /// Return true if the glyph under the cursor has foreground color \p c.
    auto cursor_glyph_is(cppurses::Color c) -> bool
    {
        auto const color =
            this->contents()[this->cursor_index()].brush.foreground_color();
        return color ? *color == c : false;
    }

    /// Set the foreground color to \p c of the glyph under the cursor.
    void set_cursor_glyph_color(cppurses::Color c)
    {
        this->contents()[this->cursor_index()].brush.set_foreground(c);
    }
};

class Typing_window : public cppurses::layout::Vertical<> {
   public:
    cppurses::Fixed_height& space_1 =
        this->make_child<cppurses::Fixed_height>(1);

    Typer_widget& typer_widget = this->make_child<Typer_widget>();

    cppurses::Fixed_height& space_2 =
        this->make_child<cppurses::Fixed_height>(1);

    cppurses::Open_file<>& load_file =
        this->make_child<cppurses::Open_file<>>();
};

}  // namespace typer::ui
#endif  // TYPER_UI_TYPER_WIDGET_HPP
