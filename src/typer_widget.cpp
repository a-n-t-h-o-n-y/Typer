#include <ui/typer_widget.hpp>

#include <cppurses/widget/focus_policy.hpp>

namespace typer {
namespace ui {

Typer_widget::Typer_widget() {
    this->focus_policy = cppurses::Focus_policy::Strong;
}

bool Typer_widget::key_press_event(cppurses::Key key, char symbol) {
    if (key == cppurses::Key::Enter) {
        symbol = '\n';
    }
    if (!(std::isprint(symbol) || std::isspace(symbol))) {
        return Widget::key_press_event(key, symbol);
    }
    auto key_valid{key_pressed(symbol)};
    if (key_valid && *key_valid) {
        if (last_missed_) {
            last_missed_ = false;
        } else {
            this->Text_display::glyph_at(this->Textbox_base::cursor_index())
                .brush.set_foreground(cppurses::Color::Light_gray);
        }
        this->Textbox_base::cursor_right();
    } else if (this->cursor_index() < this->contents_size()) {
        last_missed_ = true;
        this->Text_display::glyph_at(this->Textbox_base::cursor_index())
            .brush.set_foreground(cppurses::Color::Red);
    }
    this->Widget::update();
    return Widget::key_press_event(key, symbol);
}

void Typer_widget::scroll_down(std::size_t n) {
    this->Textbox_base::scroll_down(this->Widget::height());
}

}  // namespace ui
}  // namespace typer
