#include "CollisionDetection.hpp"
#include "CircleBody.hpp"
#include "RectangleBody.hpp"
#include "ConvexBody.hpp"
#include "RasterBody.hpp"

#include <cassert>

namespace phys {

    void collideCircleCircle(RigidBody& body_a, RigidBody& body_b, std::vector<CollisionConstraint>& collisions){
        assert(static_cast<bool>(dynamic_cast<CircleBody*>(&body_a)));
        assert(static_cast<bool>(dynamic_cast<CircleBody*>(&body_b)));
        auto& a = static_cast<CircleBody&>(body_a);
        auto& b = static_cast<CircleBody&>(body_b);

        vec2 diff = b.getPosition() - a.getPosition();
        float dist = abs(diff);

		float depth = a.radius() + b.radius() - dist;

        if (depth < 0.0f){
            return;
        }

        vec2 normal = diff / dist;

        vec2 r_a = normal * a.radius();
        vec2 r_b = -normal * b.radius();

		collisions.emplace_back(CollisionConstraint{a, b, normal, r_a, r_b, depth});
    }

    void collideCircleRectangle(RigidBody& body_a, RigidBody& body_b, std::vector<CollisionConstraint>& collisions){
        assert(static_cast<bool>(dynamic_cast<CircleBody*>(&body_a)));
        assert(static_cast<bool>(dynamic_cast<RectangleBody*>(&body_b)));
        auto& circ = static_cast<CircleBody&>(body_a);
        auto& rect = static_cast<RectangleBody&>(body_b);

        // cp is the the circle's position in the rectangle's coordinates
        const vec2 cp = rect.getTransform() * (circ.getPosition() - rect.getPosition());

		const vec2 corners[4] = {
			{-rect.size().x * 0.5f, -rect.size().y * 0.5f},
			{-rect.size().x * 0.5f, rect.size().y * 0.5f},
			{rect.size().x * 0.5f, -rect.size().y * 0.5f},
			{rect.size().x * 0.5f, rect.size().y * 0.5f}
		};

		// distance squared from each corner to circle's center
		float dists_sqr[4];
		for (int i = 0; i < 4; ++i){
			vec2 disp = cp - corners[i];
			dists_sqr[i] = abssqr(disp);
		}

		int min_corner = std::min_element(dists_sqr, dists_sqr + 4) - dists_sqr;

		if (dists_sqr[min_corner] <= circ.radius() * circ.radius()){
			// found a corner collision
			float dist = sqrt(dists_sqr[min_corner]);
			float depth = circ.radius() - dist;
			vec2 normal = rect.getInverseTransform() * ((corners[min_corner] - cp) / dist);
			vec2 r_rect = rect.getInverseTransform() * corners[min_corner];
			vec2 r_circ = r_rect + rect.getPosition() - circ.getPosition();
			collisions.emplace_back(CollisionConstraint{circ, rect, normal, r_circ, r_rect, depth});
			return;
		}

        // displacement from the circle's edge to the left, right, top, and bottom edges of the rectangle
        // positive means the circle is outside that edge
		const float face_disp[4] = {
			-cp.x - circ.radius() - rect.size().x * 0.5f,
			cp.x - circ.radius() - rect.size().x * 0.5f,
			-cp.y - circ.radius() - rect.size().y * 0.5f,
			cp.y - circ.radius() - rect.size().y * 0.5f
		};

		int max_face = std::max_element(face_disp, face_disp + 4) - face_disp;

		if (face_disp[max_face] > 0.0f){
			return;
		}

		if (abs(cp.x) > rect.size().x * 0.5f && abs(cp.y) > rect.size().y * 0.5f){
			return;
		}

		vec2 r_rect = rect.getInverseTransform() * vec2{
			std::clamp(cp.x, -rect.size().x * 0.5f, rect.size().x * 0.5f),
			std::clamp(cp.y, -rect.size().y * 0.5f, rect.size().y * 0.5f)
		};

		const vec2 normals[4] = {
			{-1.0f, 0.0f},
			{1.0f, 0.0f},
			{0.0f, -1.0f},
			{0.0f, 1.0f}
		};

		float depth = -face_disp[max_face];
		vec2 normal = rect.getInverseTransform() * -normals[max_face];
		vec2 r_circ = r_rect + rect.getPosition() - circ.getPosition();
		collisions.emplace_back(CollisionConstraint{circ, rect, normal, r_circ, r_rect, depth});
    }

