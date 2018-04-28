#include <ui/set_text_widget.hpp>

namespace typer {
namespace ui {

Set_text_widget::Set_text_widget() : cppurses::Textbox{"Set Text Here"} {
    enable_border(*this);
}

}  // namespace ui
}  // namespace typer
