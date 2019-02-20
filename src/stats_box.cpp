#include <ui/stats_box.hpp>

#include <cppurses/painter/color.hpp>
#include <cppurses/widget/size_policy.hpp>
#include <cppurses/widget/widgets/label.hpp>
#include <cppurses/widget/widgets/text_display.hpp>

#include <algorithm>
#include <cstddef>

using namespace cppurses;

namespace typer {
namespace ui {

Stat::Stat(const std::string& value_, const std::string& label_)
    : value{this->make_child<Text_display>(value_)},
      label{this->make_child<Label>(label_)} {
    value.set_alignment(Alignment::Right);
    value.width_policy.preferred(std::max<std::size_t>(3, value_.length()));
    label.width_policy.preferred(label_.length() + 1);
    label.disable_word_wrap();
    space.width_policy.type(Size_policy::Expanding);
}

Stats_box::Stats_box() {
    for (const auto& child : this->children.get_descendants()) {
        child->brush.set_background(Color::White);
        child->brush.set_foreground(Color::Black);
    }
    accuracy_stat.space.width_policy.fixed(0);
}
}  // namespace ui
}  // namespace typer
