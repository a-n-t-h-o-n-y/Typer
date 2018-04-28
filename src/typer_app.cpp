#include <ui/typer_app.hpp>

#include <cstddef>
#include <sstream>
#include <string>

namespace typer {
namespace ui {

Typer_app::Typer_app() {
    top_bar.options_box.change_text_btn.clicked.connect([this] {
        // if Typer_widget is currently displayed.
        if (typing_stack.active_page_index() == 0) {
            typing_stack.set_active_page(1);
        } else {  // Set_text_widget is currently displayed.
            typing_stack.set_active_page(0);
        }
    });

    typing_stack.typer_widget.scrolled_down.connect([this](auto amount) {
        // find new start of text
        std::string sub_string{typing_stack.set_text_widget.contents().str()};
        sub_string =
            sub_string.substr(typing_stack.typer_widget.index_at(0, 0));
        logic_.set_text(sub_string);
    });

    typing_stack.page_changed.connect([this](std::size_t index) {
        if (index != 0) {
            return;
        }  // Else page changed to Typer_widget
        std::string new_text{typing_stack.set_text_widget.contents().str()};
        this->set_text_in_logic_and_typer_display(new_text);
        typing_stack.typer_widget.set_cursor(0);  // typer_widget.reset();
    });

    typing_stack.typer_widget.key_pressed.connect([this](char c) {
        bool correct{logic_.validate_next_letter(c)};
        this->update_stats_box();
        return correct;
    });
}

void Typer_app::set_text_in_logic_and_typer_display(const std::string& text) {
    logic_.set_text(text);
    typing_stack.typer_widget.set_text("");  // hack to scroll to top
    typing_stack.typer_widget.set_text(text);
}

void Typer_app::update_stats_box() {
    Typer_logic::Stats stats{logic_.get_stats()};
    std::stringstream ss;
    // WPM
    ss << stats.wpm;
    std::string wpm{ss.str()};
    top_bar.stats_box.wpm_stat.value.set_text(wpm);

    // Missed
    ss.str("");
    ss << stats.missed;
    std::string missed{ss.str()};
    top_bar.stats_box.missed_stat.value.set_text(missed);

    // Total
    ss.str("");
    ss << stats.total;
    std::string total{ss.str()};
    top_bar.stats_box.total_stat.value.set_text(total);

    // Accuracy
    ss.str("");
    double percentage{
        (static_cast<double>(stats.total - stats.missed) / stats.total) * 100};
    if (stats.total == 0) {
        percentage = 0;
    }
    // if (percentage < 0) {
    //     percentage = 0;
    // }
    ss << percentage << '%';
    std::string accuracy{ss.str()};
    top_bar.stats_box.accuracy_stat.value.set_text(accuracy);
}

}  // namespace ui
}  // namespace typer