    void collideCircleConvex(RigidBody& body_a, RigidBody& body_b, std::vector<CollisionConstraint>& collisions){
        assert(static_cast<bool>(dynamic_cast<CircleBody*>(&body_a)));
        assert(static_cast<bool>(dynamic_cast<ConvexBody*>(&body_b)));
        auto& a = static_cast<CircleBody&>(body_a);
        auto& b = static_cast<ConvexBody&>(body_b);
        // TODO
        throw std::runtime_error("Not implemented");
    }

    void collideCircleRaster(RigidBody& body_a, RigidBody& body_b, std::vector<CollisionConstraint>& collisions){
        assert(static_cast<bool>(dynamic_cast<CircleBody*>(&body_a)));
        assert(static_cast<bool>(dynamic_cast<RasterBody*>(&body_b)));
        auto& a = static_cast<CircleBody&>(body_a);
        auto& b = static_cast<RasterBody&>(body_b);
        // TODO
        throw std::runtime_error("Not implemented");
    }

    void collideRectangleCircle(RigidBody& body_a, RigidBody& body_b, std::vector<CollisionConstraint>& collisions){
        collideCircleRectangle(body_b, body_a, collisions);
    }

    void collideRectangleRectangle(RigidBody& body_a, RigidBody& body_b, std::vector<CollisionConstraint>& collisions){
		assert(static_cast<bool>(dynamic_cast<RectangleBody*>(&body_a)));
        assert(static_cast<bool>(dynamic_cast<RectangleBody*>(&body_b)));
        auto& a = static_cast<RectangleBody&>(body_a);
        auto& b = static_cast<RectangleBody&>(body_b);

        // corner points of a, in world space, in clockwise order
        const vec2 a_p[4] = {
            a.getPosition() + (a.getInverseTransform() * vec2{-a.size().x, -a.size().y} * 0.5f),
            a.getPosition() + (a.getInverseTransform() * vec2{a.size().x, -a.size().y} * 0.5f),
            a.getPosition() + (a.getInverseTransform() * vec2{a.size().x, a.size().y} * 0.5f),
            a.getPosition() + (a.getInverseTransform() * vec2{-a.size().x, a.size().y} * 0.5f)
        };

        // corner points of b, in world space, in clockwise order
        const vec2 b_p[4] = {
            b.getPosition() + (b.getInverseTransform() * vec2{-b.size().x, -b.size().y} * 0.5f),
            b.getPosition() + (b.getInverseTransform() * vec2{b.size().x, -b.size().y} * 0.5f),
            b.getPosition() + (b.getInverseTransform() * vec2{b.size().x, b.size().y} * 0.5f),
            b.getPosition() + (b.getInverseTransform() * vec2{-b.size().x, b.size().y} * 0.5f)
        };

		// iterate over corners of a
		for (int i = 0; i < 4; ++i){
			// if a's corner hits b...
			if (b.hit(a_p[i])){
				int max = -1;
				float maxdist = -1e6f;
				for (int j = 0; j < 4; ++j){
					vec2 normal = unit(orthogonalCCW(b_p[j] - b_p[(j + 1) % 4]));
					float dist = 0.0f;
					for (int k = 0; k < 4; ++k){
						float disp = displacementFromLinePN(b_p[j], normal, a_p[k]);;
						dist += disp < 0.0f ? 1000.0f * disp : disp;
					}
					if (dist > maxdist){
						maxdist = dist;
						max = j;
					}
				}

   				vec2 normal = unit(orthogonalCW(b_p[max] - b_p[(max + 1) % 4]));
				vec2 r_a = a_p[i] - a.getPosition();
				vec2 r_b = a_p[i] - b.getPosition();
				float depth = displacementFromLinePN(b_p[max], normal, a_p[i]);
				collisions.emplace_back(CollisionConstraint{a, b, normal, r_a, r_b, depth});
			}
		}

		// iterate over corners of b
		for (int i = 0; i < 4; ++i){
			// if b's corner hits a...
			if (a.hit(b_p[i])){
				int max = -1;
				float maxdist = -1e6f;
				for (int j = 0; j < 4; ++j){
					vec2 normal = unit(orthogonalCCW(a_p[j] - a_p[(j + 1) % 4]));
					float dist = 0.0f;
					for (int k = 0; k < 4; ++k){
						float disp = displacementFromLinePN(a_p[j], normal, b_p[k]);
						dist += disp < 0.0f ? 1000.0f * disp : disp;
					}
					if (dist > maxdist){
						maxdist = dist;
						max = j;
					}
				}

				vec2 normal = unit(orthogonalCCW(a_p[max] - a_p[(max + 1) % 4]));
				vec2 r_a = b_p[i] - a.getPosition();
				vec2 r_b = b_p[i] - b.getPosition();
				float depth = displacementFromLinePN(a_p[max], -normal, b_p[i]);
				collisions.emplace_back(CollisionConstraint{a, b, normal, r_a, r_b, depth});
			}
		}
    }

