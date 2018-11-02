#include <ui/typer_app.hpp>

#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>

#include <ui/typing_stack.hpp>

namespace {
using namespace typer::ui;
auto toggle_active_page(Typing_stack& stack) {
    return [&stack]() {
        if (stack.active_page_index() == Typing_stack::typing_window_index) {
            stack.set_active_page(Typing_stack::set_text_index);
        } else {
            stack.set_active_page(Typing_stack::typing_window_index);
        }
    };
}

cppurses::Glyph_string glyph_substring(const cppurses::Glyph_string& gs,
                                       std::size_t index) {
    cppurses::Glyph_string substr;
    if (index < gs.size()) {
        auto begin = std::begin(gs) + index;
        substr = cppurses::Glyph_string{begin, std::end(gs)};
    }
    return substr;
}

template <typename Container_t, typename Value_t>
void remove_and_erase(Container_t& c, const Value_t& v) {
    auto new_end = std::remove(std::begin(c), std::end(c), v);
    c.erase(new_end, std::end(c));
}
}  // namespace

namespace typer {
namespace ui {

auto Typer_app::setup_next_chunk() {
    return [this]() {
        Typer_widget& typer_widget = typing_stack.typing_window.typer_widget;
        std::size_t top_left{typer_widget.index_at(0, 0)};
        auto new_chunk = glyph_substring(typer_widget.contents(), top_left);
        remove_and_erase(new_chunk, Typer_widget::carriage_return);
        engine_.set_text(new_chunk.str());
    };
}

Typer_app::Typer_app() {
    Typer_widget& typer_widget{typing_stack.typing_window.typer_widget};

    // Set Text
    top_bar.set_text_btn.clicked.connect(toggle_active_page(typing_stack));

    // Typer Widget Scrolled
    typer_widget.scrolled.connect(setup_next_chunk());

    // Key Pressed
    typer_widget.key_pressed.connect([this](char c) {
        bool correct{engine_.commit_keystroke(c)};
        this->update_stats_box();
        return correct;
    });

    // Page Changed to Typer Widget
    typing_stack.page_changed.connect([this](std::size_t index) {
        if (index == Typing_stack::typing_window_index) {
            this->init_typing_window();
        }
    });

    // Load File
    typing_stack.typing_window.load_file.open_requested.connect(
        [this](std::ifstream& ifs) {
            std::string contents{std::istreambuf_iterator<char>(ifs),
                                 std::istreambuf_iterator<char>()};
            typing_stack.set_text_widget.textbox.set_text(contents);
            this->init_typing_window();
        });
}

void Typer_app::init_typing_window() {
    std::string new_text{typing_stack.set_text_widget.textbox.contents().str()};
    engine_.set_text(new_text);
    typing_stack.typing_window.typer_widget.set_text(new_text);
}

void Typer_app::update_stats_box() {
    Typing_test_engine::Stats stats{engine_.get_stats()};
    std::stringstream ss;

    // WPM
    ss << stats.wpm;
    top_bar.stats_box.wpm_stat.value.set_text(ss.str());

    // Missed
    ss.str("");
    ss << stats.missed_keystrokes;
    top_bar.stats_box.missed_stat.value.set_text(ss.str());

    // Accuracy
    ss.str("");
    double percent{0.0};
    if (stats.correct_keystrokes != 0) {
        percent = (static_cast<double>(stats.correct_keystrokes) * 100) /
                  (stats.correct_keystrokes + stats.missed_keystrokes);
    }
    ss << std::fixed << std::setprecision(1) << percent << '%';
    top_bar.stats_box.accuracy_stat.value.set_text(ss.str());
}

}  // namespace ui
}  // namespace typer
