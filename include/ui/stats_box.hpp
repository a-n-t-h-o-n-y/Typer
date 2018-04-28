#ifndef TYPER_UI_STATS_BOX_HPP
#define TYPER_UI_STATS_BOX_HPP
#include <string>

#include <cppurses/widget/layouts/horizontal_layout.hpp>
#include <cppurses/widget/layouts/vertical_layout.hpp>
#include <cppurses/widget/widgets/label.hpp>
#include <cppurses/widget/widgets/text_display.hpp>

namespace typer {
namespace ui {

struct Stat : cppurses::Horizontal_layout {
    Stat(const std::string& label, const std::string& value = "");

    cppurses::Label& label;
    cppurses::Text_display& value;
};

struct Stats_box : cppurses::Vertical_layout {
    Stats_box();
    Stat& wpm_stat{this->make_child<Stat>("WPM:", "0")};
    Stat& missed_stat{this->make_child<Stat>("Missed:", "0")};
    Stat& total_stat{this->make_child<Stat>("Total:", "0")};
    Stat& accuracy_stat{this->make_child<Stat>("Accuracy:", "%")};
};

}  // namespace ui
}  // namespace typer
#endif  // TYPER_UI_STATS_BOX_HPP