    void collideRectangleConvex(RigidBody& body_a, RigidBody& body_b, std::vector<CollisionConstraint>& collisions){
        assert(static_cast<bool>(dynamic_cast<RectangleBody*>(&body_a)));
        assert(static_cast<bool>(dynamic_cast<ConvexBody*>(&body_b)));
        auto& a = static_cast<RectangleBody&>(body_a);
        auto& b = static_cast<ConvexBody&>(body_b);
        // TODO
        throw std::runtime_error("Not implemented");
    }

    void collideRectangleRaster(RigidBody& body_a, RigidBody& body_b, std::vector<CollisionConstraint>& collisions){
        assert(static_cast<bool>(dynamic_cast<RectangleBody*>(&body_a)));
        assert(static_cast<bool>(dynamic_cast<RasterBody*>(&body_b)));
        auto& a = static_cast<RectangleBody&>(body_a);
        auto& b = static_cast<RasterBody&>(body_b);
        // TODO
        throw std::runtime_error("Not implemented");
    }

    void collideConvexCircle(RigidBody& body_a, RigidBody& body_b, std::vector<CollisionConstraint>& collisions){
        collideCircleConvex(body_b, body_a, collisions);
    }

    void collideConvexRectangle(RigidBody& body_a, RigidBody& body_b, std::vector<CollisionConstraint>& collisions){
        collideRectangleConvex(body_b, body_a, collisions);
    }

    void collideConvexConvex(RigidBody& body_a, RigidBody& body_b, std::vector<CollisionConstraint>& collisions){
        assert(static_cast<bool>(dynamic_cast<ConvexBody*>(&body_a)));
        assert(static_cast<bool>(dynamic_cast<ConvexBody*>(&body_b)));
        auto& a = static_cast<ConvexBody&>(body_a);
        auto& b = static_cast<ConvexBody&>(body_b);
        // TODO
        throw std::runtime_error("Not implemented");
    }

    void collideConvexRaster(RigidBody& body_a, RigidBody& body_b, std::vector<CollisionConstraint>& collisions){
        assert(static_cast<bool>(dynamic_cast<ConvexBody*>(&body_a)));
        assert(static_cast<bool>(dynamic_cast<RasterBody*>(&body_b)));
        auto& a = static_cast<ConvexBody&>(body_a);
        auto& b = static_cast<RasterBody&>(body_b);
        // TODO
        throw std::runtime_error("Not implemented");
    }

    void collideRasterCircle(RigidBody& body_a, RigidBody& body_b, std::vector<CollisionConstraint>& collisions){
        collideCircleRaster(body_b, body_a, collisions);
    }

    void collideRasterRectangle(RigidBody& body_a, RigidBody& body_b, std::vector<CollisionConstraint>& collisions){
        collideRectangleRaster(body_b, body_a, collisions);
    }

    void collideRasterConvex(RigidBody& body_a, RigidBody& body_b, std::vector<CollisionConstraint>& collisions){
        collideConvexRaster(body_b, body_a, collisions);
    }

    void collideRasterRaster(RigidBody& body_a, RigidBody& body_b, std::vector<CollisionConstraint>& collisions){
        assert(static_cast<bool>(dynamic_cast<RasterBody*>(&body_a)));
        assert(static_cast<bool>(dynamic_cast<RasterBody*>(&body_b)));
        // raster bodies do not collide with other raster bodies
    }

}