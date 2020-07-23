#ifndef TYPER_UI_SET_TEXT_WIDGET_HPP
#define TYPER_UI_SET_TEXT_WIDGET_HPP
#include <algorithm>
#include <iterator>

#include <cppurses/painter/color.hpp>
#include <cppurses/widget/layouts/horizontal.hpp>
#include <cppurses/widget/layouts/vertical.hpp>
#include <cppurses/widget/pipe.hpp>
#include <cppurses/widget/widgets/button.hpp>
#include <cppurses/widget/widgets/open_file.hpp>
#include <cppurses/widget/widgets/textbox.hpp>

namespace typer::ui {

class Text_modify_bar : public cppurses::layout::Horizontal<cppurses::Button> {
   public:
    cppurses::Button& clear_btn = this->make_child("Clear Text");
    cppurses::Button& tidy_btn  = this->make_child("Tidy");

   public:
    Text_modify_bar()
    {
        using namespace cppurses::pipe;
        using cppurses::Color;

        *this | fixed_height(1);
        clear_btn | preferred_width(10) | bg(Color::White) | fg(Color::Black);
        tidy_btn | preferred_width(4);
    }
};

struct Set_text_widget : cppurses::layout::Vertical<> {
   public:
    cppurses::Textbox& textbox =
        this->make_child<cppurses::Textbox>("Place Text Here");
    Text_modify_bar& modify_bar = this->make_child<Text_modify_bar>();

   private:
    template <typename Container_t>
    static void remove_adjacent_spaces(Container_t& c)
    {
        auto const is_each_a_space = [](auto const& a, auto const& b) {
            return a == ' ' && b == ' ';
        };
        auto const new_end =
            std::unique(std::begin(c), std::end(c), is_each_a_space);
        c.erase(new_end, std::end(c));
    }

    static auto tidy_text(cppurses::Textbox& tb)
    {
        return [&tb]() {
            auto text = tb.contents();
            std::replace(std::begin(text), std::end(text), L'\n', L' ');
            remove_adjacent_spaces(text);
            tb.set_contents(text);
        };
    }

   public:
    Set_text_widget()
    {
        using namespace cppurses::pipe;
        textbox | bg(cppurses::Color::Dark_gray);
        modify_bar.clear_btn | on_press([this]() { textbox.clear(); });
        modify_bar.tidy_btn | on_press(tidy_text(textbox));
    }
};

}  // namespace typer::ui
#endif  // TYPER_UI_SET_TEXT_WIDGET_HPP
