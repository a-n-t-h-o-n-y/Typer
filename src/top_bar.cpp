#include <ui/top_bar.hpp>

#include <cppurses/widget/size_policy.hpp>

namespace typer {
namespace ui {

Top_bar::Top_bar() {
    this->height_policy.type(cppurses::Size_policy::Fixed);
    this->height_policy.hint(6);
}

}  // namespace ui
}  // namespace typer
