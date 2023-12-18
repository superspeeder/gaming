#include <iostream>
#include <memory>


#include "kat/context.hpp"
#include "kat/window.hpp"

int main() {
    auto window = std::make_shared<kat::Window>(kat::WindowSettings{
        .size  = { 800, 600 },
        .title = "Hello!",
    });

    auto context = std::make_shared<kat::Context>(window, kat::ContextSettings{});

    while (window->isOpen()) {
        kat::Window::pollEvents();
    }

    return 0;
}
