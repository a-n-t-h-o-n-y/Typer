#include <ui/stats_box.hpp>

#include <algorithm>
#include <cstddef>

#include <cppurses/widget/widget_free_functions.hpp>

namespace typer {
namespace ui {

Stat::Stat(const std::string& value_, const std::string& label_)
    : value{this->make_child<cppurses::Text_display>(value_)},
      label{this->make_child<cppurses::Label>(label_)} {
    using namespace cppurses;
    value.set_alignment(Alignment::Right);
    value.width_policy.type(Size_policy::Preferred);
    value.width_policy.hint(std::max<std::size_t>(3, value_.length()));

    label.width_policy.type(Size_policy::Preferred);
    label.width_policy.hint(label_.length() + 1);
    label.disable_word_wrap();

    space.width_policy.type(Size_policy::Expanding);
}

Stats_box::Stats_box() {
    using namespace cppurses;
    set_background_recursive(*this, Color::White);
    set_foreground_recursive(*this, Color::Black);
    accuracy_stat.space.width_policy.type(Size_policy::Fixed);
    accuracy_stat.space.width_policy.hint(0);
}

}  // namespace ui
}  // namespace typer
