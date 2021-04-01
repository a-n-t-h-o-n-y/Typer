#include "typer_widget.hpp"

#include <algorithm>
#include <iterator>

#include <termox/termox.hpp>

namespace {

/// Insert \p to_insert before every occurance of \p to_find.
template <typename Container_t, typename Value_t>
void insert_before_each(Container_t& container,
                        Value_t const& to_insert,
                        Value_t const& to_find)
{
    auto iter = std::find(std::begin(container), std::end(container), to_find);
    while (iter != std::end(container)) {
        iter = container.insert(iter, to_insert);
        std::advance(iter, 2);
        iter = std::find(iter, std::end(container), to_find);
    }
}

}  // namespace

namespace typer::ui {

auto Typer_widget::key_press_event(ox::Key key) -> bool
{
    using ox::Key;
    if (!this->cursor_is_over_text())
        return Widget::key_press_event(key);
    auto const c              = key == Key::Enter ? '\n' : key_to_char(key);
    auto const is_correct_key = key_pressed(c).value_or(false);
    color_cursor_glyph(is_correct_key);
    if (is_correct_key)
        this->Textbox_base::cursor_right(c == '\n' ? 2 : 1);
    this->update();
    return Widget::key_press_event(key);
}

void Typer_widget::set_contents(ox::Glyph_string text)
{
    using namespace ox;
    insert_before_each(text, carriage_return, U'\n');
    this->Text_display::set_contents(text);
    System::set_focus(*this);
}

}  // namespace typer::ui
