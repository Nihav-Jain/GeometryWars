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
		if (texture!= nullptr)
			*texture = mTexture;
		if (size != nullptr)
			*size = &mSize;
	}

	const std::string & Image::Path() const
	{
		return mPath;
	}

	void Image::SetPath(const std::string & path)
	{
		mPath = path;
	}

	const glm::vec4 & Image::Size() const
	{
		return mSize;
	}

	void Image::SetSize(const glm::vec4 & size)
	{
		mSize = size;
	}
}