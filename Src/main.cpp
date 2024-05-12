#include <webgpu/webgpu.h>
#include <Concerto/Core/Assert.hpp>
#include <Concerto/Core/Logger.hpp>
#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>

#include "CommandEncoder.hpp"
#include "Device.hpp"
#include "PhysicalDevice.hpp"
#include "Queue.hpp"
#include "RenderPassEncoder.hpp"
#include "ShaderModule.hpp"

constexpr auto* shaderSource = R"(
@vertex
fn vs_main(@builtin(vertex_index) in_vertex_index: u32) -> @builtin(position) vec4<f32> {
	var p = vec2f(0.0, 0.0);
	if (in_vertex_index == 0u) {
		p = vec2f(-0.5, -0.5);
	} else if (in_vertex_index == 1u) {
		p = vec2f(0.5, -0.5);
	} else {
		p = vec2f(0.0, 0.5);
	}
	return vec4f(p, 0.0, 1.0);
}

@fragment
fn fs_main() -> @location(0) vec4f {
    return vec4f(0.0, 0.4, 1.0, 1.0);
}
)";

int main()
{
    WGPUInstanceDescriptor desc = {};
    desc.nextInChain = nullptr;

    WGPUInstance instance = wgpuCreateInstance(&desc);
    if (!instance) {
        Concerto::Logger::Debug("Could not initialize WebGPU!");
        return 1;
    }

	if (!glfwInit()) {
		Concerto::Logger::Debug("Could not initialize GLFW!");
		return 1;
	}
	constexpr Concerto::UInt32 width = 800;
	constexpr Concerto::UInt32 height = 600;

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* window = glfwCreateWindow(width, height, "WebGPU", nullptr, nullptr);
	if (!window) {
		Concerto::Logger::Debug("Could not open window!");
		glfwTerminate();
		return 1;
	}

	WGPUSurface surface = glfwGetWGPUSurface(instance, window);
	
	PhysicalDevice physicalDevice(instance, surface);
	physicalDevice.PrintFeatures();
	Device device(physicalDevice);
	const WGPUTextureFormat swapChainFormat = wgpuSurfaceGetPreferredFormat(surface, physicalDevice.Get());
	SwapChain swapChain = device.CreateSwapChain(width, height, surface, swapChainFormat);
	Queue queue = device.GetQueue();
	ShaderModule shaderModule = device.CreateShaderModule(shaderSource);
	RenderPipeline renderPipeline = device.CreateRenderPipeline(shaderModule, swapChainFormat);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		CommandEncoder commandEncoder(device, "Command Encoder");
		{
			auto textureView = swapChain.GetCurrentTextureView();
			if (textureView.Get() == nullptr)
			{
				CONCERTO_ASSERT_FALSE("Invalid texture View");
				continue;
			}
			auto* renderPassEncoder = commandEncoder.BeginRenderPass(textureView, { 0.9, 0.1, 0.2, 1.0 });
			renderPassEncoder->SetPipeline(renderPipeline);
			renderPassEncoder->DrawTriangle();
			commandEncoder.EndRenderPass();
		}
		CommandBuffer commandBuffer = commandEncoder.Finish();
		queue.Submit(commandBuffer);
		swapChain.Present();
	}

	wgpuInstanceRelease(instance);

	glfwDestroyWindow(window);
	glfwTerminate();
    return 0;
}
