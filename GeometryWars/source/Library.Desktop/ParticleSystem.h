#pragma once
#include <vector>
#include "Action.h"
#include "Renderable.h"

namespace Library {

	class Shader;
	class Buffer;

	template <typename ParticlePattern>
	class ParticleSystem : public Entity, public Renderable
	{
	public:
		ParticleSystem();
		virtual ~ParticleSystem();

		static ParticleSystem * CreateParticleSystem(Sector * parent, std::uint32_t cnt,
			glm::vec4 pos, glm::vec4 scale, glm::vec4 color);

		virtual void SetEnalbe(bool enable);

		virtual void Update(WorldState& worldState) override;

		virtual void OnDestroy(WorldState& worldState) override;

		virtual void Render(RenderDevice * device) override;

	private:
		virtual void GenerateParticles(Sector * parent, std::uint32_t cnt, 
			glm::vec4 pos, glm::vec4 scale, glm::vec4 color);
		std::vector<ParticlePattern> mParticles;

		void Init(RenderDevice * device);
		bool mInited;
		bool mEnabled;
		bool mExpired;
		Shader * mShader;
		Buffer * mBuffer;

		glm::vec4 mColor;

		glm::vec4 mPosition;
		glm::vec4 mScale;

		glm::vec4 mForce;
	};


}

#include "ParticleSystem.inl"