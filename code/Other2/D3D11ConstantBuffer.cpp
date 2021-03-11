#include "stdafx.h"
#if SE_D3D11
#include "Profiler.h"
#include "Color.h"
#include "Matrix3.h"
#include "Matrix3x4.h"
#include "D3D11ConstantBuffer.h"
#include "D3D11Graphics.h"
#include "Engine/DebugNew.h"

ConstantBuffer::ConstantBuffer() :
    buffer(nullptr),
    byteSize(0),
    usage(USAGE_DEFAULT),
    dirty(false)
{
}

ConstantBuffer::~ConstantBuffer()
{
    Release();
}

void ConstantBuffer::Release()
{
    if (graphics)
    {
        for (size_t i = 0; i < MAX_SHADER_STAGES; ++i)
        {
            for (size_t j = 0; j < MAX_CONSTANT_BUFFERS; ++j)
            {
                if (graphics->GetConstantBuffer((ShaderStage)i, j) == this)
                    graphics->SetConstantBuffer((ShaderStage)i, j, 0);
            }
        }
    }
    
    if (buffer)
    {
        ID3D11Buffer* d3dBuffer = (ID3D11Buffer*)buffer;
        d3dBuffer->Release();
        buffer = nullptr;
    }
}

bool ConstantBuffer::SetData(const void* data, bool copyToShadow)
{
    if (copyToShadow)
        memcpy(shadowData.Get(), data, byteSize);

    if (usage == USAGE_IMMUTABLE)
    {
        if (!buffer)
            return Create(data);
        else
        {
            SE_LOG_ERROR("Apply can only be called once on an immutable constant buffer");
            return false;
        }
    }

    if (buffer)
    {
        ID3D11DeviceContext* d3dDeviceContext = (ID3D11DeviceContext*)graphics->D3DDeviceContext();
        if (usage == USAGE_DYNAMIC)
        {
            D3D11_MAPPED_SUBRESOURCE mappedData;
            mappedData.pData = nullptr;

            d3dDeviceContext->Map((ID3D11Buffer*)buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
            if (mappedData.pData)
            {
                memcpy((unsigned char*)mappedData.pData, data, byteSize);
                d3dDeviceContext->Unmap((ID3D11Buffer*)buffer, 0);
            }
            else
            {
                SE_LOG_ERROR("Failed to map constant buffer for update");
                return false;
            }
        }
        else
            d3dDeviceContext->UpdateSubresource((ID3D11Buffer*)buffer, 0, nullptr, data, 0, 0);
    }

    dirty = false;
    return true;
}

bool ConstantBuffer::Create(const void* data)
{
    dirty = false;

    if (graphics && graphics->IsInitialized())
    {
        D3D11_BUFFER_DESC bufferDesc;
        D3D11_SUBRESOURCE_DATA initialData;
        memset(&bufferDesc, 0, sizeof bufferDesc);
        memset(&initialData, 0, sizeof initialData);
        initialData.pSysMem = data;

        bufferDesc.ByteWidth = (unsigned)byteSize;
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bufferDesc.CPUAccessFlags = (usage == USAGE_DYNAMIC) ? D3D11_CPU_ACCESS_WRITE : 0;
        bufferDesc.Usage = (D3D11_USAGE)usage;

        ID3D11Device* d3dDevice = (ID3D11Device*)graphics->D3DDevice();
        d3dDevice->CreateBuffer(&bufferDesc, data ? &initialData : nullptr, (ID3D11Buffer**)&buffer);

        if (!buffer)
        {
            SE_LOG_ERROR("Failed to create constant buffer");
            return false;
        }
        else
            SE_LOG("Created constant buffer size " + std::to_string((unsigned)byteSize));
    }

    return true;
}

#endif // SE_D3D11