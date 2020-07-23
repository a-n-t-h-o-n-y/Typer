#include "typer_widget.hpp"

#include <algorithm>
#include <iterator>

#include <cppurses/system/events/key.hpp>
#include <cppurses/system/system.hpp>
#include <cppurses/widget/widgets/detail/textbox_base.hpp>

namespace {

/// Insert \p to_insert before every occurance of \p to_find.
template <typename Container_t, typename Value_t>
void insert_before_each(Container_t& container,
                        const Value_t& to_insert,
                        const Value_t& to_find)
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

auto Typer_widget::key_press_event(cppurses::Key::State const& keyboard) -> bool
{
    using cppurses::Key;
    if (!this->cursor_is_over_text())
        return Widget::key_press_event(keyboard);
    auto const key = keyboard.key == Key::Enter ? '\n' : keyboard.symbol;
    auto const is_correct_key = key_pressed(key).value_or(false);
    color_cursor_glyph(is_correct_key);
    if (is_correct_key)
        this->Textbox_base::cursor_right(key == '\n' ? 2 : 1);
    this->update();
    return Widget::key_press_event(keyboard);
}

void Typer_widget::set_contents(cppurses::Glyph_string text)
{
    using namespace cppurses;
    insert_before_each(text, carriage_return, L'\n');
    this->Text_display::set_contents(text);
    System::set_focus(*this);
}

}  // namespace typer::ui
