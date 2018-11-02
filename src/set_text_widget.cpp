#include <ui/set_text_widget.hpp>

#include <algorithm>
#include <iterator>

#include <cppurses/painter/glyph_string.hpp>

namespace {
auto is_each_space() {
    return [](const auto& a, const auto& b) { return a == ' ' && b == ' '; };
}

template <typename Container_t>
void remove_adjacent_spaces(Container_t& c) {
    auto new_end = std::unique(std::begin(c), std::end(c), is_each_space());
    c.erase(new_end, std::end(c));
}

auto tidy_text(cppurses::Textbox& tb) {
    using namespace cppurses;
    return [&tb]() {
        Glyph_string text{tb.contents()};
        std::replace(std::begin(text), std::end(text), L'\n', L' ');
        remove_adjacent_spaces(text);
        tb.set_text(text);
    };
}
}  // namespace

namespace typer {
namespace ui {

Text_modify_bar::Text_modify_bar() {
    using namespace cppurses;
    this->height_policy.type(Size_policy::Fixed);
    this->height_policy.hint(1);

    set_background(clear_btn, Color::White);
    set_foreground(clear_btn, Color::Black);
    clear_btn.width_policy.hint(10);
    clear_btn.width_policy.type(Size_policy::Preferred);
    clear_btn.disable_word_wrap();

    tidy_btn.width_policy.hint(4);
    tidy_btn.width_policy.type(Size_policy::Preferred);
    tidy_btn.disable_word_wrap();
}

Set_text_widget::Set_text_widget() {
    set_background(textbox, cppurses::Color::Dark_gray);
    modify_bar.clear_btn.clicked.connect([this]() { textbox.clear(); });
    modify_bar.tidy_btn.clicked.connect(tidy_text(textbox));
}

}  // namespace ui
}  // namespace typer
