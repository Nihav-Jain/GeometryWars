#pragma once
#include "Action.h"

namespace Library
{
	class RenderDevice;
	class Texture;

	class Image : public Action
	{
		RTTI_DECLARATIONS(Image, Action);
	public:
		Image();
		~Image();

		Image(const Image & rhs);

		void Init(RenderDevice * device);

		void GetImageInfo(Texture ** texture, glm::vec4 ** size);

		const std::string& Path() const;
		void SetPath(const std::string& path);
		const glm::vec4& Size() const;
		void SetSize(const glm::vec4& size);

		virtual void Update(WorldState& worldState) {
			(worldState);
		};

		virtual Scope* Clone(const Scope& rhs) const override;

	private:
		Texture * mTexture;

		std::string mPath;
		glm::vec4 mSize;
		static const std::string ATTRIBUTE_PATH;
		static const std::string ATTRIBUTE_SIZE;
	};

	CONCRETE_ACTION_FACTORY(Image);

}
