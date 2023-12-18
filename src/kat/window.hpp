#pragma once

#include <string>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include "kat/types.hpp"

namespace kat {

    struct WindowSettings {
        Size2d      size;
        std::string title;
    };

    class Window {
      public:
        explicit Window(const WindowSettings &settings);

        ~Window();

        [[nodiscard]] bool isOpen() const;
        vk::SurfaceKHR     createSurface(vk::Instance instance) const;

        static void pollEvents();

      private:
        GLFWwindow *m_Window;
    };

} // namespace kat
