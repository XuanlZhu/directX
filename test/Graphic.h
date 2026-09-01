#pragma once

// #include <Windows.h>
// #include <d3d11.h>
// #include <dxgi.h>
// #include <d3dcompiler.h>
#include <DirectXMath.h>
#include <string>
#include <WICTextureLoader.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")

struct Vertex
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT2 uv;
};

class Graphic
{
public:
    ~Graphic();
    bool Initialize(HWND hWnd);
    void BeginFrame(float r,float g,float b);
    void EndFrame();
    void DrawTexture(ID3D11ShaderResourceView* texture,float x,float y,float width,float height);
    ID3D11ShaderResourceView* LoadTexture(std::string path);
    void CreateVertexBuffer();
    void CreatePixelShader();
    void CreateVertexShader();
    void CreateSampler();
    void CreateBlendState();
private:
    HWND m_hWnd = nullptr;
    IDXGISwapChain* m_swapChain = nullptr;//交换链
    ID3D11Device* m_device = nullptr;//设备
    ID3D11DeviceContext* m_context = nullptr;//设备上下文
    ID3D11RenderTargetView* m_renderTargetView = nullptr;//渲染目标视图

    ID3D11Buffer* m_vertexBuffer = nullptr;//顶点缓冲区
    ID3D11InputLayout* m_inputLayout = nullptr;//输入布局
    ID3D11VertexShader* m_vertexShader = nullptr;//顶点Shader
    ID3D11PixelShader* m_pixelShader = nullptr;//着色器
    ID3D11SamplerState* m_sampler = nullptr;//取样器
    ID3D11BlendState* m_alphaBlendState = nullptr;// Alpha混合状态

    ID3D11ShaderResourceView* test_texture;
    ID3D11ShaderResourceView* texture1;
    ID3D11ShaderResourceView* texture2;
    ID3D11ShaderResourceView* texture3;
    ID3D11ShaderResourceView* texture4;
};