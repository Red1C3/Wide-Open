#define LOG Common::Log::instance()
#define WINDOW Common::Window::instance()
#define ALLOCATOR nullptr
#define RENDERERBP WideOpenBP::RendererBP::instance()
#define GLFW_INCLUDE_VULKAN
#define DEVICEBP RENDERERBP.getDevice()
#define PIPELINEBP WideOpenBP::PipelineBP::instance()
#define RENDERPASSBP WideOpenBP::RenderpassBP::instance()
#define LAYOUTBP WideOpenBP::DescriptorSetLayoutBP::instance()