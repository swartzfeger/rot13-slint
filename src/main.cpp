#include "app-window.h"

#include <cctype>
#include <string>

namespace {

char rotate_alpha(char ch, char base)
{
    return static_cast<char>(base + ((ch - base + 13) % 26));
}

slint::SharedString rot13(const slint::SharedString &text)
{
    std::string transformed(text);

    for (char &ch : transformed) {
        const auto uch = static_cast<unsigned char>(ch);
        if (std::isupper(uch)) {
            ch = rotate_alpha(ch, 'A');
        } else if (std::islower(uch)) {
            ch = rotate_alpha(ch, 'a');
        }
    }

    return slint::SharedString(transformed);
}

} // namespace

int main()
{
    auto app = AppWindow::create();
    slint::ComponentWeakHandle<AppWindow> weak_app(app);

    app->on_encode_decode([weak_app] {
        if (auto locked_app = weak_app.lock()) {
            (*locked_app)->set_output_text(rot13((*locked_app)->get_input_text()));
        }
    });

    app->run();
    return 0;
}
