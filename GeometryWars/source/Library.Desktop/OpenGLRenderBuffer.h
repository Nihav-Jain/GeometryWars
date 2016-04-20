#pragma once
#include "RenderBuffer.h"

namespace Library {

	class OpenGLRenderBuffer :
		public RenderBuffer
	{
	public:
		OpenGLRenderBuffer();
		~OpenGLRenderBuffer();
	};

}
