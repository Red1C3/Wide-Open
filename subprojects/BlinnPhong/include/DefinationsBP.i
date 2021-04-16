#define LOG Common::Log::instance()
#define WINDOW Common::Window::instance()
#define ALLOCATOR nullptr
#define RENDERER WideOpenBP::RendererBP::instance()
#define GLFW_INCLUDE_VULKAN
#define DEVICE RENDERER.getDevice()
#define PIPELINE WideOpenBP::PipelineBP::instance()
#define RENDERPASS WideOpenBP::RenderpassBP::instance()
#define LAYOUT WideOpenBP::DescriptorSetLayoutBP::instance()