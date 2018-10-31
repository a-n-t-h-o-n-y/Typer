#include <thread>

#include "catch.hpp"

#include <typing_test_engine.hpp>

TEST_CASE("Validation", "[Typing_test_engine]") {
    using namespace std::chrono_literals;

    const int word_length{5};
    typer::Typing_test_engine engine;

    engine.set_text("Test ");
    CHECK(engine.commit_keystroke('T'));
    CHECK(!engine.commit_keystroke('T'));
    CHECK(engine.commit_keystroke('e'));
    CHECK(engine.commit_keystroke('s'));
    std::this_thread::sleep_for(2s);
    CHECK(engine.commit_keystroke('t'));
    CHECK(engine.commit_keystroke(' '));
    CHECK(!engine.commit_keystroke('a'));

    typer::Typing_test_engine::Stats stats{engine.get_stats()};
    CHECK(stats.missed_keystrokes == 1);
    CHECK(stats.correct_keystrokes == 5);
    CHECK(stats.wpm == static_cast<int>((5.0 / word_length) / (2.0 / 60)));

    engine.set_text("Two");
    CHECK(engine.commit_keystroke('T'));
    CHECK(engine.commit_keystroke('w'));
    std::this_thread::sleep_for(2s);
    CHECK(engine.commit_keystroke('o'));
    CHECK(!engine.commit_keystroke('o'));

    stats = engine.get_stats();
    CHECK(stats.missed_keystrokes == 0);
    CHECK(stats.correct_keystrokes == 3);
    CHECK(stats.wpm == static_cast<int>((3.0 / word_length) / (2.0 / 60)));

    engine.reset();
    stats = engine.get_stats();
    CHECK(stats.missed_keystrokes == 0);
    CHECK(stats.correct_keystrokes == 0);
    CHECK(stats.wpm == 0);
}
