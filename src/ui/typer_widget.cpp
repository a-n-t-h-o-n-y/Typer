#include "typer_widget.hpp"

#include <algorithm>
#include <iterator>

#include <termox/termox.hpp>

namespace {

/// Insert \p to_insert before every occurance of \p to_find.
template <typename Container_t, typename Value_t>
void insert_before_each(Container_t& container,
                        Value_t const& to_insert,
                        Value_t const& to_find)
{
    auto iter = std::find(std::begin(container), std::end(container), to_find);
    while (iter != std::end(container)) {
        iter = container.insert(iter, to_insert);
        std::advance(iter, 2);
        iter = std::find(iter, std::end(container), to_find);
    }
}

}  // namespace

namespace typer::ui {

void Carriage_textbox::set_contents(ox::Glyph_string text)
{
    original_contents_ = text.u32str();
    insert_before_each(text, CR, U'\n');
    Textbox_base::set_contents(std::move(text));
}

}  // namespace typer::ui
