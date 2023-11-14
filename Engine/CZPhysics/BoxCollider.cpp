#include "BoxCollider.h"
#include "SphereCollider.h"

#include <Engine/CZPhysics/CZPhysics.h>

#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/Assets/GameObject.h>

#include <cfloat>
#include <algorithm>

ChrisZ::Physics::BoxCollider::BoxCollider(eae6320::Math::sVector i_center, eae6320::Math::sVector i_extents, eae6320::Assets::GameObject* i_gameObject) : Collider(i_gameObject)
{
	this->center = i_center;
	this->extents = i_extents;
}

ChrisZ::Physics::CollisionInfo ChrisZ::Physics::BoxCollider::Intersects(Collider* other)
{
    // If the other collider is a sphere collider, use the sphere-box intersection formula
    if (SphereCollider* otherSphere = dynamic_cast<SphereCollider*>(other))
    {
        // Get the closest point on the box to the sphere center
        eae6320::Math::sVector closestPoint = this->ClosestPoint(otherSphere->GetCenter());

        // Get the distance between the closest point and the sphere center
        float distance = (otherSphere->GetCenter() - closestPoint).GetLength();

        // If the distance is less than or equal to the radius, the sphere and the box intersect
        if (distance <= otherSphere->GetRadius())
        {
            // Calculate and return CollisionInfo
            eae6320::Math::sVector contactNormal = (closestPoint - otherSphere->GetCenter()).GetNormalized();
            float penetrationDepth = otherSphere->GetRadius() - distance;
            return CollisionInfo(contactNormal, penetrationDepth);
        }
    }

    // If the other collider is a box collider, use the box-box intersection detections
    if (BoxCollider* otherBox = dynamic_cast<BoxCollider*>(other))
    {
        // Use the bounding sphere detection as a broad phase detection to check if the boxes have a possibility of intersecting
        if (!this->BoundingSphereDetection(otherBox))
        {
            // No collision
            return CollisionInfo(eae6320::Math::sVector(0.0f, 0.0f, 0.0f), 0.0f);
        }

        // Use the AABB method as a middle phase detection to check if the boxes have a possibility of intersecting
        if (!this->AABBDetection(otherBox))
        {
            // No collision
            return CollisionInfo(eae6320::Math::sVector(0.0f, 0.0f, 0.0f), 0.0f);
        }

        // Use the SAT method to check if the boxes intersect and return the CollisionInfo
        return this->SATDetection(otherBox);
    }

    // No collision
    return CollisionInfo(eae6320::Math::sVector(0.0f, 0.0f, 0.0f), 0.0f);
}

// Helper Methods
//===============

eae6320::Math::sVector ChrisZ::Physics::BoxCollider::ClosestPoint(eae6320::Math::sVector point)
{
    // Transform the point to the local space of the box
    eae6320::Math::sVector localPoint = this->gameObject->GetOrientation().GetInverse() * (point - this->center);

    // Clamp the point to the extents of the box
    eae6320::Math::sVector localClosestPoint;
    localClosestPoint.x = std::clamp(localPoint.x, -this->extents.x / 2, this->extents.x / 2);
    localClosestPoint.y = std::clamp(localPoint.y, -this->extents.y / 2, this->extents.y / 2);
    localClosestPoint.z = std::clamp(localPoint.z, -this->extents.z / 2, this->extents.z / 2);

    // Transform the point back to the world space
    eae6320::Math::sVector worldClosestPoint = this->center + this->gameObject->GetOrientation() * localClosestPoint;

    // Return the closest point
    return worldClosestPoint;
}

void ChrisZ::Physics::BoxCollider::ProjectOntoAxis(eae6320::Math::sVector axis, float& min, float& max)
{
    // Normalize the axis
    axis.Normalize();

    // Initialize the min and max values
    min = FLT_MAX;
    max = -FLT_MAX;

    // For each vertex of the box
    for (int i = 0; i < 8; i++)
    {
        // Get the vertex position
        eae6320::Math::sVector vertex = center + gameObject->GetOrientation() * eae6320::Math::sVector(
			(i & 1) ? extents.x : -extents.x,
			(i & 2) ? extents.y : -extents.y,
			(i & 4) ? extents.z : -extents.z
		);

        // Rotate the vertex by the orientation of the box
        vertex = gameObject->GetOrientation() * vertex;

        // Project the vertex onto the axis
        float projection = eae6320::Math::Dot(vertex, axis);

        // Update the min and max values
        if (projection < min)
        {
            min = projection;
        }
        if (projection > max)
        {
            max = projection;
        }
    }
}

bool ChrisZ::Physics::BoxCollider::BoundingSphereDetection(BoxCollider* other)
{
    // Get the distance between the centers of the two boxes
    eae6320::Math::sVector distance = center - other->GetCenter();

    // Get the sum of the radii of the two bounding spheres
    float sumRadii = extents.GetLength() + other->extents.GetLength();

    // Check if the distance is less than the sum of the radii
    if (distance.GetLength() < sumRadii)
    {
        // There is a possibility of intersection
        return true;
    }
    else
    {
        // There is no intersection
        return false;
    }
}

