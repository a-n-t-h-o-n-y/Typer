#include <ui/typer_widget.hpp>

#include <algorithm>
#include <iterator>

#include <cppurses/system/focus.hpp>
#include <cppurses/widget/focus_policy.hpp>
#include <cppurses/widget/widgets/detail/textbox_base.hpp>

using namespace cppurses;

namespace typer {
namespace ui {

const wchar_t Typer_widget::carriage_return{L'↵'};

Typer_widget::Typer_widget() {
    this->focus_policy = Focus_policy::Strong;
    this->border.enable();
    this->border.segments.disable_all();
    this->border.segments.west.enable();
    this->border.segments.east.enable();
}
}  // namespace ui
}  // namespace typer

namespace typer {
namespace ui {

bool Typer_widget::cursor_over_text() {
    return this->cursor_index() < this->contents().size();
}

bool Typer_widget::cursor_glyph_is(Color c) {
    auto color =
        this->contents()[this->cursor_index()].brush.foreground_color();
    return color ? *color == c : false;
}

void Typer_widget::set_cursor_glyph(Color c) {
    this->contents()[this->cursor_index()].brush.set_foreground(c);
}

bool Typer_widget::key_press_event(const Key::State& keyboard) {
    if (!this->cursor_over_text()) {
        return Widget::key_press_event(keyboard);
    }
    char key{keyboard.key == Key::Enter ? '\n' : keyboard.symbol};
    bool is_correct_key{key_pressed(key).value_or(false)};
    color_cursor_glyph(is_correct_key);
    if (is_correct_key) {
        this->cursor_right(key == '\n' ? 2 : 1);
    }
    this->update();
    return Widget::key_press_event(keyboard);
}

void Typer_widget::color_cursor_glyph(bool is_correct_key) {
    Color foreground{incorrect_color};
    if (is_correct_key && !this->cursor_glyph_is(incorrect_color)) {
        foreground = correct_color;
    }
    this->set_cursor_glyph(foreground);
}

void Typer_widget::scroll_down(std::size_t) {
    this->detail::Textbox_base::scroll_down(this->Widget::height());
}

}  // namespace ui
}  // namespace typer

namespace {
template <typename Container_t, typename Value_t>
void insert_before_each(Container_t& container,
                        const Value_t& to_insert,
                        const Value_t& to_find) {
    auto iter = std::find(std::begin(container), std::end(container), to_find);
    while (iter != std::end(container)) {
        iter = container.insert(iter, to_insert);
        std::advance(iter, 2);
        iter = std::find(iter, std::end(container), to_find);
    }
}
}  // namespace

namespace typer {
namespace ui {

void Typer_widget::set_contents(Glyph_string text) {
    insert_before_each(text, carriage_return, L'\n');
    this->Text_display::set_contents(text);
    Focus::set_focus_to(this);
}
}  // namespace ui
}  // namespace typer
