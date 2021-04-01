#ifndef TYPER_UI_SET_TEXT_WIDGET_HPP
#define TYPER_UI_SET_TEXT_WIDGET_HPP
#include <algorithm>
#include <iterator>

#include <termox/termox.hpp>

namespace typer::ui {

class Text_modify_bar : public ox::layout::Horizontal<ox::Button> {
   public:
    ox::Button& clear_btn = this->make_child("Clear Text");
    ox::Button& tidy_btn  = this->make_child("Tidy");

   public:
    Text_modify_bar()
    {
        using namespace ox::pipe;
        using ox::Color;

        *this | fixed_height(1);
        clear_btn | preferred_width(10) | bg(Color::White) | fg(Color::Black);
        tidy_btn | preferred_width(4);
    }
};

struct Set_text_widget : ox::layout::Vertical<> {
   public:
    ox::Textbox& textbox = this->make_child<ox::Textbox>("Place Text Here");
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

    /// Turn newlines into spaces, ’ -> '
    static auto tidy_text(ox::Textbox& tb)
    {
        return [&tb]() {
            auto text = tb.contents();
            std::replace(std::begin(text), std::end(text), U'\n', U' ');
            std::replace(std::begin(text), std::end(text), U'’', U'\'');
            remove_adjacent_spaces(text);
            tb.set_contents(text);
        };
    }

   public:
    Set_text_widget()
    {
        using namespace ox::pipe;
        textbox | bg(ox::Color::Dark_gray);
        modify_bar.clear_btn | on_press([this]() { textbox.clear(); });
        modify_bar.tidy_btn | on_press(tidy_text(textbox));
    }
};

}  // namespace typer::ui
#endif  // TYPER_UI_SET_TEXT_WIDGET_HPP
