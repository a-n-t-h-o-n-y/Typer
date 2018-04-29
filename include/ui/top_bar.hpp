#ifndef TYPER_UI_TOP_BAR_HPP
#define TYPER_UI_TOP_BAR_HPP
#include <cppurses/widget/layouts/horizontal_layout.hpp>
#include <cppurses/widget/widgets/push_button.hpp>

#include <ui/stats_box.hpp>

namespace typer {
namespace ui {

struct Top_bar : cppurses::Horizontal_layout {
    Top_bar();

    Stats_box& stats_box{this->make_child<Stats_box>()};
    cppurses::Push_button& set_text_btn{
        this->make_child<cppurses::Push_button>("Set Text")};
};

}  // namespace ui
}  // namespace typer
#endif  // TYPER_UI_TOP_BAR_HPP
