#include "SphereCollider.h"
#include "BoxCollider.h"

#include <Engine/Assets/GameObject.h>

ChrisZ::Physics::SphereCollider::SphereCollider(eae6320::Math::sVector i_center, float i_radius, eae6320::Assets::GameObject* i_gameObject)
{
    this->center = i_center;
	this->radius = i_radius;
	this->gameObject = i_gameObject;
}

bool ChrisZ::Physics::SphereCollider::Intersects(Collider* other)
{
    // If the other collider is a sphere collider, use the sphere-sphere intersection formula
    if (SphereCollider* otherSphere = dynamic_cast<SphereCollider*>(other)) {
        // Get the distance between the centers of the spheres
        float distance = (this->center - otherSphere->center).GetLength();

        // If the distance is less than or equal to the sum of the radius, the spheres intersect
        return distance <= (this->radius + otherSphere->radius);
    }

    // If the other collider is a box collider, use the sphere-box intersection formula
    if (BoxCollider* otherBox = dynamic_cast<BoxCollider*>(other)) {
        // Get the closest point on the box to the sphere center
        eae6320::Math::sVector closestPoint = otherBox->ClosestPoint(this->center);

        // Get the distance between the closest point and the sphere center
        float distance = (this->center - closestPoint).GetLength();

        // If the distance is less than or equal to the radius, the sphere and the box intersect
        return distance <= this->radius;
    }

    // Otherwise, return false
    return false;
}

float ChrisZ::Physics::SphereCollider::GetRadius() const
{
	return this->radius;
}

void ChrisZ::Physics::SphereCollider::SetRadius(float i_radius)
{
	this->radius = i_radius;
}