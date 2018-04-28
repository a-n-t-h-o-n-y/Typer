#include <thread>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <typer_logic.hpp>

TEST_CASE("Validation", "[Typer_logic]") {
    typer::Typer_logic tl;
    tl.set_text("Test ");
    const int word_length{5};

    REQUIRE(tl.validate_next_letter('T'));
    REQUIRE(!tl.validate_next_letter('T'));
    REQUIRE(tl.validate_next_letter('e'));
    REQUIRE(tl.validate_next_letter('s'));
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(2s);
    REQUIRE(tl.validate_next_letter('t'));
    REQUIRE(tl.validate_next_letter(' '));
    REQUIRE(!tl.validate_next_letter('a'));

    typer::Typer_logic::Stats stats{tl.get_stats()};
    REQUIRE(stats.missed == 1);
    REQUIRE(stats.total == 5);
    REQUIRE(stats.wpm == (5.0 / word_length) / (2.0 / 60));

    tl.set_text("Two");
    REQUIRE(tl.validate_next_letter('T'));
    REQUIRE(tl.validate_next_letter('w'));
    std::this_thread::sleep_for(2s);
    REQUIRE(tl.validate_next_letter('o'));
    REQUIRE(!tl.validate_next_letter('o'));

    stats = tl.get_stats();
    REQUIRE(stats.missed == 0);
    REQUIRE(stats.total == 3);
    REQUIRE(stats.wpm == (3.0 / word_length) / (2.0 / 60));

    tl.reset();
    stats = tl.get_stats();
    REQUIRE(stats.missed == 0);
    REQUIRE(stats.total == 0);
    REQUIRE(stats.wpm == 0);
}
