#include <Windows.h>
#include <stdio.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl.h>

using namespace Microsoft::WRL;

struct __declspec(uuid("8C803E30-9E41-4DDF-B206-46F28E90E405")) IDXGISwapChainTest : IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject) = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef() = 0;
	virtual ULONG STDMETHODCALLTYPE Release() = 0;

	virtual bool STDMETHODCALLTYPE HasProxyFrontBufferSurface() = 0;
	virtual HRESULT STDMETHODCALLTYPE GetFrameStatisticsTest(void *) = 0;
	virtual void STDMETHODCALLTYPE EmulateXBOXBehavior(BOOL) = 0;
	virtual DXGI_COLOR_SPACE_TYPE STDMETHODCALLTYPE GetColorSpace1() = 0;
	virtual void STDMETHODCALLTYPE GetBufferLayoutInfoTest(void *) = 0;
	virtual void *STDMETHODCALLTYPE GetDFlipOutput() = 0;
	virtual UINT STDMETHODCALLTYPE GetBackBufferImplicitRotationCount() = 0;
	virtual void STDMETHODCALLTYPE GetBufferConverterInfo(void *) = 0;
};

int main(int argc, char **argv)
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, DefWindowProcA, 0L, 0L, GetModuleHandleA(NULL), NULL, NULL, NULL, NULL, "D3D12WindowClass", NULL };
	RegisterClassExA(&wc);

	HWND hwnd = CreateWindowA(wc.lpszClassName, "D3D12 Window", WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, NULL, NULL, wc.hInstance, NULL);

	ComPtr<IDXGIFactory4> factory;
	CreateDXGIFactory1(IID_PPV_ARGS(&factory));

	printf("Creating device...\n");

	ComPtr<ID3D12Device> device;
	D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device));

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	ComPtr<ID3D12CommandQueue> commandQueue;
	device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue));

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = 2;
	swapChainDesc.Width = 800;
	swapChainDesc.Height = 600;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;

	printf("Creating swapchain...\n");

	ComPtr<IDXGISwapChain1> swapChain1;
	factory->CreateSwapChainForHwnd(commandQueue.Get(), hwnd, &swapChainDesc, nullptr, nullptr, &swapChain1);

	ComPtr<IDXGISwapChain3> swapChain;
	swapChain1.As(&swapChain);

	printf("Querying interface...\n");

	ComPtr<IDXGISwapChainTest> swapChainTest;
	HRESULT hr = swapChain.As(&swapChainTest);

	printf("IDXGISwapChainTest interface query: %s\n", SUCCEEDED(hr) ? "Succeeded" : "Failed");

	if (SUCCEEDED(hr))
	{
		printf("Initial GetColorSpace1: %u\n", swapChainTest->GetColorSpace1());

		printf("Changing color space to DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709 (%u)\n", DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020);
		swapChain->SetColorSpace1(DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020);

		printf("New GetColorSpace1: %u\n", swapChainTest->GetColorSpace1());
	}

	return 0;
}
