#include "typer_app.hpp"

#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iterator>
#include <sstream>
#include <string>
#include <utility>

#include <termox/termox.hpp>

#include "typing_stack.hpp"

namespace {

using namespace typer::ui;

/// Return lambda to toggle between the set_text and typing window pages.
auto toggle_active_page(Typing_stack& stack)
{
    return [&stack]() {
        if (stack.active_page_index() == Typing_stack::typing_window_index)
            stack.set_active_page(Typing_stack::set_text_index);
        else
            stack.set_active_page(Typing_stack::typing_window_index);
    };
}

}  // namespace

namespace typer::ui {

Typer_app::Typer_app()
{
    using namespace ox::pipe;

    auto& typer_widget = typing_stack.typing_window.typer_widget;

    top_bar.set_text_btn | on_press(toggle_active_page(typing_stack));

    typer_widget.scrolled_up.connect(setup_next_chunk());
    typer_widget.scrolled_down.connect(setup_next_chunk());

    typer_widget | on_key_press([this](char c) {
        auto const is_correct = engine_.commit_keystroke(c);
        this->update_stats_box();
        return is_correct;
    });

    // Page Changed to Typer Widget
    typing_stack.page_changed.connect([this](std::size_t index) {
        if (index == Typing_stack::typing_window_index)
            this->init_typing_window();
    });

    // Load File
    typing_stack.typing_window.load_file.open_requested.connect(
        [this](std::ifstream& ifs) {
            auto contents = std::string{std::istreambuf_iterator<char>(ifs),
                                        std::istreambuf_iterator<char>()};
            typing_stack.set_text_widget.textbox.set_contents(
                std::move(contents));
            this->init_typing_window();
        });
}

void Typer_app::update_stats_box()
{
    auto const stats = engine_.get_stats();
    auto ss          = std::stringstream{};

    // WPM
    ss << stats.wpm;
    top_bar.stats_box.wpm_stat.value.set_contents(ss.str());

    // Missed
    ss.str("");
    ss << stats.missed_keystrokes;
    top_bar.stats_box.missed_stat.value.set_contents(ss.str());

    // Accuracy
    ss.str("");
    auto percent = 0.;
    if (stats.correct_keystrokes != 0) {
        percent = static_cast<double>(stats.correct_keystrokes) /
                  (stats.correct_keystrokes + stats.missed_keystrokes) * 100;
    }
    ss << std::fixed << std::setprecision(1) << percent << '%';
    top_bar.stats_box.accuracy_stat.value.set_contents(ss.str());
}

}  // namespace typer::ui
