#include <thread>

#include "catch.hpp"

#include <typing_test_engine.hpp>

TEST_CASE("Validation", "[Typing_test_engine]")
{
    using namespace std::chrono_literals;

    const int word_length{5};
    typer::Typing_test_engine engine;

    {
        engine.set_text("Test ");
        CHECK(engine.commit_keystroke('T').first);
        CHECK(!engine.commit_keystroke('T').first);
        CHECK(engine.commit_keystroke('e').first);
        CHECK(engine.commit_keystroke('s').first);
        std::this_thread::sleep_for(2s);
        CHECK(engine.commit_keystroke('t').first);
        CHECK(engine.commit_keystroke(' ').first);
        auto const [b, stats] = engine.commit_keystroke('a');
        CHECK(!b);

        CHECK(stats.missed_keystrokes == 1);
        CHECK(stats.correct_keystrokes == 5);
        CHECK(stats.wpm == static_cast<int>((5.0 / word_length) / (2.0 / 60)));
    }

    {
        engine.set_text("Two");
        CHECK(engine.commit_keystroke('T').first);
        CHECK(engine.commit_keystroke('w').first);
        std::this_thread::sleep_for(2s);
        CHECK(engine.commit_keystroke('o').first);
        auto const [b, stats] = engine.commit_keystroke('o');
        CHECK(!b);

        CHECK(stats.missed_keystrokes == 0);
        CHECK(stats.correct_keystrokes == 3);
        CHECK(stats.wpm == static_cast<int>((3.0 / word_length) / (2.0 / 60)));
    }

    {
        auto const stats = engine.reset();
        CHECK(stats.missed_keystrokes == 0);
        CHECK(stats.correct_keystrokes == 0);
        CHECK(stats.wpm == 0);
    }
}
