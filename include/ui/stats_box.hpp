#ifndef TYPER_UI_STATS_BOX_HPP
#define TYPER_UI_STATS_BOX_HPP
#include <string>

#include <cppurses/widget/layouts/horizontal.hpp>
#include <cppurses/widget/widgets/label.hpp>
#include <cppurses/widget/widgets/text_display.hpp>

namespace typer {
namespace ui {

struct Stat : cppurses::layout::Horizontal {
    Stat(const std::string& value_, const std::string& label_);
    cppurses::Text_display& value;
    cppurses::Label& label;
    cppurses::Widget& space{this->make_child<cppurses::Widget>()};
};

struct Stats_box : cppurses::layout::Horizontal {
    Stats_box();
    Stat& wpm_stat{this->make_child<Stat>("0", " WPM")};
    Stat& missed_stat{this->make_child<Stat>("0", " Missed")};
    Stat& accuracy_stat{this->make_child<Stat>("--.--%", "")};
};

}  // namespace ui
}  // namespace typer
#endif  // TYPER_UI_STATS_BOX_HPP
