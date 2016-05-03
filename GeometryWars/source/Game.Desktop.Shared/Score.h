#pragma once
#include "../../source/Library.Desktop/Game.h"
#include <vector>

namespace Library
{
	class Score final
	{
	public:
		static Score* CreateInstance();
		static Score* GetInstance();
		static void DeleteInstance();

		Score(std::int32_t value = 0);
		~Score();

		void Init();

		void AddScore(std::int32_t value);
		void SetScore(std::int32_t value);
		std::int32_t GetScore() const;

		void RefreshSprites();

	private:
		static Score *sInstance;

		std::int32_t mValue;

		std::vector<std::vector<class SpriteRenderer*>> mSprites;
		std::vector<std::vector<class Image*>> mDigits;
		std::vector<Library::SpriteRenderer*> mCurrentSprites;

		const std::string mPath;
		const std::string mExtension;
	};
}