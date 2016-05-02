#pragma once
#include "FrameBuffer.h"


namespace OpenGLImplmentation {
	class OpenGLFrameBuffer :
		public Library::FrameBuffer
	{
	public:
		OpenGLFrameBuffer();
		~OpenGLFrameBuffer();
	};

}