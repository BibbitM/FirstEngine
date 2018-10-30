#pragma once

#include "NonCopyable.h"

struct RendererDevice;

class IRenderer : private NonCopyable
{
public:
	virtual RendererDevice GetDevice() const = 0;

protected:
	virtual ~IRenderer() = 0 { }
};
