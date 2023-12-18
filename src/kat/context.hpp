#pragma once
#include <memory>

#include <vulkan/vulkan.hpp>

#include "window.hpp"

namespace kat {

    class missing_queues_error final : public std::exception {};

    struct ContextSettings {};

    struct QueueSupport {
        uint32_t graphics, present, transfer;

        [[nodiscard]] static QueueSupport query(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);

        [[nodiscard]] bool                     isComplete() const;
        std::vector<vk::DeviceQueueCreateInfo> generateCreateInfos() const;
    };

    struct Queues {
        vk::Queue graphics, present, transfer;

        [[nodiscard]] static Queues query(vk::Device device, const QueueSupport &queueSupport);
    };

    class Context {
      public:
         Context(const std::shared_ptr<Window> &window, const ContextSettings &settings);
        ~Context();


        [[nodiscard]] vk::Instance       instance() const;
        [[nodiscard]] vk::PhysicalDevice physicalDevice() const;
        [[nodiscard]] vk::Device         device() const;
        [[nodiscard]] QueueSupport       queueSupport() const;
        [[nodiscard]] Queues             queues() const;
        [[nodiscard]] vk::SurfaceKHR     surface() const;


      private:
        vk::Instance       m_Instance;
        vk::PhysicalDevice m_PhysicalDevice;
        vk::Device         m_Device;

        QueueSupport m_QueueSupport;
        Queues       m_Queues;

        vk::SurfaceKHR m_Surface;
    };

} // namespace kat
