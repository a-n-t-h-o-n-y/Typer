#include <ui/typer_widget.hpp>

#include <algorithm>
#include <iterator>

#include <cppurses/system/focus.hpp>
#include <cppurses/widget/focus_policy.hpp>

namespace typer {
namespace ui {

const wchar_t Typer_widget::carriage_return{L'↵'};

Typer_widget::Typer_widget() {
    this->focus_policy = cppurses::Focus_policy::Strong;
    enable_border(*this);
    disable_walls(this->border);
    disable_corners(this->border);
    this->border.west_enabled = true;
    this->border.east_enabled = true;
}
}  // namespace ui
}  // namespace typer

namespace {
using namespace cppurses;
bool cursor_over_text(Textbox_base& typer) {
    return typer.cursor_index() < typer.contents_size();
}

bool cursor_glyph_is(Color c, Textbox_base& typer) {
    auto color = typer.glyph_at(typer.cursor_index()).brush.foreground_color();
    return color ? *color == c : false;
}

void set_cursor_glyph(Color c, Textbox_base& typer) {
    typer.glyph_at(typer.cursor_index()).brush.set_foreground(c);
}
}  // namespace

namespace typer {
namespace ui {

bool Typer_widget::key_press_event(const cppurses::Keyboard_data& keyboard) {
    if (!cursor_over_text(*this)) {
        return Widget::key_press_event(keyboard);
    }
    char key{keyboard.key == cppurses::Key::Enter ? '\n' : keyboard.symbol};
    bool is_correct_key{key_pressed(key).value_or(false)};
    color_cursor_glyph(is_correct_key);
    if (is_correct_key) {
        this->cursor_right(key == '\n' ? 2 : 1);
    }
    this->update();
    return Widget::key_press_event(keyboard);
}

void Typer_widget::color_cursor_glyph(bool is_correct_key) {
    cppurses::Color foreground{incorrect_color};
    if (is_correct_key && !cursor_glyph_is(incorrect_color, *this)) {
        foreground = correct_color;
    }
    set_cursor_glyph(foreground, *this);
}

void Typer_widget::scroll_down(std::size_t n) {
    this->Textbox_base::scroll_down(this->Widget::height());
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

void Typer_widget::set_text(cppurses::Glyph_string text) {
    insert_before_each(text, carriage_return, L'\n');
    this->Text_display::set_text(text);
    cppurses::Focus::set_focus_to(this);
}

}  // namespace ui
}  // namespace typer
