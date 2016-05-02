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

		virtual void Apply(RenderDevice * device, FrameBuffer * buffer) override;

	private:
		void Init(RenderDevice * device);
		bool mInited;
		FrameBuffer * mFrameBuffer;
		Texture * mTexture;
		Shader * mShader;
		Buffer * mBuffer;
	};

}
