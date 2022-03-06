#include"DXSample.h"

DXSample::DXSample(uint width, uint height, std::wstring name) :
    m_width(width),
    m_height(height),
    m_title(name),
    m_useWarpDevice(false)
{
    wchar_t assetsPath[512];
    GetAssetsPath(assetsPath, _countof(assetsPath));
    m_assetsPath = assetsPath;
    m_aspectRatio= static_cast<float>(width) / static_cast<float>(height);
}
DXSample::~DXSample()
{

}

void DXSample::ParseCommandLineArgs(_In_reads_(argc)wchar_t* argv[], int argc) 
{
    for (int i = 1; i < argc; ++i)
    {
        if (_wcsnicmp(argv[i], L"-warp", wcslen(argv[i])) == 0 ||
            _wcsnicmp(argv[i], L"/warp", wcslen(argv[i])) == 0)
        {
            m_useWarpDevice = true;
            m_title = m_title + L" (WARP)";
        }
    }
}

std::wstring DXSample::GetAssetFullPath(LPCWSTR assetName) 
{
    return m_assetsPath + assetName;
}

void DXSample::GetHardwareAdapter(_In_ IDXGIFactory1* pFactory, _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter, bool requestHighPerfermanceAdapter = false) 
{
    *ppAdapter = nullptr;
    ComPtr<IDXGIAdapter1>adapter;
    ComPtr<IDXGIFactory1>factory1;
    if (SUCCEEDED(pFactory->QueryInterface(IID_PPV_ARGS(&factory1)))) 
    {
        for (uint adapterIndex = 0; SUCCEEDED(factory1->EnumAdapterByGpuPreference(
            adapterIndex,
            requestHighPerformanceAdapter == true ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED,
            IID_PPV_ARGS(&adapter))); ++adapterIndex) 
        {
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);
            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) 
            {

            }
            if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr))) 
            {
                break;
            }
        }
    }

    if (adapter.Get() == nullptr) 
    {
        for (uint32_t adapterIndex = 0; SUCCEEDED(pFactory->EnumAdapters1(adapterIndex, &adapter)); ++adapterIndex)
        {
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                // Don't select the Basic Render Driver adapter.
                // If you want a software adapter, pass in "/warp" on the command line.
                continue;
            }

            // Check to see whether the adapter supports Direct3D 12, but don't create the
            // actual device yet.
            if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
            {
                break;
            }
        }
    }

    *ppAdapter = adapter.Detach();
}

void DXSample::SetCustomWindowText(LPCWSTR text) 
{
    std::wstring windowText = m_title + L": " + text;
    SetWindowText(Win32Application::GetHwnd(), windowText.c_str());
}
