#ifndef TYPER_TYPING_TEST_ENGINE_HPP
#define TYPER_TYPING_TEST_ENGINE_HPP
#include <chrono>
#include <string>

namespace typer {

/// Responsible for holding a body of text to be typed through, validating each
/// keystroke and updating statistics.
class Typing_test_engine {
   public:
    /// Holds live statistics of the current typing test run.
    struct Stats {
        int wpm{0};
        int missed_keystrokes{0};
        int correct_keystrokes{0};
    };

    /// Sets the text that will be used in the test, resets Stats.
    void set_text(const std::string& text);

    /// Matches the given char against the current letter, updates Stats.
    bool commit_keystroke(char c);

    /// Reset the current index and stats. Does not clear the text body.
    void reset();

    /// Query if the entire text has been typed.
    bool is_complete() const;

    /// Retrieve the text body.
    const std::string& get_text() const;

    /// Return the Stats object(wpm, missed, total).
    Stats get_stats() const;

    /// Set the average length of a word in chars.
    void set_word_length(int length);

    /// Retrieve the set average length of a word.
    int get_word_length() const;

   private:
    using Clock_t = std::chrono::steady_clock;

    std::string text_body_;
    int word_length_{5};
    Stats stats_;
    Clock_t::time_point initial_time_;

    /// Returns whether \p c is at the current index of text_body_.
    bool verify_keystroke(char c) const;

    /// Updates the stats_ object.
    void update_state(bool was_accepted);

    /// Updates the words per minute in stats_ according to the current time.
    void update_wpm();

    /// Update the correct/missed keystroke counts in stats_.
    void update_keystroke_counts(bool was_accepted);
};

}  // namespace typer
#endif  // TYPER_TYPING_TEST_ENGINE_HPP
