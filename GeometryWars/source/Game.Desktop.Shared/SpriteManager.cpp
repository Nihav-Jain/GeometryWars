#include "pch.h"
#include "SpriteManager.h"

namespace Library
{
	void SpriteManager::AddValue(std::int32_t value)
	{
		SetValue(mValue + value);
	}

	void SpriteManager::SetValue(std::int32_t value)
	{
		mValue = value;

		RefreshSprites();
	}

	std::int32_t SpriteManager::GetValue() const
	{
		return mValue;
	}

	void SpriteManager::SetData(std::int32_t value, std::int32_t numSprites, std::int32_t imageSize, std::int32_t xOffset, std::int32_t yOffset,
		std::int32_t padding, bool leftToRight, const std::string & path, const std::string & folder, const std::string & extension)
	{
		mValue = value;
		mNumSprites = numSprites;
		mImageSize = imageSize;

		mHorizontalOffset = xOffset;
		mVerticalOffset = yOffset;
		mPadding = padding;

		mOrderLeftToRight = leftToRight;

		mBasePath = path;
		mFolderPath = folder;
		mExtension = extension;
	}
}