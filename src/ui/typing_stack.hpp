#ifndef TYPER_UI_TYPING_STACK_HPP
#define TYPER_UI_TYPING_STACK_HPP
#include <cppurses/widget/layouts/stack.hpp>

#include <cstddef>

#include "set_text_widget.hpp"
#include "typer_widget.hpp"

namespace typer::ui {

class Typing_stack : public cppurses::layout::Stack<> {
   public:
    Typing_window& typing_window     = this->make_page<Typing_window>();
    Set_text_widget& set_text_widget = this->make_page<Set_text_widget>();

   public:
    static auto const typing_window_index = 0uL;
    static auto const set_text_index      = 1uL;

   public:
    Typing_stack() { this->set_active_page(typing_window_index); }
};

}  // namespace typer::ui
#endif  // TYPER_UI_TYPING_STACK_HPP
