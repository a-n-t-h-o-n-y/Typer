#ifndef TYPER_UI_STATS_BOX_HPP
#define TYPER_UI_STATS_BOX_HPP
#include <algorithm>
#include <string>

#include <termox/termox.hpp>

namespace typer::ui {

class Stat : public ox::Passive<ox::HPair<ox::Text_display, ox::HLabel>> {
   public:
    ox::Text_display& value = this->first;
    ox::HLabel& label       = this->second;

   public:
    Stat(std::string const& initial_value, std::string const& label_text)
    {
        using namespace ox::pipe;

        value | contents(initial_value) | align_right() |
            fixed_width(std::max(3uL, initial_value.length()));

        label | dynamic_growth() | text(label_text + " ");
    }
};

class Stats_box : public ox::layout::Horizontal<> {
   public:
    Stat& wpm_stat      = this->make_child<Stat>("0", " WPM");
    ox::Widget& buf_1   = this->make_child();
    Stat& missed_stat   = this->make_child<Stat>("0", " Missed");
    ox::Widget& buf_2   = this->make_child();
    Stat& accuracy_stat = this->make_child<Stat>("--.--%", "");

   public:
    Stats_box()
    {
        using namespace ox::pipe;
        *this | descendants() | bg(ox::Color::White) | fg(ox::Color::Black);
    }
};

}  // namespace typer::ui
#endif  // TYPER_UI_STATS_BOX_HPP
