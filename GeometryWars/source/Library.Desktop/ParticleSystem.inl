#include "ParticleSystem.h"
#include "ActionDestroyEntity.h"
#include "Shader.h"
#include "Buffer.h"
#include "Renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

namespace Library {

	template<typename ParticlePattern>
	ParticleSystem<ParticlePattern>::ParticleSystem() :
		mInited(false),
		mEnabled(false),
		mExpired(false),
		mShader(nullptr),
		mBuffer(nullptr)
	{
	}

	template<typename ParticlePattern>
	ParticleSystem<ParticlePattern>::~ParticleSystem()
	{
		Renderer::GetInstance()->RemoveRenderable(this);
		if (mShader != nullptr) {
			delete mShader;
			mShader = nullptr;
		}
		if (mBuffer != nullptr) {
			delete mBuffer;
			mBuffer = nullptr;
		}
	}

	template<typename ParticlePattern>
	ParticleSystem<ParticlePattern> * ParticleSystem<ParticlePattern>::CreateParticleSystem(Sector * parent, std::uint32_t cnt, 
		glm::vec4 pos, glm::vec4 scale, glm::vec4 color)
	{
		ParticleSystem<ParticlePattern> * particleSys = new ParticleSystem<ParticlePattern>();
		particleSys->GenerateParticles(parent, cnt, pos, scale, color);
		return particleSys;
	}

	template<typename ParticlePattern>
	void ParticleSystem<ParticlePattern>::GenerateParticles(Sector * parent, std::uint32_t cnt, 
		glm::vec4 pos, glm::vec4 scale, glm::vec4 color)
	{
		mColor = color;
		char buf[256] = { 0 };
		sprintf_s(buf, 256, "%p", this);
		
		mPosition = pos;
		mScale = scale;

		parent->AdoptEntity(*this, std::string(buf));

		mParticles.reserve(cnt);

		for (std::uint32_t i = 0; i < cnt; ++i ) {

			ParticlePattern p;
			p.Color = mColor;
			p.Position = pos;
			p.Rotation = glm::vec4();
			p.Scale = scale;
			p.Force = glm::vec4(glm::linearRand<float>(-20.0f, 20.0f),
				glm::linearRand<float>(-20.0f, 20.0f),
				0.0f,
				0.0f);

			mParticles.push_back(p);
		}

		Renderer::GetInstance()->AddRenderable(this);
	}

	template<typename ParticlePattern>
	void ParticleSystem<ParticlePattern>::SetEnalbe(bool enable)
	{
		mEnabled = enable;
	}

	template<typename ParticlePattern>
	void ParticleSystem<ParticlePattern>::Update(WorldState & worldState)
	{
		// Try to update our position, and check if we are expired
		mExpired = true;
		for (auto & t : mParticles) {
			t.Update(glm::vec4(), worldState);
			mExpired = t.IsExpired;
		}

		if (mExpired) {
			MarkForDestroy(worldState);
		}
		
	}

	template<typename ParticlePattern>
	void ParticleSystem<ParticlePattern>::OnDestroy(WorldState& worldState)
	{
		(worldState);
		Renderer::GetInstance()->RemoveRenderable(this);
		if (mShader != nullptr) {
			delete mShader;
			mShader = nullptr;
		}
		if (mBuffer != nullptr) {
			delete mBuffer;
			mBuffer = nullptr;
		}
	}

	template<typename ParticlePattern>
	void ParticleSystem<ParticlePattern>::Render(RenderDevice * device)
	{
		if (!mEnabled || mExpired) {
			return;
		}

		if (!mInited) {
			Init(device);
		}

		std::uint32_t cnt = static_cast<std::uint32_t>(mParticles.size());

		for (std::uint32_t i = 0; i < cnt; ++i) {

			mShader->Use();
			
			mShader->SetVector4("inputColor", mParticles[i].Color);
			mBuffer->SetData(mParticles[i].Vertices, 24 * sizeof(float), 4 * sizeof(float), nullptr, 0, 4);

			glm::mat4 model;
			model = glm::translate(model, glm::vec3(mParticles[i].Position));
			model = glm::rotate(model, mParticles[i].Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(mParticles[i].Scale.x, mParticles[i].Scale.y, 1.0f));

			float halfwidth = (float)device->GetViewportWidth() / 2;
			float halfheight = (float)device->GetViewportHeight() / 2;

			glm::mat4 projection = glm::ortho(-halfwidth, halfwidth, -halfheight, halfheight, -1.0f, 1.0f);
			mShader->SetMatrix4("model_view_projection", projection * model);

			mBuffer->Use();
			device->Draw();
		}
	}

	template<typename ParticlePattern>
	void ParticleSystem<ParticlePattern>::Init(RenderDevice * device)
	{
		mShader = device->CreateShader("Content/shader/glsl/sprite_v.glsl",
			"Content/shader/glsl/polygon_f.glsl", "");
		mBuffer = device->CreateBuffer(false);
	}

}
