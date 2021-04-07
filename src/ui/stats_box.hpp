#ifndef TYPER_UI_STATS_BOX_HPP
#define TYPER_UI_STATS_BOX_HPP
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>

#include <termox/termox.hpp>

#include "../typing_test_engine.hpp"

namespace typer::ui {

template <int Width = 3>
class Int_display : public ox::Fixed_width<Width, ox::HLabel> {
   public:
    explicit Int_display(int init = 0)
    {
        using namespace ox::pipe;
        *this | text(std::to_string(init)) | align_right();
    }

   public:
    void set(int value) { *this | ox::pipe::text(std::to_string(value)); }
};

template <int Precision = 1>
class Percent_display : public ox::Fixed_width<Precision + 5, ox::HLabel> {
   public:
    explicit Percent_display(double init = 0.)
    {
        using namespace ox::pipe;
        *this | text(as_str(init)) | align_right();
    }

   public:
    void set(double ratio) { *this | ox::pipe::text(as_str(ratio)); }

   private:
    static auto as_str(double x) -> std::string
    {
        auto ss = std::ostringstream{};
        ss.precision(Precision);
        ss << std::fixed << (x * 100.) << '%';
        return ss.str();
    }
};

struct Dynamic_label : ox::HLabel {
    explicit Dynamic_label(std::string const& text) : ox::HLabel{text}
    {
        *this | ox::pipe::dynamic_growth();
    }
};

struct WPM_label : Dynamic_label {
    WPM_label() : Dynamic_label{" WPM"} {}
};
using WPM_stat = ox::Passive<ox::HPair<Int_display<>, WPM_label>>;

struct Missed_label : Dynamic_label {
    Missed_label() : Dynamic_label{" Missed"} {}
};
using Missed_stat = ox::Passive<ox::HPair<Int_display<>, Missed_label>>;

using Buf = ox::Widget;

struct Stats_box_widgets
    : ox::HTuple<WPM_stat, Buf, Missed_stat, Buf, Percent_display<>, ox::Tile> {
   public:
    Int_display<>& wpm_stat          = this->get<0>().first;
    Int_display<>& missed_stat       = this->get<2>().first;
    Percent_display<>& accuracy_stat = this->get<4>();

   public:
    Stats_box_widgets()
    {
        using namespace ox;
        *this | pipe::descendants() | bg(Color::White) | fg(Color::Black);
    }
};

class Stats_box : public Stats_box_widgets {
   public:
    /// Update the display to show \p stats.
    void set(typer::Typing_test_engine::Stats stats)
    {
        wpm_stat.set(stats.wpm);
        missed_stat.set(stats.missed_keystrokes);
        accuracy_stat.set(
            ratio(stats.correct_keystrokes, total_keystrokes(stats)));
    }

   private:
    static auto ratio(int a, int b) -> double
    {
        return (b == 0) ? 0. : static_cast<double>(a) / b;
    }

    static auto total_keystrokes(typer::Typing_test_engine::Stats stats) -> int
    {
        return stats.correct_keystrokes + stats.missed_keystrokes;
    }
};

}  // namespace typer::ui
#endif  // TYPER_UI_STATS_BOX_HPP
