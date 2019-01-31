#pragma once

#include <Engine/Components/Math/types.h>
#include <Engine/Components/Math/Geometry/Rect.h>

#include "Texture.h"

using RTClearMode = unsigned int;

/*!
 * Allows rendering to texture and supports Multiple Render Targets
 */
class RenderTarget {
public:
	static const RTClearMode CLEAR_DEPTH = 1;
	static const RTClearMode CLEAR_COLOR = 2;
	static const RTClearMode CLEAR_STENCIL = 4;

	enum class CopyFilter {
		Nearest, Linear
	};

public:
	RenderTarget();
	virtual ~RenderTarget();

	virtual void create() = 0;
	virtual void destroy() = 0;

	virtual void bind() = 0;
	virtual void unbind() = 0;

	virtual void setClearColor(float r, float g, float b, float a = 1.0f) = 0;
	virtual void setClearColor(const vector3& color) = 0;
	virtual void setClearColor(const vector4& color) = 0;
	
	virtual void setDepthClearValue(float depthValue) = 0;
	virtual void setStencilClearValue(int stencilValue) = 0;

	/*!
	 * Clear render target data
	 * 
	 * /param mode Bitfield with clear flags
	 */
	virtual void clear(RTClearMode mode) = 0 ;

	/*!
	 * Attach color component to render target 
	 * 
	 * /param index Index between 0 and MAX_RENDER_TARGET_COLOR_ATTACHMENTS
	 * /param texture 2D texture
	 */
	virtual void attachColorComponent(size_t index, Texture* texture) = 0;

	/*!
	 * Attach depth and stencil component to render target
	 * 
	 * /param texture Attachment, should be DepthStencil texture
	 */
	virtual void attachDepthStencilComponent(Texture* texture) = 0;

	/*!
	 * Copy one color component from current render target to another.
	 * 
	 * /param sourceComponent Index of the component in current render target
	 * /param destination Destination render target
	 * /param destinationComponentIndex Index of the component in destination render target
	 * /param sourceArea The size of a region to get data from source render target
	 * /param destinationArea The size of a region to insert data in destination render target
	 * /param copyFilter Interpolation filter to be applied if the component is stretched
	 */
	virtual void copyColorComponentData(
		size_t sourceComponentIndex,
		RenderTarget* destination, 
		size_t destinationComponentIndex,
		const Rect& sourceArea, const Rect& destinationArea,
		CopyFilter copyFilter = CopyFilter::Linear
	) = 0;

	virtual void copyColorComponentData(
		size_t sourceComponentIndex,
		RenderTarget* destination,
		size_t destinationComponentIndex,
		const Rect& area
	);

	/*!
	 * Copy depth component from current render target to another.
	 * 
	 * /param destination Destination render target
	 * /param sourceArea The size of a region to get data from source render target
	 * /param destinationArea The size of a region to insert data in destination render target
	 * /param copyFilter Interpolation filter to be applied if the component is stretched
	 */
	virtual void copyDepthStencilComponentData(
		RenderTarget* destination,
		const Rect& sourceArea, const Rect& destinationArea,
		CopyFilter copyFilter = CopyFilter::Linear
	) = 0;

	virtual void copyDepthStencilComponentData(
		RenderTarget* destination,
		const Rect& area
	);
};