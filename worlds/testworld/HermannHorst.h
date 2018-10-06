#pragma once

#include "creature.h"
#include "spritesheet.h"

struct HermannHorst;

extern CreatureTypeT<HermannHorst> HermannHorstType;

struct HermannHorst : Creature {
	HermannHorst();

	vec2 getContactAcceleration(const Obstruction& obstruction, vec2 normal) const override;

	enum States {
		Standing,
		Running,
		Walking,
		Leaping,
	};
};