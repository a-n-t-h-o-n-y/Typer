#ifndef TYPER_UI_SET_TEXT_PANE_HPP
#define TYPER_UI_SET_TEXT_PANE_HPP
#include <algorithm>
#include <iterator>
#include <utility>

#include <termox/termox.hpp>

namespace typer::ui {

struct Clear_btn : ox::Preferred_width<10, ox::Button> {
    Clear_btn() : Preferred_width{U"Clear Text"}
    {
        *this | bg(ox::Color::White) | fg(ox::Color::Black);
    }
};

struct Tidy_btn : ox::Preferred_width<4, ox::Button> {
    Tidy_btn() : Preferred_width{U"Tidy"} {}
};

struct Text_mod_bar : ox::Fixed_height<1, ox::HPair<Clear_btn, Tidy_btn>> {
    Clear_btn& clear_btn = this->first;
    Tidy_btn& tidy_btn   = this->second;
};

struct Enter_textbox : ox::Textbox {
    Enter_textbox() : Textbox{U"Place Text Here"}
    {
        *this | bg(ox::Color::Dark_gray);
    }
};

struct Set_text_pane_widgets : ox::VPair<Enter_textbox, Text_mod_bar> {
    Enter_textbox& textbox     = this->first;
    Text_mod_bar& text_mod_bar = this->second;
    Clear_btn& clear_btn       = text_mod_bar.clear_btn;
    Tidy_btn& tidy_btn         = text_mod_bar.tidy_btn;
};

/// Display for editing text and modifying it.
class Set_text_pane : public Set_text_pane_widgets {
   public:
    Set_text_pane()
    {
        clear_btn | ox::pipe::on_press([this] { textbox.clear(); });
        tidy_btn | ox::pipe::on_press(tidy_text(textbox));
        *this | ox::pipe::direct_focus();
    }

   private:
    template <typename String_t>
    static void remove_adjacent_spaces(String_t& s)
    {
        auto const end =
            std::unique(std::begin(s), std::end(s),
                        [](auto a, auto b) { return a == b && a == U' '; });
        s.erase(end, std::end(s));
    }

    /// Turn newlines into spaces
    static auto tidy_text(ox::Textbox& tb) -> sl::Slot<void()>
    {
        return [&tb]() {
            auto text = tb.contents().u32str();
            std::replace(std::begin(text), std::end(text), U'\n', U' ');
            remove_adjacent_spaces(text);
            tb.set_contents(text);
        };
    }
};

}  // namespace typer::ui
#endif  // TYPER_UI_SET_TEXT_PANE_HPP
