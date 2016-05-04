#pragma once
#include "WorldState.h"
#include <chrono>

namespace Library {

	class LineParticle
	{
	public:
		LineParticle();
		~LineParticle();

		void Update(glm::vec4 force, WorldState& worldState);

		bool IsExpired;

		float Vertices[24];

		glm::vec4 Position;
		glm::vec4 Rotation;
		glm::vec4 Scale;
		glm::vec4 Force;
		glm::vec4 Color;

	private:
		std::chrono::milliseconds Duration;
	};

}

