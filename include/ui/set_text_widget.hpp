#ifndef TYPER_UI_SET_TEXT_WIDGET_HPP
#define TYPER_UI_SET_TEXT_WIDGET_HPP
#include <cppurses/widget/layouts/horizontal_layout.hpp>
#include <cppurses/widget/layouts/vertical_layout.hpp>
#include <cppurses/widget/widgets/open_file.hpp>
#include <cppurses/widget/widgets/push_button.hpp>
#include <cppurses/widget/widgets/textbox.hpp>

namespace typer {
namespace ui {

struct Text_modify_bar : cppurses::Horizontal_layout {
    Text_modify_bar();
    cppurses::Push_button& clear_btn{
        this->make_child<cppurses::Push_button>("Clear Text")};
    cppurses::Push_button& tidy_btn{
        this->make_child<cppurses::Push_button>("Tidy")};
};

struct Set_text_widget : cppurses::Vertical_layout {
    Set_text_widget();
    cppurses::Textbox& textbox{
        this->make_child<cppurses::Textbox>("Place Text Here")};
    Text_modify_bar& modify_bar{this->make_child<Text_modify_bar>()};
};

}  // namespace ui
}  // namespace typer
#endif  // TYPER_UI_SET_TEXT_WIDGET_HPP
