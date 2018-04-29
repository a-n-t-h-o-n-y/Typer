#ifndef TYPER_UI_TYPING_STACK_HPP
#define TYPER_UI_TYPING_STACK_HPP
#include <cppurses/widget/widgets/widget_stack.hpp>

#include <ui/set_text_widget.hpp>
#include <ui/typer_widget.hpp>

namespace typer {
namespace ui {

struct Typing_stack : cppurses::Widget_stack {
    Typing_stack();

    // Typer_widget& typer_widget{this->make_page<Typer_widget>()};
    Main_window& main_window{this->make_page<Main_window>()};
    Set_text_widget& set_text_widget{this->make_page<Set_text_widget>()};
};

}  // namespace ui
}  // namespace typer
#endif  // TYPER_UI_TYPING_STACK_HPP
