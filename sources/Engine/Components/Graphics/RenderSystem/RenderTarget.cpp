#include "RenderTarget.h"

RenderTarget::RenderTarget()
{
}

RenderTarget::~RenderTarget()
{
}

void RenderTarget::copyColorComponentData(size_t sourceComponentIndex, RenderTarget * destination, size_t destinationComponentIndex, const Rect & area)
{
	copyColorComponentData(sourceComponentIndex, destination, destinationComponentIndex, area, area, CopyFilter::Linear);
}

void RenderTarget::copyDepthStencilComponentData(RenderTarget * destination, const Rect & area)
{
	copyDepthStencilComponentData(destination, area, area, CopyFilter::Linear);
}