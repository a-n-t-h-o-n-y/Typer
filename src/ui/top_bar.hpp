#ifndef TYPER_UI_TOP_BAR_HPP
#define TYPER_UI_TOP_BAR_HPP
#include <termox/termox.hpp>

#include "stats_box.hpp"

namespace typer::ui {

struct Set_text_btn : ox::Button {
    Set_text_btn() : ox::Button{U"Set Text"}
    {
        using namespace ox::pipe;
        using ox::Color;
        *this | preferred_width(10) | bg(Color::Light_green) | fg(Color::Black);
    }
};

class Top_bar : public ox::HPair<Stats_box, Set_text_btn> {
   public:
    Stats_box& stats_box       = this->first;
    Set_text_btn& set_text_btn = this->second;

   public:
    Top_bar()
    {
        using namespace ox::pipe;
        *this | fixed_height(1);
        stats_box | expanding_width(0);
    }
};

}  // namespace typer::ui
#endif  // TYPER_UI_TOP_BAR_HPP
