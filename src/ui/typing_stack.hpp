#ifndef TYPER_UI_TYPING_STACK_HPP
#define TYPER_UI_TYPING_STACK_HPP
#include <termox/termox.hpp>

#include "set_text_pane.hpp"
#include "typer_widget.hpp"

namespace typer::ui {

struct Typing_stack_widgets : ox::SPair<Typing_window, Set_text_pane> {
   public:
    Typing_window& typing_window   = this->first;
    Set_text_pane& set_text_widget = this->second;

   protected:
    static auto constexpr typing_window_index = 0uL;
    static auto constexpr set_text_index      = 1uL;
};

/// This is the main text input and typing space. Does not include the Stats.
struct Typing_stack : Typing_stack_widgets {
   public:
    sl::Signal<void(std::u32string const&)> text_set;

   public:
    Typing_stack()
    {
        typing_window.focused_in.connect(
            [this] { ox::System::set_focus(typing_window.typer_widget); });
        set_text_widget.focused_in.connect(
            [this] { ox::System::set_focus(set_text_widget.textbox); });

        this->set_active_page(typing_window_index);
        this->page_changed.connect([this](auto index) {
            if (index == typing_window_index)
                text_set.emit(set_text_widget.textbox.contents().u32str());
        });
    }

   public:
    /// Toggle between the set_text and typing window pages.
    void toggle_page()
    {
        auto const index = this->active_page_index() == typing_window_index
                               ? set_text_index
                               : typing_window_index;
        this->set_active_page(index);
    }
};

}  // namespace typer::ui
#endif  // TYPER_UI_TYPING_STACK_HPP
