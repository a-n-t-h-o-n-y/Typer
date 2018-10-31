#include <typing_test_engine.hpp>

#include <cctype>
#include <chrono>
#include <cmath>
#include <ratio>
#include <string>

namespace typer {

void Typing_test_engine::set_text(const std::string& text) {
    this->reset();
    text_body_ = text;
}

const std::string& Typing_test_engine::get_text() const {
    return text_body_;
}

bool Typing_test_engine::commit_keystroke(char c) {
    if (text_body_.empty() || this->is_complete()) {
        return false;
    }
    bool was_accepted = verify_keystroke(c);
    update_state(was_accepted);
    return was_accepted;
}

void Typing_test_engine::reset() {
    stats_ = Stats();
}

bool Typing_test_engine::is_complete() const {
    return stats_.correct_keystrokes >= text_body_.size();
}

Typing_test_engine::Stats Typing_test_engine::get_stats() const {
    return stats_;
}

void Typing_test_engine::set_word_length(int length) {
    if (length > 0) {
        word_length_ = length;
    }
}

int Typing_test_engine::get_word_length() const {
    return word_length_;
}

bool Typing_test_engine::verify_keystroke(char c) const {
    return (std::isprint(c) || std::isspace(c)) &&
           c == text_body_.at(stats_.correct_keystrokes);
}

void Typing_test_engine::update_state(bool was_accepted) {
    this->update_keystroke_counts(was_accepted);
    this->update_wpm();
}

void Typing_test_engine::update_wpm() {
    using Minute_dbl_t = std::chrono::duration<double, std::ratio<60>>;
    using std::chrono::duration_cast;
    if (stats_.correct_keystrokes + stats_.missed_keystrokes == 1) {
        initial_time_ = Clock_t::now();
        return;
    }
    const double word_count{static_cast<double>(stats_.correct_keystrokes) /
                            word_length_};
    const Clock_t::duration time_elapsed_ = Clock_t::now() - initial_time_;
    const double minutes = duration_cast<Minute_dbl_t>(time_elapsed_).count();
    stats_.wpm = static_cast<int>(std::round(word_count / minutes));
}

void Typing_test_engine::update_keystroke_counts(bool was_accepted) {
    if (was_accepted) {
        ++stats_.correct_keystrokes;
    } else {
        ++stats_.missed_keystrokes;
    }
}

}  // namespace typer
