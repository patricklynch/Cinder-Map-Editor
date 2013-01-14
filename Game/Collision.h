#pragma once

#include "Node.h"

namespace ly {
	
// These map to UIButton 'tag' values
typedef enum {
	StrikeLowKick,
	StrikeLowPunch,
	StrikeHighPunch,
	StrikeHighKick,
	StrikeCount
} StrikeType;

typedef enum {
	AttackZoneLegs,
	AttackZoneMiddle,
	AttackZoneBody,
	AttackZoneHead,
	AttackZoneCount
} AttackZoneType;

class Character;

class Strike {
public:
	Strike();
	StrikeType type;
	void update( const float deltaTime );
	Node* node;
	bool isActive() const { return mIsActive; }
	void activate();
	void deactivate();
	Character* character;
	void onComplete();
	float animPosition;
private:
	float strikePosition;
	float duration;
	bool mIsActive;
	float lifetime;
};

class AttackZone {
public:
	AttackZone();
	AttackZoneType type;
	void update( const float deltaTime );
	Node* node;
	void activate();
	void hit();
	Character* character;
private:
	float duration;
	bool mIsHit;
	float lifetime;
};
	
class Hit {
public:
	Hit( ci::Vec3f position, float duration = 0.5f );
	virtual ~Hit();
	Node& node() { return *mNode; }
	void update( const float deltaTime );
private:
	void animationUpdate();
	Node* mNode;
	float mAlpha;
};

}