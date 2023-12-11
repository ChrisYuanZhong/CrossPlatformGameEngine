#pragma once

#include <Engine/Assets/GameObject.h>

class PlatformerGameObject : public eae6320::Assets::GameObject
{
public:
	//PlatformerGameObject() = default;
	//PlatformerGameObject(const PlatformerGameObject& i_copy) = default;
	//PlatformerGameObject(PlatformerGameObject&& i_move) = default;
	//PlatformerGameObject& operator=(const PlatformerGameObject& i_copy) = default;
	//PlatformerGameObject& operator=(PlatformerGameObject&& i_move) = default;
	//~PlatformerGameObject() = default;

	inline void SetIsStandable(bool i_isStandable);
	inline bool GetIsStandable() const;
protected:
	bool isStandable = true;
};

inline void PlatformerGameObject::SetIsStandable(bool i_isStandable)
{
	isStandable = i_isStandable;
}

inline bool PlatformerGameObject::GetIsStandable() const
{
	return isStandable;
}