#include "BoxCollider.h"
#include "SphereCollider.h"

#include <Engine/CZPhysics/CZPhysics.h>

#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/Assets/GameObject.h>

#include <cfloat>
#include <algorithm>

ChrisZ::Physics::BoxCollider::BoxCollider(eae6320::Math::sVector i_center, eae6320::Math::sVector i_extents, eae6320::Assets::GameObject* i_gameObject) : Collider(i_center, i_gameObject), extents(i_extents)
{
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

        // If the boxes are axis aligned, use the AABB method as the final phase detection
        if (isAxisAligned(this->gameObject->GetOrientation()) && isAxisAligned(otherBox->GetGameObject()->GetOrientation()))
        {
			// Use the AABB method as the final phase detection
			return this->AABBDetectionAxisAligned(otherBox);
		}
        // If the boxes are not axis aligned, use the AABB method as a middle phase detection
		else 
        {
            // Calculate the vertices of the boxes
            CalculateVertices();
            otherBox->CalculateVertices();

            if (!this->AABBDetection(otherBox))
            {
				// No collision
				return CollisionInfo(eae6320::Math::sVector(0.0f, 0.0f, 0.0f), 0.0f);
			}
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

void ChrisZ::Physics::BoxCollider::CalculateVertices()
{
    // Get the orientation matrix of the box
    eae6320::Math::cMatrix_transformation orientation = eae6320::Math::cMatrix_transformation(gameObject->GetOrientation(), center);

    // Get the right, up, and back vectors of the box
    eae6320::Math::sVector right = orientation.GetRightDirection() * extents.x;
    eae6320::Math::sVector up = orientation.GetUpDirection() * extents.y;
    eae6320::Math::sVector back = orientation.GetBackDirection() * extents.z;

    // Calculate the eight vertices of the box by adding or subtracting the right, up, and back vectors from the center
    this->vertices[0] = this->center + right + up + back; // front top right
    this->vertices[1] = this->center + right + up - back; // front top left
    this->vertices[2] = this->center + right - up + back; // front bottom right
    this->vertices[3] = this->center + right - up - back; // front bottom left
    this->vertices[4] = this->center - right + up + back; // back top right
    this->vertices[5] = this->center - right + up - back; // back top left
    this->vertices[6] = this->center - right - up + back; // back bottom right
    this->vertices[7] = this->center - right - up - back; // back bottom left
}

void ChrisZ::Physics::BoxCollider::ProjectOntoAxis(eae6320::Math::sVector axis, float& min, float& max)
{
    // Initialize the min and max values to the first vertex projection
    min = max = eae6320::Math::Dot(this->vertices[0], axis);

    // Loop through the remaining vertices and update the min and max values
    for (int i = 1; i < 8; i++)
    {
        // Project the vertex onto the axis
        float projection = eae6320::Math::Dot(this->vertices[i], axis);

        // Update the min and max values
        if (projection < min)
        {
            min = projection;
        }
        else if (projection > max)
        {
            max = projection;
        }
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

ChrisZ::Physics::CollisionInfo ChrisZ::Physics::BoxCollider::AABBDetectionAxisAligned(BoxCollider* other)
{
    // A common AABB method for axis aligned boxes
    eae6320::Math::sVector minA = this->center - this->extents;
    eae6320::Math::sVector maxA = this->center + this->extents;
    eae6320::Math::sVector minB = other->GetCenter() - other->extents;
    eae6320::Math::sVector maxB = other->GetCenter() + other->extents;

    // Check if the min and max points overlap along each axis
    if (maxA.x > minB.x && minA.x < maxB.x && maxA.y > minB.y && minA.y < maxB.y && maxA.z > minB.z && minA.z < maxB.z)
    {
		// Calculate CollisionInfo

        eae6320::Math::sVector contactNormal;

        // The contact normal is the axis of least penetration
        float penetrationDepth = FLT_MAX;
        if (maxA.x - minB.x < penetrationDepth)
		{
			contactNormal = eae6320::Math::sVector(1.0f, 0.0f, 0.0f);
			penetrationDepth = maxA.x - minB.x;
		}
        if (maxB.x - minA.x < penetrationDepth)
		{
			contactNormal = eae6320::Math::sVector(-1.0f, 0.0f, 0.0f);
			penetrationDepth = maxB.x - minA.x;
		}

		if (maxA.y - minB.y < penetrationDepth)
		{
			contactNormal = eae6320::Math::sVector(0.0f, 1.0f, 0.0f);
			penetrationDepth = maxA.y - minB.y;
		}

		if (maxB.y - minA.y < penetrationDepth)
		{
			contactNormal = eae6320::Math::sVector(0.0f, -1.0f, 0.0f);
			penetrationDepth = maxB.y - minA.y;
		}

		if (maxA.z - minB.z < penetrationDepth)
		{
			contactNormal = eae6320::Math::sVector(0.0f, 0.0f, 1.0f);
			penetrationDepth = maxA.z - minB.z;
		}

		if (maxB.z - minA.z < penetrationDepth)
		{
            contactNormal = eae6320::Math::sVector(0.0f, 0.0f, -1.0f);
			penetrationDepth = maxB.z - minA.z;
		}

		return CollisionInfo(contactNormal, penetrationDepth);
	}
    else
    {
		// No collision
		return CollisionInfo(eae6320::Math::sVector(0.0f, 0.0f, 0.0f), 0.0f);
	}
}

ChrisZ::Physics::CollisionInfo ChrisZ::Physics::BoxCollider::SATDetection(BoxCollider* otherBox)
{
    // Initialize the collision information
    CollisionInfo info(eae6320::Math::sVector(0.0f, 0.0f, 0.0f), 0.0f);

    // Get the axes of the boxes
    eae6320::Math::sVector axes[6];
    // Get the forward vector of this box
    axes[2] = this->gameObject->GetOrientation().CalculateForwardDirection();
    // Get the right vector of this box by crossing the forward vector with the world up vector
    axes[0] = eae6320::Math::Cross(axes[2], eae6320::Math::sVector(0.0f, 1.0f, 0.0f));
    // Get the up vector of this box by crossing the right vector with the forward vector
    axes[1] = eae6320::Math::Cross(axes[0], axes[2]);
    // Get the forward vector of the other box
    axes[5] = otherBox->GetGameObject()->GetOrientation().CalculateForwardDirection();
    // Get the right vector of the other box by crossing the forward vector with the world up vector
    axes[3] = eae6320::Math::Cross(axes[5], eae6320::Math::sVector(0.0f, 1.0f, 0.0f));
    // Get the up vector of the other box by crossing the right vector with the forward vector
    axes[4] = eae6320::Math::Cross(axes[3], axes[5]);

    // Initialize the minimum overlap and the index of the axis with the minimum overlap
    float minOverlap = FLT_MAX;
    int minIndex = -1;

    // Loop through the axes and test for overlap
    for (int i = 0; i < 6; i++)
    {
        // Project the boxes onto the axis
        float minA, maxA, minB, maxB;
        this->ProjectOntoAxis(axes[i], minA, maxA);
        otherBox->ProjectOntoAxis(axes[i], minB, maxB);

        // Check if the projections overlap
        if (minA <= maxB && minB <= maxA)
        {
            // Calculate the overlap
            float overlap = std::min(maxA, maxB) - std::max(minA, minB);

            // Update the minimum overlap and the index
            if (overlap < minOverlap)
            {
                minOverlap = overlap;
                minIndex = i;
            }
        }
        else
        {
            // No overlap, return zero collision information
            return info;
        }
    }

    // If we reach this point, then there is a collision
    // Set the contact normal to the axis with the minimum overlap
    info.contactNormal = axes[minIndex];

    // Set the penetration depth to the minimum overlap
    info.penetrationDepth = minOverlap;

    // Return the collision information
    return info;
}