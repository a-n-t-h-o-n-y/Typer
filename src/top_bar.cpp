#include <ui/top_bar.hpp>

#include <cppurses/painter/color.hpp>
#include <cppurses/widget/size_policy.hpp>
#include <cppurses/widget/widget_free_functions.hpp>

namespace typer {
namespace ui {

Top_bar::Top_bar() {
    using namespace cppurses;
    this->height_policy.type(Size_policy::Fixed);
    this->height_policy.hint(1);

    stats_box.width_policy.type(Size_policy::Expanding);

    set_background(set_text_btn, Color::Light_green);
    set_foreground(set_text_btn, Color::Black);
    set_text_btn.width_policy.type(Size_policy::Preferred);
    set_text_btn.width_policy.hint(10);
}

}  // namespace ui
}  // namespace typer
