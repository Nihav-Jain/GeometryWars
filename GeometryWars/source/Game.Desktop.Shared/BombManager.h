#pragma once

#include "SpriteManager.h"
#include "../../source/Library.Desktop/Game.h"

#include <vector>

namespace Library
{
	class BombManager final : public SpriteManager
	{
	public:
		static BombManager* CreateInstance();
		static BombManager* GetInstance();
		static void DeleteInstance();

		BombManager();
		~BombManager();

		virtual void Init() override;
		virtual void RefreshSprites() override;
		virtual void CleanupSprites() override;

	private:
		static BombManager *sInstance;
		static const std::string IMAGE_NAME;

		std::vector<class SpriteRenderer*> mSprites;
		std::vector<class Image*> mImages;
	};
}

