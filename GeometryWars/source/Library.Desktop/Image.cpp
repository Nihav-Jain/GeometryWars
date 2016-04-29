#include "pch.h"
#include "Image.h"
#include "RenderDevice.h"

namespace Library
{
	RTTI_DEFINITIONS(Image, Action);

	const std::string Image::ATTRIBUTE_PATH = "path";
	const std::string Image::ATTRIBUTE_SIZE = "size";

	Image::Image() :
		mTexture(nullptr)
	{
		AddExternalAttribute(ATTRIBUTE_PATH, 1, &mPath);
		AddExternalAttribute(ATTRIBUTE_SIZE, 1, &mSize);
	}

	Image::~Image()
	{
	}

	void Image::Init(RenderDevice * device)
	{
		if (mTexture != nullptr)
			return;

		mTexture = device->CreateTexture(mPath);
	}
	void Image::GetImageInfo(Texture ** texture, glm::vec4 ** size)
	{
		*texture = mTexture;
		*size = &mSize;
	}
}