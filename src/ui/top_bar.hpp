#ifndef TYPER_UI_TOP_BAR_HPP
#define TYPER_UI_TOP_BAR_HPP
#include <termox/termox.hpp>

#include "stats_box.hpp"

namespace typer::ui {

struct Set_text_btn : ox::Fixed_width<10, ox::Button> {
    Set_text_btn() : Fixed_width{U"Set Text"}
    {
        *this | bg(ox::Color::Light_green) | fg(ox::Color::Black);
    }
};

struct Top_bar : ox::Fixed_height<1, ox::HPair<Stats_box, Set_text_btn>> {
    Stats_box& stats_box       = this->first;
    Set_text_btn& set_text_btn = this->second;
};

}  // namespace typer::ui
#endif  // TYPER_UI_TOP_BAR_HPP
