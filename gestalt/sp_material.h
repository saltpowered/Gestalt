#pragma once

#include "sp_sprite.h"
#include "sp_shader.h"

#include "glm/glm.hpp"

struct SPMaterial {
	SPSprite sprite;
	glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	SPShader* shader;
	b32 flipped = false;
};