#include "app-window.h"

#include <cctype>
#include <string>

namespace {

char rotate_alpha(char ch, char base, int shift)
{
    return static_cast<char>(base + ((ch - base + shift + 26) % 26));
}

slint::SharedString tabula_transform(const slint::SharedString &text, int direction)
{
    std::string transformed(text);
    int key_index = 0;

    for (char &ch : transformed) {
        const auto uch = static_cast<unsigned char>(ch);
        const int shift = direction * (key_index % 26);

        if (std::isupper(uch)) {
            ch = rotate_alpha(ch, 'A', shift);
            ++key_index;
        } else if (std::islower(uch)) {
            ch = rotate_alpha(ch, 'a', shift);
            ++key_index;
        }
    }

    return slint::SharedString(transformed);
}

slint::SharedString tabula_encode(const slint::SharedString &text)
{
    return tabula_transform(text, 1);
}

slint::SharedString tabula_decode(const slint::SharedString &text)
{
    return tabula_transform(text, -1);
}

} // namespace

int main()
{
    auto app = AppWindow::create();
    slint::ComponentWeakHandle<AppWindow> weak_app(app);

    app->on_encode_text([weak_app] {
        if (auto locked_app = weak_app.lock()) {
            (*locked_app)->set_output_text(tabula_encode((*locked_app)->get_input_text()));
        }
    });

    app->on_decode_text([weak_app] {
        if (auto locked_app = weak_app.lock()) {
            (*locked_app)->set_output_text(tabula_decode((*locked_app)->get_input_text()));
        }
    });

    app->run();
    return 0;
}
