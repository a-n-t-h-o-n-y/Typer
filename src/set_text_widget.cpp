#include <ui/set_text_widget.hpp>

#include <algorithm>
#include <iterator>

#include <cppurses/painter/glyph_string.hpp>

using namespace cppurses;
namespace {

auto is_each_space() {
    return [](const auto& a, const auto& b) { return a == ' ' && b == ' '; };
}

template <typename Container_t>
void remove_adjacent_spaces(Container_t& c) {
    auto new_end = std::unique(std::begin(c), std::end(c), is_each_space());
    c.erase(new_end, std::end(c));
}

auto tidy_text(Textbox& tb) {
    return [&tb]() {
        Glyph_string text{tb.contents()};
        std::replace(std::begin(text), std::end(text), L'\n', L' ');
        remove_adjacent_spaces(text);
        tb.set_contents(text);
    };
}
}  // namespace

namespace typer {
namespace ui {

Text_modify_bar::Text_modify_bar() {
    this->height_policy.fixed(1);
    clear_btn.width_policy.preferred(10);
    tidy_btn.width_policy.preferred(4);
    clear_btn.brush.set_background(Color::White);
    clear_btn.brush.set_foreground(Color::Black);
}

Set_text_widget::Set_text_widget() {
    textbox.brush.set_background(Color::Dark_gray);
    modify_bar.clear_btn.clicked.connect([this]() { textbox.clear(); });
    modify_bar.tidy_btn.clicked.connect(tidy_text(textbox));
}
}  // namespace ui
}  // namespace typer
