#pragma once
#include "creature.h"

namespace Event {
	bool operator < (const Event& l, const Event&r){
		return l.id < r.id;
	}
}