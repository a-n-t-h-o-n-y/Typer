#include "typer_app.hpp"

#include <cstddef>
#include <cuchar>
#include <cwchar>
#include <fstream>
#include <iomanip>
#include <iterator>
#include <sstream>
#include <string>
#include <utility>

#include <termox/termox.hpp>

#include "typing_stack.hpp"

namespace {

/// Convert byte stream into char32_t stream.
[[nodiscard]] auto mb_to_u32(std::string const& s) -> std::u32string
{
    auto mb_state = std::mbstate_t{};
    auto offset   = 0uL;
    auto result   = std::u32string{};
    while (offset != s.size()) {
        auto temp = char32_t{};
        offset += std::mbrtoc32(&temp, s.data() + offset, s.size() - offset,
                                &mb_state);
        result.push_back(temp);
    }
    return result;
}

}  // namespace

namespace typer::ui {

Typer_app::Typer_app()
{
    using namespace ox::pipe;

    top_bar.set_text_btn | on_press([this] { typing_stack.toggle_page(); });

    auto& typer_widget = typing_stack.typing_window.typer_widget;

    typer_widget.chunk_change.connect(
        [this](std::u32string chunk) { engine_.set_text(sanitize(chunk)); });

    typer_widget.key_input.connect([this](char c) {
        auto const [is_correct, stats] = engine_.commit_keystroke(c);
        top_bar.stats_box.set(stats);
        return is_correct;
    });

    typing_stack.text_set.connect(
        [this](std::u32string const& contents) { this->set_text(contents); });

    // Load File
    typing_stack.typing_window.load_file.open_requested.connect(
        [this](std::ifstream& ifs) {
            using Iter_t  = std::istreambuf_iterator<char>;
            auto contents = std::string{Iter_t(ifs), Iter_t()};
            this->set_text(mb_to_u32(contents));
            typing_stack.set_text_widget.textbox.set_contents(
                std::move(contents));
        });
}

}  // namespace typer::ui
