#ifndef TYPER_UI_TYPER_APP_HPP
#define TYPER_UI_TYPER_APP_HPP
#include <string>

#include <cppurses/widget/layouts/vertical.hpp>

#include <typing_test_engine.hpp>
#include <ui/top_bar.hpp>
#include <ui/typing_stack.hpp>

namespace typer {
namespace ui {

struct Typer_app : public cppurses::layout::Vertical {
    Typer_app();

    Top_bar& top_bar{this->make_child<Top_bar>()};
    Typing_stack& typing_stack{this->make_child<Typing_stack>()};

   private:
    Typing_test_engine engine_;

    void update_stats_box();
    void init_typing_window();
    auto setup_next_chunk();
};

}  // namespace ui
}  // namespace typer
#endif  // TYPER_UI_TYPER_APP_HPP
