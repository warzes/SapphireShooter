#pragma once

class IRenderResource
{
public:
	IRenderResource() = default;
	virtual ~IRenderResource()
	{
		Destroy();
	}

	virtual void Destroy() noexcept = 0;

private:
	IRenderResource(const IRenderResource&) = delete;
	IRenderResource(IRenderResource&&) = delete;
	IRenderResource operator=(const IRenderResource&) = delete;
	IRenderResource operator=(IRenderResource&&) = delete;
};