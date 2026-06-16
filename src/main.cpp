#include "app-window.h"

#include <cctype>
#include <cstdio>
#include <cstdlib>
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

void copy_to_clipboard(const slint::SharedString &text)
{
#ifdef _WIN32
    FILE *clipboard = _popen("clip", "w");
#else
    FILE *clipboard = popen("pbcopy", "w");
#endif

    if (!clipboard) {
        return;
    }

    const std::string text_to_copy(text);
    fwrite(text_to_copy.data(), sizeof(char), text_to_copy.size(), clipboard);

#ifdef _WIN32
    _pclose(clipboard);
#else
    pclose(clipboard);
#endif
}

void configure_backend()
{
#ifdef _WIN32
    if (std::getenv("SLINT_BACKEND") == nullptr) {
        _putenv_s("SLINT_BACKEND", "winit-software");
    }
#endif
}

} // namespace

int main()
{
    configure_backend();

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

    app->on_copy_input([weak_app] {
        if (auto locked_app = weak_app.lock()) {
            copy_to_clipboard((*locked_app)->get_input_text());
        }
    });

    app->on_copy_output([weak_app] {
        if (auto locked_app = weak_app.lock()) {
            copy_to_clipboard((*locked_app)->get_output_text());
        }
    });

    app->run();
    return 0;
}
