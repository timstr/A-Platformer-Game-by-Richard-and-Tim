#pragma once

#include "RigidBody.hpp"

namespace phys {

	struct RasterBody : RigidBody {
		RasterBody(vec2 size, std::shared_ptr<sf::Image> _image, float elasticity);

		BoundingBox getBoundingBox() const override;

		// returns true if the point (in world coordinates) intersects with the body
		bool hit(vec2 point) const;

	private:

		// TODO: use vector<Octree
		sf::Image m_image;
		const vec2 m_size;
	};

} // namespace phys