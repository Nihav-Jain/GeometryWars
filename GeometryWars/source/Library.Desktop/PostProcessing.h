#pragma once

namespace Library {

	class RenderDevice;
	class FrameBuffer;
	class PostProcessing
	{
	public:
		PostProcessing() = default;
		~PostProcessing() = default;

		virtual void Apply(RenderDevice * device, FrameBuffer * src_buffer, FrameBuffer * target_buffer) = 0;
	};

}