#include <ui/set_text_widget.hpp>

#include <algorithm>
#include <fstream>

namespace typer {
namespace ui {

Text_modify_bar::Text_modify_bar() {
    this->height_policy.type(cppurses::Size_policy::Fixed);
    this->height_policy.hint(1);

    set_background(clear_btn, cppurses::Color::White);
    set_foreground(clear_btn, cppurses::Color::Black);
    clear_btn.width_policy.hint(10);
    clear_btn.disable_word_wrap();

    remove_newlines_btn.width_policy.hint(15);
    remove_newlines_btn.disable_word_wrap();

    set_background(remove_extra_space_btn, cppurses::Color::White);
    set_foreground(remove_extra_space_btn, cppurses::Color::Black);
    remove_extra_space_btn.width_policy.hint(18);
    remove_extra_space_btn.disable_word_wrap();
}

Set_text_widget::Set_text_widget() {
    set_background(textbox, cppurses::Color::Dark_gray);

    modify_bar.clear_btn.clicked.connect([this]() { textbox.clear(); });
    modify_bar.remove_newlines_btn.clicked.connect([this] {
        std::string contents{textbox.contents().str()};
        auto iter = std::remove(std::begin(contents), std::end(contents), '\n');
        contents.erase(iter, std::end(contents));
        textbox.set_text(contents);
    });
    modify_bar.remove_extra_space_btn.clicked.connect([this] {
        std::string contents{textbox.contents().str()};
        auto iter = std::unique(
            std::begin(contents), std::end(contents), [](char a, char b) {
                return (a == b && b == ' ') || (a == '\n' && b == ' ');
            });
        contents.erase(iter, std::end(contents));
        if (!contents.empty() && contents.front() == ' ') {
            contents.erase(std::begin(contents));
        }
        textbox.set_text(contents);
    });
}

}  // namespace ui
}  // namespace typer
