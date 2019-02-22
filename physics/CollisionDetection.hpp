#pragma once

#include <memory>

#include "Constraint.hpp"
#include "RigidBody.hpp"

namespace phys {

    void collideCircleCircle(RigidBody& a, RigidBody& b, std::vector<CollisionConstraint>& collisions);
	
    void collideCircleRectangle(RigidBody& a, RigidBody& b, std::vector<CollisionConstraint>& collisions);

    void collideCircleConvex(RigidBody& a, RigidBody& b, std::vector<CollisionConstraint>& collisions);

    void collideCircleRaster(RigidBody& a, RigidBody& b, std::vector<CollisionConstraint>& collisions);



    void collideRectangleCircle(RigidBody& a, RigidBody& b, std::vector<CollisionConstraint>& collisions);

    void collideRectangleRectangle(RigidBody& a, RigidBody& b, std::vector<CollisionConstraint>& collisions);

    void collideRectangleConvex(RigidBody& a, RigidBody& b, std::vector<CollisionConstraint>& collisions);

    void collideRectangleRaster(RigidBody& a, RigidBody& b, std::vector<CollisionConstraint>& collisions);



    void collideConvexCircle(RigidBody& a, RigidBody& b, std::vector<CollisionConstraint>& collisions);

    void collideConvexRectangle(RigidBody& a, RigidBody& b, std::vector<CollisionConstraint>& collisions);

    void collideConvexConvex(RigidBody& a, RigidBody& b, std::vector<CollisionConstraint>& collisions);

    void collideConvexRaster(RigidBody& a, RigidBody& b, std::vector<CollisionConstraint>& collisions);



    void collideRasterCircle(RigidBody& a, RigidBody& b, std::vector<CollisionConstraint>& collisions);

    void collideRasterRectangle(RigidBody& a, RigidBody& b, std::vector<CollisionConstraint>& collisions);

    void collideRasterConvex(RigidBody& a, RigidBody& b, std::vector<CollisionConstraint>& collisions);

    void collideRasterRaster(RigidBody& a, RigidBody& b, std::vector<CollisionConstraint>& collisions);

}