#include "window.hpp"

namespace kat {

    Window::Window(const WindowSettings &settings) {
        glfwInit();

        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, false);

        m_Window = glfwCreateWindow(static_cast<int>(settings.size.width), static_cast<int>(settings.size.height),
                                    settings.title.c_str(), nullptr, nullptr);

        glfwSetWindowUserPointer(m_Window, this);
    }

    Window::~Window() {
        glfwDestroyWindow(m_Window);
    }

    bool Window::isOpen() const {
        return !glfwWindowShouldClose(m_Window);
    }

    vk::SurfaceKHR Window::createSurface(vk::Instance instance) const {
        VkSurfaceKHR s;
        glfwCreateWindowSurface(instance, m_Window, nullptr, &s);
        return s;
    }

    void Window::pollEvents() {
        glfwPollEvents();
    }

} // namespace kat
