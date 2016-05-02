#pragma once

namespace Library {

	class RenderDevice;
	class FrameBuffer;
	class PostProcessing
	{
	public:
		PostProcessing() = default;
		~PostProcessing() = default;

		virtual void Apply(RenderDevice * device, FrameBuffer * buffer) = 0;
	};

}