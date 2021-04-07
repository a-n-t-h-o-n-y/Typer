#ifndef TYPER_TYPING_TEST_ENGINE_HPP
#define TYPER_TYPING_TEST_ENGINE_HPP
#include <cctype>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <optional>
#include <ratio>
#include <string>
#include <utility>

namespace typer {

/// Responsible for holding a body of text to be typed through.
/** Validates each keystroke and updates statistics. */
class Typing_test_engine {
   public:
    /// Holds live statistics of the current typing test run.
    struct Stats {
        int wpm                = 0;
        int missed_keystrokes  = 0;
        int correct_keystrokes = 0;
    };

   public:
    /// Sets the text that will be used in the test, resets Stats.
    /** Returns a default constructed Stats object. */
    auto set_text(std::string const& text) -> Stats
    {
        this->reset();
        text_ = text;
        return stats_;
    }

    /// Retrieve the text body.
    [[nodiscard]] auto get_text() const -> std::string const& { return text_; }

    /// Matches the given char against the current letter, updates Stats.
    [[nodiscard]] auto commit_keystroke(char c) -> std::pair<bool, Stats>
    {
        if (text_.empty() || this->is_complete())
            return {false, stats_};
        auto const is_correct = verify_keystroke(c);
        auto elapsed          = std::optional<Clock_t::duration>{std::nullopt};
        if (this->at_initial_keystroke())
            initial_time_ = Clock_t::now();
        else
            elapsed = Clock_t::now() - initial_time_;
        stats_ = increment_stats(stats_, is_correct, elapsed);
        return {is_correct, stats_};
    }

    /// Reset the current index and stats. Does not clear the text body.
    auto reset() -> Stats
    {
        stats_ = Stats{};
        return stats_;
    }

    /// Query if the entire text has been typed.
    [[nodiscard]] auto is_complete() const -> bool
    {
        return stats_.correct_keystrokes >= (int)text_.size();
    }

   private:
    using Clock_t = std::chrono::steady_clock;

    std::string text_;
    Stats stats_;
    Clock_t::time_point initial_time_;

   private:
    /// Returns whether \p c is at the current index of text_.
    [[nodiscard]] auto verify_keystroke(char c) const -> bool
    {
        return (std::isprint(c) || std::isspace(c)) &&
               c == text_.at(stats_.correct_keystrokes);
    }

    /// Returns the words per minute according to the given parameters.
    [[nodiscard]] static auto generate_wpm(Clock_t::duration elapsed,
                                           double correct_count) -> int
    {
        using Minute_t = std::chrono::duration<double, std::ratio<60>>;
        using std::chrono::duration_cast;
        auto constexpr word_length = 5;

        auto const word_count = correct_count / word_length;
        auto const mins       = duration_cast<Minute_t>(elapsed).count();
        return std::round(word_count / mins);
    }

    /// Return true if no keys have been pressed since a reset.
    [[nodiscard]] auto at_initial_keystroke() const -> bool
    {
        return (stats_.correct_keystrokes + stats_.missed_keystrokes) == 1;
    }

    /// Takes in the current stats and updated depending on \p correct_key.
    /** Uses \p elapsed to calculate WPM. */
    [[nodiscard]] static auto increment_stats(
        Stats current,
        bool is_correct,
        std::optional<Clock_t::duration> elapsed) -> Stats
    {
        ++(is_correct ? current.correct_keystrokes : current.missed_keystrokes);
        if (elapsed.has_value())
            current.wpm = generate_wpm(*elapsed, current.correct_keystrokes);
        return current;
    }
};

}  // namespace typer
#endif  // TYPER_TYPING_TEST_ENGINE_HPP
