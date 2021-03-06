
#include "sp_prop.h"
#include "sp_window.h"

#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <algorithm>

// NOTE: Around 80% of this could likely be migrated to shaders. For now though, its working.

SPRenderable SPProp::renderable(SPCamera& camera) {
	if (material.valid()) {
		SPRenderable new_renderable;
		glm::vec2 origin_offset;
		SPSprite as = material.val().sprite;

		if (transform.origin.x < -1.0f) origin_offset.x = 1.0f;
		else if (transform.origin.x > 1.0f) origin_offset.x = 1.0f;
		else origin_offset.x = -transform.origin.x;
		if (transform.origin.y < -1.0f) origin_offset.y = -1.0f;
		else if (transform.origin.y > 1.0f) origin_offset.y = 1.0f;
		else origin_offset.y = transform.origin.y;
		origin_offset.x = (transform.scale.x * material.val().sprite.size_exact.x) * (origin_offset.x / 2);
		origin_offset.y = (transform.scale.y * material.val().sprite.size_exact.y) * (origin_offset.y / 2);

		if (transform.anchor == SP_ANCHOR_WORLD) {
			glm::mat4 model(1.0f);
			model = glm::translate(model, glm::vec3(origin_offset.x, origin_offset.y, 0.0f));
			model = glm::translate(model, glm::vec3(transform.pos.x * SP_UNIT_PIXELS, transform.pos.y * SP_UNIT_PIXELS, 0.0f));
			model = glm::rotate(model, transform.rot, glm::vec3(0.0f, 0.0f, 1.0f));
			if (material.val().flipped) {
				model = glm::scale(model, glm::vec3((transform.scale.x * material.val().sprite.size_exact.x) * -1, transform.scale.y * material.val().sprite.size_exact.y, 0.0f));
			}
			else {
				model = glm::scale(model, glm::vec3(transform.scale.x * material.val().sprite.size_exact.x, transform.scale.y * material.val().sprite.size_exact.y, 0.0f));
			}
			glm::mat4 view(1.0f);
			view = glm::translate(view, glm::vec3((-camera.pos.x * SP_UNIT_PIXELS), (camera.pos.y * SP_UNIT_PIXELS), 0.0f));
			glm::mat4 proj(1.0f);
			proj = glm::ortho(-((f32)SPWindow::getWidth() / 2), (f32)SPWindow::getWidth() / 2, (f32)SPWindow::getHeight() / 2, -((f32)SPWindow::getHeight() / 2), -64.0f, 64.0f);
			material = material.val();
			new_renderable.mvp = proj * view * model;
			new_renderable.material = material.val();
			new_renderable.layer = transform.layer;
			return new_renderable;
		}
		else if (transform.anchor == SP_ANCHOR_VIEWPORT) {

			/*
			Position the prop anchored to the viewport in normalized view coordinates (-1.0 <-> 1.0).
			*/

			glm::vec2 viewport_norm;
			viewport_norm.x = (((f32)SPWindow::getWidth() / 2) / (SP_UNIT_PIXELS));
			viewport_norm.y = (((f32)SPWindow::getHeight() / 2) / (SP_UNIT_PIXELS));
			viewport_norm.x *= transform.pos.x * SP_UNIT_PIXELS;
			viewport_norm.y *= -transform.pos.y * SP_UNIT_PIXELS;

			SPSprite spr = material.val().sprite;

			glm::mat4 model(1.0f);
			model = glm::translate(model, glm::vec3(origin_offset.x, origin_offset.y, 0.0f));
			model = glm::translate(model, glm::vec3(viewport_norm.x, viewport_norm.y, 0.0f));
			model = glm::rotate(model, transform.rot, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(transform.scale.x * material.val().sprite.size_exact.x, transform.scale.y * material.val().sprite.size_exact.y, 0.0f));
			glm::mat4 view(1.0f);
			glm::mat4 proj(1.0f);
			proj = glm::ortho(-((f32)SPWindow::getWidth() / 2), ((f32)SPWindow::getWidth() / 2), ((f32)SPWindow::getHeight() / 2), -((f32)SPWindow::getHeight() / 2), -64.0f, 64.0f);
			material = material.val();
			new_renderable.mvp = proj * view * model;
			new_renderable.material = material.val();
			new_renderable.layer = transform.layer;
			return new_renderable;

		}

	} else {
		std::cout << "WARNING : Renderable created with no valid sprite. Potential wasted draw." << std::endl;
		return {};
	}
}