#ifndef TYPER_UI_TYPER_WIDGET_HPP
#define TYPER_UI_TYPER_WIDGET_HPP
#include <cstddef>

#include <termox/termox.hpp>

namespace typer::ui {

class Typer_widget : public ox::detail::Textbox_base {
   public:
    Typer_widget()
    {
        using namespace ox::pipe;
        *this | east_west_border() | strong_focus();
    }

    /// Override scroll down function to scroll height amount instead of one.
    void scroll_down(int) override
    {
        this->Textbox_base::scroll_down(this->Widget::height());
    }

    /// Filters text before sending onto ox::Text_display::set_contents().
    void set_contents(ox::Glyph_string text);

   public:
    sl::Signal<bool(char)> key_pressed;

    static constexpr auto carriage_return = U'↵';

   protected:
    /// Override keyboard input to move cursor.
    auto key_press_event(ox::Key key) -> bool override;

   private:
    using ox::Text_display::set_contents;

   private:
    inline static auto const correct_color   = ox::Color::Light_gray;
    inline static auto const incorrect_color = ox::Color::Red;

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
    auto cursor_glyph_is(ox::Color c) -> bool
    {
        auto const color =
            this->contents()[this->cursor_index()].brush.foreground;
        return color == c;
    }

    /// Set the foreground color to \p c of the glyph under the cursor.
    void set_cursor_glyph_color(ox::Color c)
    {
        this->contents()[this->cursor_index()].brush.foreground = c;
        this->update();
    }
};

class Typing_window : public ox::layout::Vertical<> {
   public:
    ox::Widget& space_1 = this->make_child();

    Typer_widget& typer_widget = this->make_child<Typer_widget>();

    ox::Widget& space_2 = this->make_child();

    ox::Open_file<>& load_file = this->make_child<ox::Open_file<>>();

   public:
    Typing_window()
    {
        using namespace ox::pipe;
        space_1 | fixed_height(1);
        space_2 | fixed_height(1);
    }
};

}  // namespace typer::ui
#endif  // TYPER_UI_TYPER_WIDGET_HPP