bool ChrisZ::Physics::BoxCollider::AABBDetection(BoxCollider* other)
{
    // Get the orientation matrices of the two boxes
    eae6320::Math::cMatrix_transformation localToWorld = eae6320::Math::cMatrix_transformation(gameObject->GetOrientation(), center);
    eae6320::Math::cMatrix_transformation otherLocalToWorld = eae6320::Math::cMatrix_transformation(other->GetGameObject()->GetOrientation(), other->GetCenter());

    // Get the world axes
    eae6320::Math::sVector axes[3];
    axes[0] = eae6320::Math::sVector(1.0f, 0.0f, 0.0f);
    axes[1] = eae6320::Math::sVector(0.0f, 1.0f, 0.0f);
    axes[2] = eae6320::Math::sVector(0.0f, 0.0f, 1.0f);

    // Initialize the min and max points of the two axis aligned bounding boxes
    eae6320::Math::sVector minA, maxA, minB, maxB;
    minA = maxA = center;
    minB = maxB = other->GetCenter();

    // For each axis
    for (int i = 0; i < 3; i++)
    {
        // Project the two boxes onto the axis
        float minA1, maxA1, minB1, maxB1;
        ProjectOntoAxis(axes[i], minA1, maxA1);
        other->ProjectOntoAxis(axes[i], minB1, maxB1);

        // Update the min and max points of the two axis aligned bounding boxes
        minA = minA + axes[i] * minA1;
        maxA = maxA + axes[i] * maxA1;
        minB = minB + axes[i] * minB1;
        maxB = maxB + axes[i] * maxB1;
    }

    // Check if the min and max points overlap along each axis
    if (maxA.x > minB.x && minA.x < maxB.x && maxA.y > minB.y && minA.y < maxB.y && maxA.z > minB.z && minA.z < maxB.z)
    {
        // There is a possibility of intersection
        return true;
    }
    else
    {
        // There is no intersection
        return false;
    }
}

ChrisZ::Physics::CollisionInfo ChrisZ::Physics::BoxCollider::SATDetection(BoxCollider* otherBox)
{
    // Get the orientation matrices of the two boxes
    eae6320::Math::cMatrix_transformation localToWorld = eae6320::Math::cMatrix_transformation(gameObject->GetOrientation(), center);
    eae6320::Math::cMatrix_transformation otherLocalToWorld = eae6320::Math::cMatrix_transformation(otherBox->GetGameObject()->GetOrientation(), otherBox->GetCenter());

    // Get the axes of the two boxes
    eae6320::Math::sVector axes[15];
    axes[0] = localToWorld.GetRightDirection();
    axes[1] = localToWorld.GetUpDirection();
    axes[2] = localToWorld.GetBackDirection();
    axes[3] = otherLocalToWorld.GetRightDirection();
    axes[4] = otherLocalToWorld.GetUpDirection();
    axes[5] = otherLocalToWorld.GetBackDirection();

    // Get the cross products of the axes
    for (int i = 0; i < 3; i++)
    {
        for (int j = 3; j < 6; j++)
        {
            axes[6 + i * 3 + j - 3] = eae6320::Math::Cross(axes[i], axes[j]);
        }
    }

    // Initialize MTV variables
    float minOverlap = FLT_MAX;
    eae6320::Math::sVector mtv(0.0f, 0.0f, 0.0f);

    // For each axis
    for (int i = 0; i < 15; i++)
    {
        // Project the two boxes onto the axis
        float minA, maxA, minB, maxB;
        ProjectOntoAxis(axes[i], minA, maxA);
        otherBox->ProjectOntoAxis(axes[i], minB, maxB);

        // Check if the projections overlap
        if (maxA < minB || maxB < minA)
        {
            // There is no overlap on this axis, so there is no intersection
            mtv = eae6320::Math::sVector(0.0f, 0.0f, 0.0f);
            break;
        }
        else
        {
            // Calculate the overlap on this axis
            float overlap = std::min(maxA, maxB) - std::max(minA, minB);

            // Update the minimum overlap and the MTV
            if (overlap < minOverlap)
            {
                minOverlap = overlap;
                mtv = axes[i] * overlap;
            }
        }
    }

    // If there is an intersection, return CollisionInfo
    if (mtv.GetLength() * mtv.GetLength() > 0.0f)
    {
        eae6320::Math::sVector contactNormal = mtv.GetNormalized();
        float penetrationDepth = mtv.GetLength();
        return CollisionInfo(contactNormal, penetrationDepth);
    }
    else
    {
        // No collision
        return CollisionInfo(eae6320::Math::sVector(0.0f, 0.0f, 0.0f), 0.0f);
    }
}