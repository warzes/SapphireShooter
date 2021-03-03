#include "stdafx.h"
#if SE_D3D11
#include "Profiler.h"
#include "D3D11Graphics.h"
#include "D3D11IndexBuffer.h"
#include "Engine/DebugNew.h"

IndexBuffer::IndexBuffer() :
    buffer(nullptr),
    numIndices(0),
    indexSize(0),
    usage(USAGE_DEFAULT)
{
}

IndexBuffer::~IndexBuffer()
{
    Release();
}

void IndexBuffer::Release()
{
    if (graphics && graphics->GetIndexBuffer() == this)
        graphics->SetIndexBuffer(nullptr);
    
    if (buffer)
    {
        ID3D11Buffer* d3dBuffer = (ID3D11Buffer*)buffer;
        d3dBuffer->Release();
        buffer = nullptr;
    }
}

bool IndexBuffer::SetData(size_t firstIndex, size_t numIndices_, const void* data)
{
    PROFILE(UpdateIndexBuffer);

    if (!data)
    {
        SE_LOG_ERROR("Null source data for updating index buffer");
        return false;
    }
    if (firstIndex + numIndices_ > numIndices)
    {
        SE_LOG_ERROR("Out of bounds range for updating index buffer");
        return false;
    }
    if (buffer && usage == USAGE_IMMUTABLE)
    {
        SE_LOG_ERROR("Can not update immutable index buffer");
        return false;
    }

    if (shadowData)
        memcpy(shadowData.Get() + firstIndex * indexSize, data, numIndices_ * indexSize);

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
                memcpy((unsigned char*)mappedData.pData + firstIndex * indexSize, data, numIndices_ * indexSize);
                d3dDeviceContext->Unmap((ID3D11Buffer*)buffer, 0);
            }
            else
            {
                SE_LOG_ERROR("Failed to map index buffer for update");
                return false;
            }
        }
        else
        {
            D3D11_BOX destBox;
            destBox.left = (unsigned)(firstIndex * indexSize);
            destBox.right = destBox.left + (unsigned)(numIndices_ * indexSize);
            destBox.top = 0;
            destBox.bottom = 1;
            destBox.front = 0;
            destBox.back = 1;

            d3dDeviceContext->UpdateSubresource((ID3D11Buffer*)buffer, 0, &destBox, data, 0, 0);
        }
    }

    return true;
}

bool IndexBuffer::Create(const void* data)
{
    if (graphics && graphics->IsInitialized())
    {
        D3D11_BUFFER_DESC bufferDesc;
        D3D11_SUBRESOURCE_DATA initialData;
        memset(&bufferDesc, 0, sizeof bufferDesc);
        memset(&initialData, 0, sizeof initialData);
        initialData.pSysMem = data;

        bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bufferDesc.CPUAccessFlags = (usage == USAGE_DYNAMIC) ? D3D11_CPU_ACCESS_WRITE : 0;
        bufferDesc.Usage = (D3D11_USAGE)usage;
        bufferDesc.ByteWidth = (unsigned)(numIndices * indexSize);

        ID3D11Device* d3dDevice = (ID3D11Device*)graphics->D3DDevice();
        d3dDevice->CreateBuffer(&bufferDesc, data ? &initialData : nullptr, (ID3D11Buffer**)&buffer);

        if (!buffer)
        {
            SE_LOG_ERROR("Failed to create index buffer");
            return false;
        }
        else
            LOGDEBUGF("Created index buffer numIndices %u indexSize %u", (unsigned)numIndices, (unsigned)indexSize);
    }

    return true;
}

#endif // SE_D3D11