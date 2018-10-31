#ifndef TYPER_TYPER_LOGIC_HPP
#define TYPER_TYPER_LOGIC_HPP
#include <chrono>
#include <cstddef>
#include <string>

namespace typer {

/// Represents a piece of text to be typed through, calculates wpm and accuracy.
/// Calculates wpm from first char validation
class Typer_logic {
   public:
    /// Contains words per minute from current text, total chars and missed.
    struct Stats {
        int wpm{0};
        std::size_t missed{0};
        std::size_t total{0};
    };

    /// Set the current text to work off of.
    void set_text(std::string text);

    /// Matches the given char against the current letter, updates state.
    bool validate_next_letter(char c);

    /// Reset the current index and stats.
    void reset();

    /// Query if the entire text has been typed.
    bool complete() const;

    /// Retrieve the entire text.
    std::string get_text() const;

    /// Retrieve the text starting from the current index.
    std::string get_text_from_current_index() const;

    /// Return a Stats object(wpm, missed, total)
    Stats get_stats() const;

    /// Set the average length of a word in chars.
    void set_word_length(int length);

    /// Retrieve the set average length of a word.
    int get_word_length() const;

   private:
    using Clock = std::chrono::steady_clock;

    std::string text_{" "};
    std::size_t current_index_{0};
    Clock::time_point initial_time_;
    Stats stats_;
    int word_length_{5};
    bool already_missed_{false};
    bool complete_{true};

    void update_wpm();
};

}  // namespace typer
#endif  // TYPER_TYPER_LOGIC_HPP
