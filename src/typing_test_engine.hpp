#ifndef TYPER_TYPING_TEST_ENGINE_HPP
#define TYPER_TYPING_TEST_ENGINE_HPP
#include <cctype>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <ratio>
#include <string>

namespace typer {

/// Responsible for holding a body of text to be typed through, validating each
/// keystroke and updating statistics.
class Typing_test_engine {
   public:
    /// Holds live statistics of the current typing test run.
    struct Stats {
        std::size_t wpm{0};
        std::size_t missed_keystrokes{0};
        std::size_t correct_keystrokes{0};
    };

   public:
    /// Sets the text that will be used in the test, resets Stats.
    void set_text(std::string const& text)
    {
        this->reset();
        text_body_ = text;
    }

    /// Retrieve the text body.
    [[nodiscard]] auto get_text() const -> std::string const&
    {
        return text_body_;
    }

    /// Matches the given char against the current letter, updates Stats.
    auto commit_keystroke(char c) -> bool
    {
        if (text_body_.empty() || this->is_complete())
            return false;
        auto const is_correct = verify_keystroke(c);
        update_state(is_correct);
        return is_correct;
    }

    /// Reset the current index and stats. Does not clear the text body.
    void reset() { stats_ = Stats{}; }

    /// Query if the entire text has been typed.
    [[nodiscard]] auto is_complete() const -> bool
    {
        return stats_.correct_keystrokes >= text_body_.size();
    }

    /// Return the Stats object(wpm, missed, total).
    [[nodiscard]] auto get_stats() const -> Stats { return stats_; }

    /// Set the average length of a word in chars.
    void set_word_length(std::size_t length)
    {
        if (length != 0uL)
            word_length_ = length;
    }

    /// Retrieve the set average length of a word.
    [[nodiscard]] auto get_word_length() const -> std::size_t
    {
        return word_length_;
    }

   private:
    using Clock_t = std::chrono::steady_clock;

    std::string text_body_;
    std::size_t word_length_{5uL};
    Stats stats_;
    Clock_t::time_point initial_time_;

   private:
    /// Returns whether \p c is at the current index of text_body_.
    [[nodiscard]] auto verify_keystroke(char c) const -> bool
    {
        return (std::isprint(c) || std::isspace(c)) &&
               c == text_body_.at(stats_.correct_keystrokes);
    }

    /// Updates the stats_ object.
    void update_state(bool was_accepted)
    {
        this->update_keystroke_counts(was_accepted);
        stats_.wpm = this->get_wpm();
    }

    /// Updates the words per minute in stats_ according to the current time.
    auto get_wpm() -> std::size_t
    {
        using Minute_t = std::chrono::duration<double, std::ratio<60>>;
        using std::chrono::duration_cast;

        if (this->at_initial_keystroke()) {
            initial_time_ = Clock_t::now();
            return 0uL;
        }
        else {
            auto const word_count =
                static_cast<double>(stats_.correct_keystrokes) / word_length_;
            auto const time_elapsed = Clock_t::now() - initial_time_;
            auto const mins = duration_cast<Minute_t>(time_elapsed).count();
            return std::round(word_count / mins);
        }
    }

    /// Update the correct/missed keystroke counts in stats_.
    void update_keystroke_counts(bool was_accepted)
    {
        if (was_accepted)
            ++stats_.correct_keystrokes;
        else
            ++stats_.missed_keystrokes;
    }

    [[nodiscard]] auto at_initial_keystroke() const -> bool
    {
        return stats_.correct_keystrokes + stats_.missed_keystrokes == 1uL;
    }
};

}  // namespace typer
#endif  // TYPER_TYPING_TEST_ENGINE_HPP
