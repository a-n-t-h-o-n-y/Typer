#ifndef TYPER_UI_STATS_BOX_HPP
#define TYPER_UI_STATS_BOX_HPP
#include <algorithm>
#include <string>

#include <cppurses/painter/color.hpp>
#include <cppurses/widget/layouts/horizontal.hpp>
#include <cppurses/widget/pipe.hpp>
#include <cppurses/widget/widgets/label.hpp>
#include <cppurses/widget/widgets/text_display.hpp>

namespace typer::ui {

class Stat : public cppurses::layout::Horizontal<> {
   public:
    cppurses::Text_display& value;
    cppurses::Label& label;
    cppurses::Widget& space = this->make_child<cppurses::Widget>();

   public:
    Stat(std::string const& value_, std::string const& label_)
        : value{this->make_child<cppurses::Text_display>(value_)},
          label{this->make_child<cppurses::Label>(label_)}
    {
        using namespace cppurses::pipe;
        value | align_right() | preferred_width(std::max(3uL, value_.length()));
        label | preferred_width(label_.length() + 1uL) | word_wrap(false);
        space | expanding_width(0);
    }
};

class Stats_box : public cppurses::layout::Horizontal<Stat> {
   public:
    Stat& wpm_stat      = this->make_child("0", " WPM");
    Stat& missed_stat   = this->make_child("0", " Missed");
    Stat& accuracy_stat = this->make_child("--.--%", "");

   public:
    Stats_box()
    {
        using namespace cppurses::pipe;
        using cppurses::Color;

        *this | descendants() | bg(Color::White) | fg(Color::Black);
        accuracy_stat.space | fixed_width(0);
    }
};

}  // namespace typer::ui
#endif  // TYPER_UI_STATS_BOX_HPP
