#pragma once
#include "PostProcessing.h"

namespace Library {

	class FrameBuffer;
	class Texture;
	class Shader;
	class Buffer;

	class BloomPostProcessing :
		public PostProcessing
	{
	public:
		BloomPostProcessing();
		~BloomPostProcessing();

		virtual FrameBuffer * Apply(RenderDevice * device, FrameBuffer * src_buffer, FrameBuffer * target_buffer) override;

	private:
		void Init(RenderDevice * device);
		bool mInited;
		FrameBuffer * mFrameBuffer;
		Shader * mShader;
		Buffer * mBuffer;
	};

}
