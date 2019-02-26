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
        auto& circ = static_cast<CircleBody&>(body_a);
        auto& conv = static_cast<ConvexBody&>(body_b);
        
		// cp is the the circle's position in the rectangle's coordinates
		const vec2 cp = conv.getTransform() * (circ.getPosition() - conv.getPosition());

		const auto& corners = conv.getPoints();
		
		// distance squared from each corner to circle's center
		std::vector<float> dists_sqr;
		dists_sqr.resize(corners.size());
		for (int i = 0; i < dists_sqr.size(); ++i){
			vec2 disp = cp - corners[i];
			dists_sqr[i] = abssqr(disp);
		}

		int min_corner = std::min_element(dists_sqr.begin(), dists_sqr.end()) - dists_sqr.begin();

		if (dists_sqr[min_corner] <= circ.radius() * circ.radius()){
			// found a corner collision
			float dist = sqrt(dists_sqr[min_corner]);
			float depth = circ.radius() - dist;
			vec2 normal = conv.getInverseTransform() * ((corners[min_corner] - cp) / dist);
			vec2 r_conv = conv.getInverseTransform() * corners[min_corner];
			vec2 r_circ = r_conv + conv.getPosition() - circ.getPosition();
			collisions.emplace_back(CollisionConstraint{circ, conv, normal, r_circ, r_conv, depth});
			return;
		}


		std::vector<float> face_disp;
		face_disp.resize(corners.size());
		for (int i = 0; i < corners.size(); ++i){
			const auto& p0 = corners[i];
			const auto& p1 = corners[(i + 1) % corners.size()];
			float disp = displacementFromLinePN(p0, unit(orthogonalCW(p1 - p0)), cp);
			float min = disp - circ.radius();
			float max = disp + circ.radius();
			if (min > 0.0f){
				return;
			}
			float val = (min < 0.0f ? 1000.0f * min : min) + (max < 0.0f ? 1000.0f * max : max);
			face_disp[i] = val;
		}
		
		int max_face = std::max_element(face_disp.begin(), face_disp.end()) - face_disp.begin();

		const auto& p0 = corners[max_face];
		const auto& p1 = corners[(max_face + 1) % corners.size()];

		vec2 nearest = projectOnto(cp - p0, p1 - p0) + p0;

		// make sure nearest point is inside the edge
		const float t = (abs(p1.x - p0.x) > 1e-6f) ?
			(nearest.x - p0.x) / (p1.x - p0.x) :
			(nearest.y - p0.y) / (p1.y - p0.y);
		if (t < 0.0f || t > 1.0f){
			return;
		}

		vec2 r_conv = conv.getInverseTransform() * nearest;

		vec2 normal_conv = unit(orthogonalCCW(p1 - p0));
		vec2 normal = conv.getInverseTransform() * normal_conv;
		float depth = displacementFromLinePN(p0, normal_conv, cp) + circ.radius();
		vec2 r_circ = r_conv + conv.getPosition() - circ.getPosition();
		collisions.emplace_back(CollisionConstraint{circ, conv, normal, r_circ, r_conv, depth});
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
        auto& rect = static_cast<RectangleBody&>(body_a);
        auto& conv = static_cast<ConvexBody&>(body_b);
        
		// corner points of rectangle, in world space, in clockwise order
		const vec2 rect_p[4] = {
			rect.getPosition() + (rect.getInverseTransform() * vec2{-rect.size().x, -rect.size().y} * 0.5f),
			rect.getPosition() + (rect.getInverseTransform() * vec2{rect.size().x, -rect.size().y} * 0.5f),
			rect.getPosition() + (rect.getInverseTransform() * vec2{rect.size().x, rect.size().y} * 0.5f),
			rect.getPosition() + (rect.getInverseTransform() * vec2{-rect.size().x, rect.size().y} * 0.5f)
		};

		// corner points of convex body in local space
		const auto& conv_p_body = conv.getPoints();

		// corner points of convex body, in world space, in clockwise order
		std::vector<vec2> conv_p{conv_p_body.size()};
		for (int i = 0; i < conv_p_body.size(); ++i){
			conv_p[i] = conv.getPosition() + conv.getInverseTransform() * conv_p_body[i];
		}
		
		// iterate over corners of rectangle
		for (int i = 0; i < 4; ++i){
			// if rect's corner hits convex...
			if (conv.hit(rect_p[i])){
				int max = -1;
				float maxdist = -1e6f;
				for (int j = 0; j < conv_p.size(); ++j){
					vec2 normal = unit(orthogonalCCW(conv_p[j] - conv_p[(j + 1) % conv_p.size()]));
					float dist = 0.0f;
					for (int k = 0; k < 4; ++k){
						float disp = displacementFromLinePN(conv_p[j], normal, rect_p[k]);;
						dist += disp < 0.0f ? 1000.0f * disp : disp;
					}
					if (dist > maxdist){
						maxdist = dist;
						max = j;
					}
				}

				vec2 normal = unit(orthogonalCW(conv_p[max] - conv_p[(max + 1) % conv_p.size()]));
				vec2 r_rect = rect_p[i] - rect.getPosition();
				vec2 r_conv = rect_p[i] - conv.getPosition();
				float depth = displacementFromLinePN(conv_p[max], normal, rect_p[i]);
				collisions.emplace_back(CollisionConstraint{rect, conv, normal, r_rect, r_conv, depth});
			}
		}

		// iterate over corners of convex body
		for (int i = 0; i < conv_p.size(); ++i){
			// if convex body's corner hits rect...
			if (rect.hit(conv_p[i])){
				int max = -1;
				float maxdist = -1e6f;
				for (int j = 0; j < 4; ++j){
					vec2 normal = unit(orthogonalCCW(rect_p[j] - rect_p[(j + 1) % 4]));
					float dist = 0.0f;
					for (int k = 0; k < conv_p.size(); ++k){
						float disp = displacementFromLinePN(rect_p[j], normal, conv_p[k]);
						dist += disp < 0.0f ? 1000.0f * disp : disp;
					}
					if (dist > maxdist){
						maxdist = dist;
						max = j;
					}
				}

				vec2 normal = unit(orthogonalCCW(rect_p[max] - rect_p[(max + 1) % 4]));
				vec2 r_rect = conv_p[i] - rect.getPosition();
				vec2 r_conv = conv_p[i] - conv.getPosition();
				float depth = displacementFromLinePN(rect_p[max], -normal, conv_p[i]);
				collisions.emplace_back(CollisionConstraint{rect, conv, normal, r_rect, r_conv, depth});
			}
		}
    }

    void collideRectangleRaster(RigidBody& body_a, RigidBody& body_b, std::vector<CollisionConstraint>& collisions){
        assert(static_cast<bool>(dynamic_cast<RectangleBody*>(&body_a)));
        assert(static_cast<bool>(dynamic_cast<RasterBody*>(&body_b)));
        auto& a = static_cast<RectangleBody&>(body_a);
        auto& b = static_cast<RasterBody&>(body_b);
        
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
        
		// corner in local space
		const auto& a_p_body = a.getPoints();
		const auto& b_p_body = b.getPoints();

		// corner points of a, in world space, in clockwise order
		std::vector<vec2> a_p{a_p_body.size()};
		for (int i = 0; i < a_p_body.size(); ++i){
			a_p[i] = a.getPosition() + a.getInverseTransform() * a_p_body[i];
		}

		// corner points of b, in world space, in clockwise order
		std::vector<vec2> b_p{b_p_body.size()};
		for (int i = 0; i < b_p_body.size(); ++i){
			b_p[i] = b.getPosition() + b.getInverseTransform() * b_p_body[i];
		}

		// iterate over corners of a
		for (int i = 0; i < a_p.size(); ++i){
			// if a's corner hits b...
			if (b.hit(a_p[i])){
				int max = -1;
				float maxdist = -1e6f;
				for (int j = 0; j < b_p.size(); ++j){
					vec2 normal = unit(orthogonalCCW(b_p[j] - b_p[(j + 1) % b_p.size()]));
					float dist = 0.0f;
					for (int k = 0; k < a_p.size(); ++k){
						float disp = displacementFromLinePN(b_p[j], normal, a_p[k]);;
						dist += disp < 0.0f ? 1000.0f * disp : disp;
					}
					if (dist > maxdist){
						maxdist = dist;
						max = j;
					}
				}

				vec2 normal = unit(orthogonalCW(b_p[max] - b_p[(max + 1) % b_p.size()]));
				vec2 r_a = a_p[i] - a.getPosition();
				vec2 r_b = a_p[i] - b.getPosition();
				float depth = displacementFromLinePN(b_p[max], normal, a_p[i]);
				collisions.emplace_back(CollisionConstraint{a, b, normal, r_a, r_b, depth});
			}
		}

		// iterate over b's corners
		for (int i = 0; i < b_p.size(); ++i){
			// if b's corner hits a...
			if (a.hit(b_p[i])){
				int max = -1;
				float maxdist = -1e6f;
				for (int j = 0; j < a_p.size(); ++j){
					vec2 normal = unit(orthogonalCCW(a_p[j] - a_p[(j + 1) % a_p.size()]));
					float dist = 0.0f;
					for (int k = 0; k < b_p.size(); ++k){
						float disp = displacementFromLinePN(a_p[j], normal, b_p[k]);
						dist += disp < 0.0f ? 1000.0f * disp : disp;
					}
					if (dist > maxdist){
						maxdist = dist;
						max = j;
					}
				}

				vec2 normal = unit(orthogonalCCW(a_p[max] - a_p[(max + 1) % a_p.size()]));
				vec2 r_a = b_p[i] - a.getPosition();
				vec2 r_b = b_p[i] - b.getPosition();
				float depth = displacementFromLinePN(a_p[max], -normal, b_p[i]);
				collisions.emplace_back(CollisionConstraint{a, b, normal, r_a, r_b, depth});
			}
		}
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