#include "pch.h"
#include "BombManager.h"
#include "../../source/Library.Desktop/RenderDevice.h"
#include "../../source/Library.Desktop/Renderer.h"
#include "../../source/Library.Desktop/Image.h"
#include "../../source/Library.Desktop/SpriteRenderer.h"

namespace Library
{
	const std::string BombManager::IMAGE_NAME = "bomb";

	BombManager* BombManager::sInstance = nullptr;

	BombManager* BombManager::CreateInstance()
	{
		if (sInstance == nullptr)
		{
			sInstance = new BombManager();
		}

		return sInstance;
	}

	BombManager* BombManager::GetInstance()
	{
		if (sInstance == nullptr)
		{
			sInstance = new BombManager();
		}
		return sInstance;
	}

	void BombManager::DeleteInstance()
	{
		if (sInstance != nullptr)
		{
			delete sInstance;
			sInstance = nullptr;
		}
	}

	BombManager::BombManager()
		: mSprites(), mImages()
	{
	}

	BombManager::~BombManager()
	{
		for (std::int32_t i = 0; i < mNumSprites; ++i)
		{
			delete mSprites[i];
			mSprites[i] = nullptr;
		}
	}

	void BombManager::Init()
	{
		mSprites.resize(mNumSprites);
		mImages.resize(mNumSprites);

		std::int32_t padding = 0;
		for (std::int32_t i = 0; i < mNumSprites; ++i)
		{
			// Determine position
			std::int32_t order = (mOrderLeftToRight) ? i : (mNumSprites - i);
			glm::vec4 pos = glm::vec4(order * mImageSize + mHorizontalOffset + padding, mVerticalOffset, 1, 1);
			padding = (mOrderLeftToRight) ? padding - mPadding : padding + mPadding;

			mSprites[i] = new SpriteRenderer();
			SpriteRenderer* renderer = mSprites[i];

			mImages[i] = renderer->CreateAction("Image", "image", *renderer, ActionList::ATTRIBUTE_ACTIONS).AssertiveAs<Image>();
			mImages[i]->SetPath(mBasePath + mFolderPath + IMAGE_NAME + mExtension);
			mImages[i]->SetSize(glm::vec4((float)(mImageSize)));

			renderer->SetTransform(pos, glm::vec4(), glm::vec4(1));
		}
	}

	void BombManager::RefreshSprites()
	{
		for (std::int32_t i = 0; i < mNumSprites; ++i)
		{
			Renderer::GetInstance()->RemoveRenderable(mSprites[i], 101);

			if (i < mValue)
			{
				Renderer::GetInstance()->AddRenderable(mSprites[i], 101);
			}
		}
	}

	void BombManager::CleanupSprites()
	{
		for (std::int32_t i = 0; i < mNumSprites; ++i)
		{
			Renderer::GetInstance()->RemoveRenderable(mSprites[i], 101);
		}
	}
}