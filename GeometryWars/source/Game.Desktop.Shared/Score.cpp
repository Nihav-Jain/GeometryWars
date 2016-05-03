#include "pch.h"
#include "Score.h"
#include "../../source/Library.Desktop/RenderDevice.h"
#include "../../source/Library.Desktop/Renderer.h"
#include "../../source/Library.Desktop/Image.h"
#include "../../source/Library.Desktop/SpriteRenderer.h"

#define NUM_DIGITS 10
#define SCORE_LENGTH 10

#define PATH "Content//resource//digits//"
#define EXT ".png"

#define IMAGE_SIZE 100
#define OFFSET -500
#define HEIGHT 150

namespace Library
{
	Score* Score::sInstance = nullptr;

	Score* Score::CreateInstance()
	{
		if (sInstance == nullptr)
		{
			sInstance = new Score();
		}

		return sInstance;
	}

	Score* Score::GetInstance()
	{
		return sInstance;
	}

	void Score::DeleteInstance()
	{
		if (sInstance != nullptr)
		{
			delete sInstance;
			sInstance = nullptr;
		}
	}

	Score::Score(std::int32_t value)
		: mValue(value), mDigits(SCORE_LENGTH), mSprites(SCORE_LENGTH), mCurrentSprites(SCORE_LENGTH), mPath(PATH), mExtension(EXT)
	{
		Init();
	}

	Score::~Score()
	{
		for (std::uint32_t i = 0; i < SCORE_LENGTH; ++i)
		{
			for (std::uint32_t j = 0; i < NUM_DIGITS; ++j)
			{
				delete mSprites[i][j];
				mSprites[i][j] = nullptr;
			}
		}
	}

	void Score::Init()
	{
		for (std::uint32_t i = 0; i < SCORE_LENGTH; ++i)
		{
			// Determine position
			glm::vec4 pos = glm::vec4((SCORE_LENGTH - i) * IMAGE_SIZE + OFFSET, HEIGHT, 1, 1);

			mDigits[i].resize(NUM_DIGITS);
			mSprites[i].resize(NUM_DIGITS);

			for (std::uint32_t j = 0; j < NUM_DIGITS; ++j)
			{
				// Create sprite renderer for each image
				mSprites[i][j] = new SpriteRenderer();
				SpriteRenderer* renderer = mSprites[i][j];

				// Create image per digit per location
				mDigits[i][j] = renderer->CreateAction("Image", "image", *renderer, ActionList::ATTRIBUTE_ACTIONS).AssertiveAs<Image>();
				mDigits[i][j]->SetPath(mPath + std::to_string(j) + mExtension);
				mDigits[i][j]->SetSize(glm::vec4(IMAGE_SIZE));

				renderer->SetTransform(pos, glm::vec4(), glm::vec4(1));
			}

			mCurrentSprites[i] = mSprites[i][0];
			Renderer::GetInstance()->AddRenderable(mCurrentSprites[i], 101U);
		}
	}

	void Score::AddScore(std::int32_t value)
	{
		SetScore(mValue + value);
	}

	void Score::SetScore(std::int32_t value)
	{
		mValue = value;

		RefreshSprites();
	}

	std::int32_t Score::GetScore() const
	{
		return mValue;
	}

	void Score::RefreshSprites()
	{
		std::int32_t value = mValue;

		for (std::uint32_t i = 0; i < SCORE_LENGTH; ++i)
		{
			std::int32_t digit = value % 10;

			// Set new image
			Renderer::GetInstance()->RemoveRenderable(mCurrentSprites[i], 101U);
			mCurrentSprites[i] = mSprites[i][digit];
			Renderer::GetInstance()->AddRenderable(mCurrentSprites[i], 101U);

			value = value / 10;
		}
	}

}