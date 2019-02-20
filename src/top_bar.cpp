#include <ui/top_bar.hpp>

#include <cppurses/painter/color.hpp>
#include <cppurses/widget/size_policy.hpp>

using namespace cppurses;

namespace typer {
namespace ui {

Top_bar::Top_bar() {
    this->height_policy.fixed(1);
    stats_box.width_policy.type(Size_policy::Expanding);
    set_text_btn.width_policy.preferred(10);
    set_text_btn.brush.set_background(Color::Light_green);
    set_text_btn.brush.set_foreground(Color::Black);
}
}  // namespace ui
}  // namespace typer
