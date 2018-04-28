#ifndef TYPER_UI_OPTIONS_BOX_HPP
#define TYPER_UI_OPTIONS_BOX_HPP
#include <cppurses/widget/layouts/vertical_layout.hpp>
#include <cppurses/widget/widgets/push_button.hpp>

namespace typer {
namespace ui {

struct Options_box : cppurses::Vertical_layout {
    Options_box();

    cppurses::Push_button& change_text_btn{
        this->make_child<cppurses::Push_button>("Change Text")};
};

}  // namespace ui
}  // namespace typer
#endif  // TYPER_UI_OPTIONS_BOX_HPP
