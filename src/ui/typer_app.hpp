#ifndef TYPER_UI_TYPER_APP_HPP
#define TYPER_UI_TYPER_APP_HPP
#include <algorithm>
#include <iterator>
#include <string>

#include <termox/termox.hpp>

#include "../typing_test_engine.hpp"
#include "top_bar.hpp"
#include "typing_stack.hpp"

namespace typer::ui {

class Typer_app : public ox::layout::Vertical<> {
   public:
    Top_bar& top_bar           = this->make_child<Top_bar>();
    Typing_stack& typing_stack = this->make_child<Typing_stack>();

   public:
    Typer_app();

   private:
    Typing_test_engine engine_;

   private:
    void update_stats_box();

    void init_typing_window()
    {
        auto const new_text =
            typing_stack.set_text_widget.textbox.contents().str();
        engine_.set_text(new_text);
        typing_stack.typing_window.typer_widget.set_contents(new_text);
    }

    /// Create a substring from index to end of the passed in Glyph_string.
    static auto glyph_substring(ox::Glyph_string const& gs, int index)
        -> ox::Glyph_string
    {
        using ox::Glyph_string;
        auto substr = Glyph_string{};
        if (index < gs.size()) {
            auto begin = std::begin(gs) + index;
            substr     = Glyph_string{begin, std::end(gs)};
        }
        return substr;
    }

    /// Remove and erase the first value equal to \p v from \p c.
    template <typename Container_t, typename Value_t>
    static void remove_and_erase(Container_t& c, Value_t const& v)
    {
        auto new_end = std::remove(std::begin(c), std::end(c), v);
        c.erase(new_end, std::end(c));
    }

    auto setup_next_chunk()
    {
        return [this](auto) {
            auto const& typer_widget = typing_stack.typing_window.typer_widget;
            auto const top_left      = typer_widget.index_at({0, 0});
            auto new_chunk = glyph_substring(typer_widget.contents(), top_left);
            remove_and_erase(new_chunk, Typer_widget::carriage_return);
            engine_.set_text(new_chunk.str());
        };
    }
};

}  // namespace typer::ui
#endif  // TYPER_UI_TYPER_APP_HPP
