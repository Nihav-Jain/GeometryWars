#pragma once
#include "../../source/Library.Desktop/Game.h"

namespace Library
{
	class SpriteManager
	{
	public:
		SpriteManager() = default;
		virtual ~SpriteManager() = default;

		void AddValue(std::int32_t value);
		void SetValue(std::int32_t value);
		std::int32_t GetValue() const;
		void SetData(std::int32_t value, std::int32_t numSprites, std::int32_t imageSize, std::int32_t xOffset, std::int32_t yOffset,
			std::int32_t padding, bool leftToRight, const std::string & path, const std::string & folder, const std::string & extension);

		virtual void Init() = 0;
		virtual void RefreshSprites() = 0;
		virtual void CleanupSprites() = 0;

	protected:

		std::int32_t mValue;
		std::int32_t mNumSprites;
		std::int32_t mImageSize;

		std::int32_t mHorizontalOffset;
		std::int32_t mVerticalOffset;
		std::int32_t mPadding;

		bool mOrderLeftToRight;

		std::string mBasePath;
		std::string mFolderPath;
		std::string mExtension;

	};
}