#ifndef TYPER_UI_TYPING_STACK_HPP
#define TYPER_UI_TYPING_STACK_HPP
#include <cppurses/widget/widgets/widget_stack.hpp>

#include <cstddef>

#include <ui/set_text_widget.hpp>
#include <ui/typer_widget.hpp>

namespace typer {
namespace ui {

struct Typing_stack : cppurses::Widget_stack {
    Typing_stack();
    Typing_window& typing_window{this->make_page<Typing_window>()};
    Set_text_widget& set_text_widget{this->make_page<Set_text_widget>()};

    static const std::size_t typing_window_index{0};
    static const std::size_t set_text_index{1};
};

}  // namespace ui
}  // namespace typer
#endif  // TYPER_UI_TYPING_STACK_HPP
