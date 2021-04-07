#ifndef TYPER_UI_TYPER_WIDGET_HPP
#define TYPER_UI_TYPER_WIDGET_HPP
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <string>

#include <termox/termox.hpp>

namespace typer::ui {

/// Inserts a carriage return Glyph before each newline.
class Carriage_textbox : public ox::detail::Textbox_base {
   public:
    void set_contents(ox::Glyph_string text);

    /// Return the entire contents of the Text_display, without carriage returns
    [[nodiscard]] auto contents() const -> std::u32string const&
    {
        return original_contents_;
    }

   protected:
    /// Implementation detail so accounts for carriage returns.
    [[nodiscard]] auto index_at(ox::Point position) const -> int
    {
        auto const at        = Textbox_base::index_at(position);
        auto const& contents = Textbox_base::contents();
        auto const begin     = std::begin(contents);
        auto const end = std::begin(contents) + at + (contents.empty() ? 0 : 1);
        auto const count = std::count_if(
            begin, end, [](ox::Glyph const& g) { return g.symbol == CR; });
        return at - count;
    }

    [[nodiscard]] auto last_display_index() const -> std::size_t
    {
        auto const at        = last_index_at(bottom_line());
        auto const& contents = Textbox_base::contents();
        auto const count =
            std::count(std::cbegin(contents), std::cbegin(contents) + at, CR);
        return at - count;
    }

    /// Returns the index into contents of the cursor position.
    [[nodiscard]] auto index_at_cursor() const -> std::size_t
    {
        return this->index_at(Widget::cursor.position());
    }

   private:
    std::u32string original_contents_;
    static auto constexpr CR = U'↵';
};

/// From Cursor position until the end of the display.
struct Chunk {
    std::size_t begin;
    std::size_t end;
};

/// Emits signal when contents modified, Chunk is from cursor to end of display.
class Chunked_textbox : public Carriage_textbox {
   public:
    sl::Signal<void(std::u32string)> chunk_change;

   public:
    Chunked_textbox()
    {
        Widget::resized.connect(
            [this](auto, auto) { chunk_change.emit(this->chunk_str()); });
    }

   public:
    void set_contents(ox::Glyph_string text)
    {
        Carriage_textbox::set_contents(std::move(text));
        chunk_change.emit(this->chunk_str());
    }

    /// Return indices into contents from cursor position until end of display.
    [[nodiscard]] auto chunk() const -> Chunk
    {
        return {index_at_cursor(), last_display_index()};
    }

    /// Return substring of cursor position to end of display.
    [[nodiscard]] auto chunk_str() const -> std::u32string
    {
        auto const& text = this->Carriage_textbox::contents();
        if (text.empty())
            return text;
        auto const chk    = chunk();
        auto const length = chk.end - chk.begin;
        return text.substr(chk.begin, length);
    }
};

class Input_follow_textbox : public Chunked_textbox {
   public:
    sl::Signal<bool(char)> key_input;

   public:
    /// Override scroll down function to scroll height amount instead of one.
    void scroll_down(int) override
    {
        this->Textbox_base::scroll_down(this->Widget::height());
        chunk_change.emit(this->Chunked_textbox::chunk_str());
    }

   protected:
    auto key_press_event(ox::Key key) -> bool override
    {
        if (this->is_cursor_at_end())
            return true;

        auto const c = to_character(key);
        if (!c.has_value())
            return true;

        auto const is_correct = key_input.emit(*c).value_or(false);
        color_cursor_glyph(is_correct);
        if (is_correct)
            this->Textbox_base::cursor_right(*c == '\n' ? 2 : 1);

        this->update();
        return true;
    }

   private:
    static auto constexpr accepted_color   = ox::Color::Light_gray;
    static auto constexpr unaccepted_color = ox::Color::Red;

   private:
    /// Return true if the cursor is past the end of the text.
    [[nodiscard]] auto is_cursor_at_end() -> bool
    {
        return this->Carriage_textbox::index_at_cursor() >=
               this->Carriage_textbox::contents().size();
    }

    /// Set the foreground color of the Glyph under the cursor based on whether
    /// a key press was correct or not.
    void color_cursor_glyph(bool correct)
    {
        auto foreground = unaccepted_color;
        if (correct && !this->cursor_glyph_is(unaccepted_color))
            foreground = accepted_color;
        this->set_cursor_glyph_color(foreground);
    }

    /// Return true if the glyph under the cursor has foreground color \p c.
    [[nodiscard]] auto cursor_glyph_is(ox::Color c) -> bool
    {
        return this->glyph_at_cursor().brush.foreground == c;
    }

    /// Set the foreground color to \p c of the glyph under the cursor.
    void set_cursor_glyph_color(ox::Color c)
    {
        this->glyph_at_cursor().brush.foreground = c;
        this->update();
    }

    /// Gets the on screen glyph that the cursor is over.
    [[nodiscard]] auto glyph_at_cursor() -> ox::Glyph&
    {
        return this
            ->Textbox_base::contents()[this->Textbox_base::cursor_index()];
    }

    [[nodiscard]] static auto to_character(ox::Key key) -> std::optional<char>
    {
        if (key == ox::Key::Enter)
            return '\n';

        char const c = key_to_char(key);
        if (std::isprint(static_cast<unsigned char>(c)))
            return c;
        else
            return std::nullopt;
    }
};

struct Typer_widget : Input_follow_textbox {
    Typer_widget()
    {
        *this | ox::pipe::east_west_border() | ox::pipe::strong_focus();
    }
};

using HBuf = ox::Fixed_height<1, ox::Widget>;

struct Typing_window : ox::VTuple<HBuf, Typer_widget, HBuf, ox::Open_file<>> {
    Typer_widget& typer_widget = this->get<1>();
    ox::Open_file<>& load_file = this->get<3>();

    Typing_window() { *this | ox::pipe::direct_focus(); }
};

}  // namespace typer::ui
#endif  // TYPER_UI_TYPER_WIDGET_HPP
