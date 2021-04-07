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

/// This is the Stats and Typing space together, basically the whole app.
struct Typer_app_widgets : ox::VPair<Top_bar, Typing_stack> {
    Top_bar& top_bar           = this->first;
    Typing_stack& typing_stack = this->second;
};

class Typer_app : public Typer_app_widgets {
   public:
    Typer_app();

   private:
    Typing_test_engine engine_;

   private:
    void set_text(std::u32string const& block)
    {
        typing_stack.typing_window.typer_widget.set_contents(sanitize(block));
    }

   private:
    /// Create a substring from index to end of the passed in Glyph_string.
    [[nodiscard]] static auto substr(ox::Glyph_string const& gs, int index)
        -> ox::Glyph_string
    {
        if (index < gs.size())
            return {std::begin(gs) + index, std::end(gs)};
        else
            return {};
    }

    /// Remove and erase the first value equal to \p v from \p c.
    template <typename Container_t, typename Value_t>
    static void remove_and_erase(Container_t& c, Value_t const& v)
    {
        c.erase(std::remove(std::begin(c), std::end(c), v), std::end(c));
    }

    [[nodiscard]] static auto sanitize(std::u32string x) -> std::string
    {
        auto result = std::string{};
        for (char32_t u : x) {
            switch (u) {
                case U'—': u = '-'; break;
                case U'“': u = '\"'; break;
                case U'”': u = '\"'; break;
                case U'’': u = '\''; break;
                case U'…': result.append(3, '.');
                default: break;
            }
            if ((u > 31 && u < 127) || u == U'\n')
                result.push_back(static_cast<char>(u));
        }
        return result;
    }
};

}  // namespace typer::ui
#endif  // TYPER_UI_TYPER_APP_HPP
