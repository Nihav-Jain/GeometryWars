#include "pch.h"
#include "LineParticle.h"
#include "WorldState.h"
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

namespace Library {

	LineParticle::LineParticle() :
		IsExpired(false),
		Duration(0)
	{
	}


	LineParticle::~LineParticle()
	{
	}

	void LineParticle::Update(glm::vec4 force, WorldState & worldState)
	{
		(force);
		if (!IsExpired) {
			Rotation.z = atan2(Force.x, -Force.y);

			glm::mat4 model;
			model = glm::rotate(model, Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

			Position += Force;

			Duration = Duration + worldState.mGameTime->ElapsedGameTime();

			if (Duration > std::chrono::milliseconds(300)) {
				float delta = (float)worldState.mGameTime->ElapsedGameTime().count() * 0.01f;

				Scale -= glm::vec4(.0f, delta, .0f, .0f);
			}

			float vertices[] = {
				-0.1f, 1.5f, 0.0f, 0.0f,
				0.1f, -1.5f, 0.0f, 0.0f,
				-0.1f, -1.5f, 0.0f, 0.0f,

				-0.1f, 1.5f, 0.0f, 0.0f,
				0.1f, 1.5f, 0.0f, 0.0f,
				0.1f, -1.5f, 0.0f, 0.0f,
			};

			memcpy(Vertices, vertices, sizeof(float) * 24);

			if (Scale.y < 0.1f) {
				IsExpired = true;
			}
			else {
				IsExpired = false;
			}
		}
	}
}
