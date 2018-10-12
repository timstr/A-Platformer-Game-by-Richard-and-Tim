#include "RasterBody.hpp"

namespace phys {

	RasterBody::RasterBody(vec2 size, std::shared_ptr<sf::Image> _image, float elasticity) :
		RigidBody(RigidBody::Raster, 0.0f, 0.0f, elasticity),
		m_size(size) {

	}

	BoundingBox RasterBody::getBoundingBox() const {
		vec2 topleft = getTransform() * (-m_size * 0.5f);
		vec2 bottomleft = getTransform() * (vec2(-m_size.x, m_size.y) * 0.5f);
		vec2 topright = getTransform() * (vec2(-m_size.x, m_size.y) * 0.5f);
		vec2 bottomright = getTransform() * (m_size * 0.5f);

		vec2 min = {
			std::min<float>({topleft.x, bottomleft.x, topright.x, bottomright.x}),
			std::min<float>({topleft.y, bottomleft.y, topright.y, bottomright.y})
		};

		vec2 max = {
			std::max<float>({topleft.x, bottomleft.x, topright.x, bottomright.x}),
			std::max<float>({topleft.y, bottomleft.y, topright.y, bottomright.y})
		};

		return BoundingBox(position + min, position + max);
	}

	bool RasterBody::hit(vec2 point) const {
		vec2 p = getInverseTransform() * point;
		p.x /= m_size.x;
		p.y /= m_size.y;
		p = p * 0.5f + vec2(0.5f, 0.5f);

		int px = floor(p.x * (float)((int)m_image.getSize().x - 1));
		int py = floor(p.y * (float)((int)m_image.getSize().y - 1));
		if (px < 0 || px >= m_image.getSize().x || py < 0 || py >= m_image.getSize().y){
			return false;
		}

		sf::Color color = m_image.getPixel(px, py);
		return ((float)color.r + (float)color.g + (float)color.b) / 3.0f / 255.0f > 0.5f;
	}

} // namespace phys