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

        if (dist > a.radius() + b.radius()){
            return;
        }

        vec2 normal = diff / dist;

        vec2 r_a = normal * a.radius();
        vec2 r_b = -normal * b.radius();

		collisions.emplace_back(CollisionConstraint{a, b, normal, r_a, r_b});
    }

    void collideCircleRectangle(RigidBody& body_a, RigidBody& body_b, std::vector<CollisionConstraint>& collisions){
        assert(static_cast<bool>(dynamic_cast<CircleBody*>(&body_a)));
        assert(static_cast<bool>(dynamic_cast<RectangleBody*>(&body_b)));
        auto& circle = static_cast<CircleBody&>(body_a);
        auto& rect = static_cast<RectangleBody&>(body_b);

        // cp is the the circle's position in the rectangle's coordinates
        const vec2 cp = rect.getTransform() * (circle.getPosition() - rect.getPosition());

        // displacement from the circle's edge to the left, right, top, and bottom edges of the rectangle
        // positive means the circle is outside that edge
        const float dl = -cp.x - circle.radius() - rect.size().x * 0.5f;
        const float dr = cp.x - circle.radius() - rect.size().x * 0.5f;
        const float dt = -cp.y - circle.radius() - rect.size().y * 0.5f;
        const float db = cp.y - circle.radius() - rect.size().y * 0.5f;

        if (dl > 0.0f || dr > 0.0f || dt > 0.0f || db > 0.f){
            return;
        }

        const vec2 hitp = (rect.getInverseTransform() * vec2{
            std::clamp(cp.x, -rect.size().x * 0.5f, rect.size().x * 0.5f),
            std::clamp(cp.y, -rect.size().y * 0.5f, rect.size().y * 0.5f)
        }) + rect.getPosition();

            vec2 normal = {1.0f, 0.0f};
            float mindist = dl;
            if (dr <= 0.0f && dr > mindist){
                normal = {-1.0f, 0.0f};
                mindist = dr;
            }
            if (dt <= 0.0f && dt > mindist){
                normal = {0.0f, 1.0f};
                mindist = dt;
            }
            if (db <= 0.0f && db > mindist){
                normal = {0.0f, -1.0f};
                mindist = db;
            }

            const vec2 tl = vec2{-rect.size().x, -rect.size().y} * 0.5f;
            const vec2 tr = vec2{rect.size().x, -rect.size().y} * 0.5f;
            const vec2 bl = vec2{-rect.size().x, rect.size().y} * 0.5f;
            const vec2 br = vec2{rect.size().x, rect.size().y} * 0.5f;

            const float rs = circle.radius() * circle.radius();
            const float dtl = abssqr(tl - cp);
            const float dtr = abssqr(tr - cp);
            const float dbl = abssqr(bl - cp);
            const float dbr = abssqr(br - cp);

            mindist = 1e6f;

            if (dtl < rs && dtl < mindist){
                mindist = dtl;
                normal = unit(tl - cp);
            }
            if (dtr < rs && dtr < mindist){
                mindist = dtr;
                normal = unit(tr - cp);
            }
            if (dbl < rs && dbl < mindist){
                mindist = dbl;
                normal = unit(bl - cp);
            }
            if (dbr < rs && dbr < mindist){
                mindist = dbr;
                normal = unit(br - cp);
            }

            normal = rect.getInverseTransform() * normal;

			collisions.emplace_back(CollisionConstraint{circle, rect, normal, hitp - circle.getPosition(), hitp - rect.getPosition()});
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
						dist += displacementFromLinePN(b_p[j], normal, a_p[k]);
					}
					if (dist > maxdist){
						maxdist = dist;
						max = j;
					}
				}

   				vec2 normal = unit(orthogonalCW(b_p[max] - b_p[(max + 1) % 4]));
				vec2 r_a = a_p[i] - a.getPosition();
				vec2 r_b = a_p[i] - b.getPosition();
				collisions.emplace_back(CollisionConstraint{a, b, normal, r_a, r_b});
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
						dist += displacementFromLinePN(a_p[j], normal, b_p[k]);
					}
					if (dist > maxdist){
						maxdist = dist;
						max = j;
					}
				}

				vec2 normal = unit(orthogonalCCW(a_p[max] - a_p[(max + 1) % 4]));
				vec2 r_a = b_p[i] - a.getPosition();
				vec2 r_b = b_p[i] - b.getPosition();
				collisions.emplace_back(CollisionConstraint{a, b, normal, r_a, r_b});
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