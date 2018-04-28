#include <ui/stats_box.hpp>

namespace typer {
namespace ui {

Stat::Stat(const std::string& label, const std::string& value)
    : label{this->make_child<cppurses::Label>(label)},
      value{this->make_child<cppurses::Text_display>(value)} {}

Stats_box::Stats_box() {
    enable_border(*this);
}

}  // namespace ui
}  // namespace typer
