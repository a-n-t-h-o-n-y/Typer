#include <ui/typer_app.hpp>

#include <cppurses/system/focus.hpp>

#include <cstddef>
#include <iomanip>
#include <sstream>
#include <string>

namespace typer {
namespace ui {

Typer_app::Typer_app() {
    top_bar.set_text_btn.clicked.connect([this] {
        // if Typer_widget is currently displayed.
        if (typing_stack.active_page_index() == 0) {
            typing_stack.set_active_page(1);
        } else {  // Set_text_widget is currently displayed.
            typing_stack.set_active_page(0);
        }
    });

    typing_stack.main_window.typer_widget.scrolled_down.connect(
        [this](auto amount) {
            // find new start of text
            std::string sub_string{
                typing_stack.set_text_widget.textbox.contents().str()};
            sub_string = sub_string.substr(
                typing_stack.main_window.typer_widget.index_at(0, 0));
            engine_.set_text(sub_string);
        });

    typing_stack.page_changed.connect([this](std::size_t index) {
        if (index != 0) {
            return;
        }  // Else page changed to Typer_widget
        this->reset_main_window();
    });

    typing_stack.main_window.typer_widget.key_pressed.connect([this](char c) {
        bool correct{engine_.commit_keystroke(c)};
        this->update_stats_box();
        return correct;
    });

    typing_stack.main_window.load_file.open_requested.connect(
        [this](std::ifstream& ifs) {
            std::string contents{std::istreambuf_iterator<char>(ifs),
                                 std::istreambuf_iterator<char>()};
            typing_stack.set_text_widget.textbox.set_text(contents);
            this->reset_main_window();
        });
}

void Typer_app::reset_main_window() {
    std::string new_text{typing_stack.set_text_widget.textbox.contents().str()};
    this->set_text_in_engine_and_typer_display(new_text);
    typing_stack.main_window.typer_widget.set_cursor(0);
    cppurses::Focus::set_focus_to(&typing_stack.main_window.typer_widget);
}

void Typer_app::set_text_in_engine_and_typer_display(const std::string& text) {
    engine_.set_text(text);
    typing_stack.main_window.typer_widget.set_text("");  // hack to scroll up
    typing_stack.main_window.typer_widget.set_text(text);
}

void Typer_app::update_stats_box() {
    Typing_test_engine::Stats stats{engine_.get_stats()};
    std::stringstream ss;
    // WPM
    ss << stats.wpm;
    std::string wpm{ss.str()};
    top_bar.stats_box.wpm_stat.value.set_text(wpm);

    // Missed
    ss.str("");
    ss << stats.missed_keystrokes;
    std::string missed{ss.str()};
    top_bar.stats_box.missed_stat.value.set_text(missed);

    // Accuracy
    ss.str("");
    double percent{0.0};
    if (stats.correct_keystrokes != 0) {
        percent = (static_cast<double>(stats.correct_keystrokes) * 100) /
                  (stats.correct_keystrokes + stats.missed_keystrokes);
    }
    ss << std::fixed << std::setprecision(1) << percent << '%';
    std::string accuracy{ss.str()};
    top_bar.stats_box.accuracy_stat.value.set_text(accuracy);
}

}  // namespace ui
}  // namespace typer
