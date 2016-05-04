#pragma once

#include "SpriteManager.h"
#include "../../source/Library.Desktop/Game.h"

#include <vector>

namespace Library
{
	class LivesManager final : public SpriteManager
	{
	public:
		static LivesManager* CreateInstance();
		static LivesManager* GetInstance();
		static void DeleteInstance();

		LivesManager();
		~LivesManager();

		virtual void Init() override;
		virtual void RefreshSprites() override;
		virtual void CleanupSprites() override;

	private:
		static LivesManager *sInstance;
		static const std::string IMAGE_NAME;

		std::vector<class SpriteRenderer*> mSprites;
		std::vector<class Image*> mImages;
	};
}

