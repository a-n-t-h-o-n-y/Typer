#ifndef TYPER_UI_TOP_BAR_HPP
#define TYPER_UI_TOP_BAR_HPP
#include <cppurses/widget/layouts/horizontal_layout.hpp>

#include <ui/options_box.hpp>
#include <ui/stats_box.hpp>

namespace typer {
namespace ui {

struct Top_bar : cppurses::Horizontal_layout {
    Top_bar();

    Stats_box& stats_box{this->make_child<Stats_box>()};
    Options_box& options_box{this->make_child<Options_box>()};
};

}  // namespace ui
}  // namespace typer
#endif  // TYPER_UI_TOP_BAR_HPP
