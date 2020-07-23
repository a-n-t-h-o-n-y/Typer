#ifndef TYPER_UI_TOP_BAR_HPP
#define TYPER_UI_TOP_BAR_HPP
#include <cppurses/painter/color.hpp>
#include <cppurses/widget/layouts/horizontal.hpp>
#include <cppurses/widget/pipe.hpp>
#include <cppurses/widget/widgets/button.hpp>

#include "stats_box.hpp"

namespace typer::ui {

class Top_bar : public cppurses::layout::Horizontal<> {
   public:
    Stats_box& stats_box = this->make_child<Stats_box>();
    cppurses::Button& set_text_btn =
        this->make_child<cppurses::Button>("Set Text");

   public:
    Top_bar()
    {
        using namespace cppurses::pipe;

        *this | fixed_height(1);
        stats_box | expanding_width(0);
        set_text_btn | preferred_width(10) | bg(cppurses::Color::Light_green) |
            fg(cppurses::Color::Black);
    }
};

}  // namespace typer::ui
#endif  // TYPER_UI_TOP_BAR_HPP
