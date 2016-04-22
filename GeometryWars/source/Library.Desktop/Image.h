#pragma once
#include "Action.h"

namespace Library
{
	class RenderDevice;
	class Texture;

	class Image : public Action
	{
		RTTI_DECLARATIONS(Image, Action);
	public:
		Image();
		~Image();

		void Init(RenderDevice * device);

		void GetImageInfo(Texture ** texture, glm::vec4 ** size);

		virtual void Update(WorldState& worldState) {
			(worldState);
		};

	private:
		Texture * mTexture;

		std::string mPath;
		glm::vec4 mSize;
		static const std::string ATTRIBUTE_PATH;
		static const std::string ATTRIBUTE_SIZE;
	};

}
