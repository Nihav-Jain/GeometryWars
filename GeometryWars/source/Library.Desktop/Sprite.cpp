#include "pch.h"
#include "Sprite.h"
#include "RenderDevice.h"

namespace Library {

	RTTI_DEFINITIONS(Sprite);

	const std::uint32_t Sprite::NUM_RESERVED_PRESCRIBED_ATTRIBUTES = 2;
	const std::string Sprite::ATTRIBUTE_POSITION = "position";
	const std::string Sprite::ATTRIBUTE_IMAGE_PATH = "imagePath";
	const std::string Sprite::ATTRIBUTE_COLOR = "color";

	Sprite::Sprite()
	{
		AddExternalAttribute(ATTRIBUTE_POSITION, 1, &mPosition);
		AddExternalAttribute(ATTRIBUTE_IMAGE_PATH, 1, &mImagePath);
		AddExternalAttribute(ATTRIBUTE_COLOR, 1, &mColor);
	}

	const glm::vec4 & Sprite::GetPosition() const
	{
		return mPosition;
	}

	const std::string & Sprite::GetImagePath() const
	{
		return mImagePath;
	}

	const glm::vec4 & Sprite::GetColor() const
	{
		return mColor;
	}

	void Sprite::Render(RenderDevice * device)
	{
		(device);
	}

}