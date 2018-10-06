#include "HermannHorst.h"

CreatureTypeT<HermannHorst> HermannHorstType;

HermannHorst::HermannHorst() : Creature("HermannHorst") {

	setType(HermannHorstType);

	addCircle(Circle({0, -250}, 220));

	friction = 0.3f;
	elasticity = 0.1f;

	setScale(0.2f, 0.2f);

	addStateTransition(Standing, Standing, AnimationEnd, 10);
	addStateTransition(Standing, Walking, AnimationEnd, 1);
	addStateTransition(Standing, Standing, AnimationEnd, 1, [this](){
		this->flip();
	});
	addStateTransition(Walking, Walking, AnimationEnd, 8);
	addStateTransition(Walking, Walking, AnimationEnd, 1, [this](){
		this->flip();
	});
	addStateTransition(Walking, Standing, AnimationEnd, 2);
	addStateTransition(Walking, Running, AnimationEnd, 1);
	addStateTransition(Running, Running, AnimationEnd, 7);
	addStateTransition(Running, Leaping, AnimationEnd, 1, [this](){
		this->velocity += vec2(0, -20);
	});
	addStateTransition(Running, Walking, AnimationEnd, 2);
	addStateTransition(Leaping, Running, ::Standing, 1);
	addStateTransition(Leaping, Walking, ::Standing, 1);
	addStateTransition(Leaping, Standing, ::Standing, 1);

	setStateAnimation(Standing, "standing");
	setStateAnimation(Running, "running");
	setStateAnimation(Walking, "walking");
	setStateAnimation(Leaping, "leaping");

	setState(Standing);
}

vec2 HermannHorst::getContactAcceleration(const Obstruction& obstruction, vec2 normal) const {
	switch (getState()){
	case Walking:
		return vec2(2 * getDirection(), 0);
	case Running:
		return vec2(8 * getDirection(), 0);
	default:
		return vec2(0, 0);
	}
}