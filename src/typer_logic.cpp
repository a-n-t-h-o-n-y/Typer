#include <typer_logic.hpp>

namespace typer {

void Typer_logic::set_text(std::string text) {
    this->reset();
    if (text.empty()) {
        text = " ";
    }
    text_ = std::move(text);
    complete_ = false;
}

std::string Typer_logic::get_text() const {
    return text_;
}

std::string Typer_logic::get_text_from_current_index() const {
    return text_.substr(current_index_);
}

bool Typer_logic::validate_next_letter(char c) {
    if (current_index_ == text_.size()) {
        complete_ = true;
        return false;
    }
    if (current_index_ == 0) {
        initial_time_ = Clock::now();
    }
    // Correct
    if (c == text_[current_index_]) {
        already_missed_ = false;
        ++current_index_;
        ++stats_.total;
        this->update_wpm();
        return true;
    }
    // Incorrect
    if (!already_missed_) {
        ++stats_.missed;
        already_missed_ = true;
    }
    this->update_wpm();
    return false;
}

void Typer_logic::reset() {
    already_missed_ = false;
    complete_ = false;
    current_index_ = 0;
    stats_ = Stats();
}

bool Typer_logic::complete() const {
    return complete_;
}

Typer_logic::Stats Typer_logic::get_stats() const {
    return stats_;
}

void Typer_logic::set_word_length(int length) {
    word_length_ = length;
}

int Typer_logic::get_word_length() const {
    return word_length_;
}

void Typer_logic::update_wpm() {
    double words{static_cast<double>(stats_.total) / word_length_};
    double ms_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                            Clock::now() - initial_time_)
                            .count();
    const double ms_to_min{1.0 / 60'000};
    double min = ms_elapsed * ms_to_min;
    stats_.wpm = words / min;
}

}  // namespace typer
