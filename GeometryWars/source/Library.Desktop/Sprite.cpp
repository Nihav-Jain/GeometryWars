#include "pch.h"
#include "Sprite.h"

namespace Library {

	const std::uint32_t Sprite::NUM_RESERVED_PRESCRIBED_ATTRIBUTES = 2;
	const std::string Sprite::ATTRIBUTE_POSITION = "position";
	const std::string Sprite::ATTRIBUTE_IMAGE_PATH = "imagePath";

	Sprite::Sprite()
	{
		AddExternalAttribute(ATTRIBUTE_POSITION, 1, &mPosition);
		AddExternalAttribute(ATTRIBUTE_IMAGE_PATH, 1, &mImagePath);
	}

	const glm::vec4 & Sprite::GetPosition() const
	{
		return mPosition;
	}

	const std::string & Sprite::GetImagePath() const
	{
		return mImagePath;
	}

}