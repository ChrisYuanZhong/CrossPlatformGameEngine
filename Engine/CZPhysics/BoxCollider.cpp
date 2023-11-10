#include "BoxCollider.h"
#include "SphereCollider.h"

#include <cfloat>
#include <algorithm>

ChrisZ::Physics::BoxCollider::BoxCollider(eae6320::Math::sVector i_center, eae6320::Math::sVector i_extents, eae6320::Math::cQuaternion i_orientation, eae6320::Assets::GameObject* i_gameObject)
{
	this->center = i_center;
	this->extents = i_extents;
	this->orientation = i_orientation;
	this->gameObject = i_gameObject;
}

bool ChrisZ::Physics::BoxCollider::Intersects(Collider* other)
{
    // If the other collider is a sphere collider, use the sphere-box intersection formula
    if (SphereCollider* otherSphere = dynamic_cast<SphereCollider*>(other)) {
        // Get the closest point on the box to the sphere center
        eae6320::Math::sVector closestPoint = this->ClosestPoint(otherSphere->GetCenter());

        // Get the distance between the closest point and the sphere center
        float distance = (otherSphere->GetCenter() - closestPoint).GetLength();

        // If the distance is less than or equal to the radius, the sphere and the box intersect
        return distance <= otherSphere->GetRadius();
    }

    // If the other collider is a box collider, use the box-box intersection formula
    if (BoxCollider* otherBox = dynamic_cast<BoxCollider*>(other)) {
        // Get the axes of the boxes
        eae6320::Math::sVector axes[15];
        axes[0] = this->orientation * eae6320::Math::sVector(1, 0, 0); // x-axis of this box
        axes[1] = this->orientation * eae6320::Math::sVector(0, 1, 0); // y-axis of this box
        axes[2] = this->orientation * eae6320::Math::sVector(0, 0, 1); // z-axis of this box
        axes[3] = otherBox->orientation * eae6320::Math::sVector(1, 0, 0); // x-axis of the other box
        axes[4] = otherBox->orientation * eae6320::Math::sVector(0, 1, 0); // y-axis of the other box
        axes[5] = otherBox->orientation * eae6320::Math::sVector(0, 0, 1); // z-axis of the other box

        // Get the cross products of the axes
        for (int i = 0; i < 3; i++) {
            for (int j = 3; j < 6; j++) {
                axes[6 + i * 3 + j - 3] = eae6320::Math::Cross(axes[i], axes[j]);
            }
        }

        // For each axis, check if the projections of the boxes overlap
        for (int i = 0; i < 15; i++) {
            // If the axis is zero, skip it
            if (axes[i].GetLength() == 0) {
				continue;
			}

            // Get the min and max projections of this box on the axis
            float min1 = FLT_MAX;
            float max1 = -FLT_MAX;
            for (int j = 0; j < 8; j++) {
                // Get the vertex of this box
                eae6320::Math::sVector vertex = this->center + this->orientation * eae6320::Math::sVector(
                    (j & 1) ? this->extents.x / 2 : -this->extents.x / 2,
                    (j & 2) ? this->extents.y / 2 : -this->extents.y / 2,
                    (j & 4) ? this->extents.z / 2 : -this->extents.z / 2
                );

                // Project the vertex on the axis
                float projection = eae6320::Math::Dot(vertex, axes[i]);

                // Update the min and max projections
                min1 = std::min(min1, projection);
                max1 = std::max(max1, projection);
            }

            // Get the min and max projections of the other box on the axis
            float min2 = FLT_MAX;
            float max2 = -FLT_MAX;
            for (int j = 0; j < 8; j++) {
                // Get the vertex of the other box
                eae6320::Math::sVector vertex = otherBox->center + otherBox->orientation * eae6320::Math::sVector(
                    (j & 1) ? otherBox->extents.x / 2 : -otherBox->extents.x / 2,
                    (j & 2) ? otherBox->extents.y / 2 : -otherBox->extents.y / 2,
                    (j & 4) ? otherBox->extents.z / 2 : -otherBox->extents.z / 2
                );

                // Project the vertex on the axis
                float projection = eae6320::Math::Dot(vertex, axes[i]);

                // Update the min and max projections
                min2 = std::min(min2, projection);
                max2 = std::max(max2, projection);
            }

            // If the projections do not overlap, the boxes do not intersect
            if (max1 < min2 || max2 < min1) {
                return false;
            }
        }

        // Otherwise, the boxes intersect
        return true;
    }

    // Otherwise, return false
    return false;
}

eae6320::Math::sVector ChrisZ::Physics::BoxCollider::ClosestPoint(eae6320::Math::sVector point)
{
    // Transform the point to the local space of the box
    eae6320::Math::sVector localPoint = this->orientation.GetInverse() * (point - this->center);

    // Clamp the point to the extents of the box
    eae6320::Math::sVector localClosestPoint;
    localClosestPoint.x = std::clamp(localPoint.x, -this->extents.x / 2, this->extents.x / 2);
    localClosestPoint.y = std::clamp(localPoint.y, -this->extents.y / 2, this->extents.y / 2);
    localClosestPoint.z = std::clamp(localPoint.z, -this->extents.z / 2, this->extents.z / 2);

    // Transform the point back to the world space
    eae6320::Math::sVector worldClosestPoint = this->center + this->orientation * localClosestPoint;

    // Return the closest point
    return worldClosestPoint;
}
