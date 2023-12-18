#include "context.hpp"


#include <iostream>
#include <set>

namespace kat {
    QueueSupport QueueSupport::query(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface) {
        auto         queue_family_properties = physicalDevice.getQueueFamilyProperties();
        QueueSupport support{ .graphics = UINT32_MAX, .present = UINT32_MAX, .transfer = UINT32_MAX };
        uint32_t     index = 0;

        for (const auto &properties : queue_family_properties) {
            if (properties.queueFlags & vk::QueueFlagBits::eGraphics) {
                support.graphics = index;
            }

            if (properties.queueFlags & vk::QueueFlagBits::eTransfer) {
                support.transfer = index;
            }

            if (physicalDevice.getSurfaceSupportKHR(index, surface)) {
                support.present = index;
            }

            if (support.isComplete())
                break;
        }

        if (!support.isComplete()) {
            throw missing_queues_error();
        }

        return support;
    }

    bool QueueSupport::isComplete() const {
        return graphics != UINT32_MAX && present != UINT32_MAX && transfer != UINT32_MAX;
    }

    constexpr float queuePriority = 1.0f; // out here to make sure its in scope forever.

    std::vector<vk::DeviceQueueCreateInfo> QueueSupport::generateCreateInfos() const {
        std::vector<vk::DeviceQueueCreateInfo> infos;
        std::set<uint32_t>                     families;


        infos.push_back(vk::DeviceQueueCreateInfo({}, graphics, 1, &queuePriority));
        families.insert(graphics);

        if (!families.contains(present)) {
            infos.push_back(vk::DeviceQueueCreateInfo({}, present, 1, &queuePriority));
            families.insert(present);
        }

        if (!families.contains(transfer)) {
            infos.push_back(vk::DeviceQueueCreateInfo({}, transfer, 1, &queuePriority));
            families.insert(transfer);
        }

        return infos;
    }

    Queues Queues::query(vk::Device device, const QueueSupport &queueSupport) {
        Queues queues{};
        queues.graphics = device.getQueue(queueSupport.graphics, 0);
        queues.present  = device.getQueue(queueSupport.present, 0);
        queues.transfer = device.getQueue(queueSupport.transfer, 0);

        return queues;
    }

    Context::Context(const std::shared_ptr<Window> &window, const ContextSettings &settings) {
        auto appInfo = vk::ApplicationInfo().setApiVersion(VK_API_VERSION_1_3);

        uint32_t     ieCount;
        const char **instanceExtensions = glfwGetRequiredInstanceExtensions(&ieCount);

        m_Instance = vk::createInstance(vk::InstanceCreateInfo({}, &appInfo, 0, nullptr, ieCount, instanceExtensions));

        m_PhysicalDevice = m_Instance.enumeratePhysicalDevices()[0];

        m_Surface = window->createSurface(m_Instance);

        m_QueueSupport = QueueSupport::query(m_PhysicalDevice, m_Surface);

        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos = m_QueueSupport.generateCreateInfos();

        vk::PhysicalDeviceFeatures2 features2{};

        std::vector<const char *> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

        m_Device = m_PhysicalDevice.createDevice(
            vk::DeviceCreateInfo({}, queueCreateInfos, {}, deviceExtensions, {}, &features2));

        m_Queues = Queues::query(m_Device, m_QueueSupport);

        std::cout << "Created Device" << std::endl;
    }

    Context::~Context() {
        m_Device.destroy();

        m_Instance.destroy(m_Surface);
        m_Instance.destroy();
    }

    vk::Instance Context::instance() const {
        return m_Instance;
    }

    vk::PhysicalDevice Context::physicalDevice() const {
        return m_PhysicalDevice;
    }

    vk::Device Context::device() const {
        return m_Device;
    }

    QueueSupport Context::queueSupport() const {
        return m_QueueSupport;
    }

    Queues Context::queues() const {
        return m_Queues;
    }

    vk::SurfaceKHR Context::surface() const {
        return m_Surface;
    }
} // namespace kat
