#pragma once

// TODO: this probably is not needed
struct Destructible {
	Destructible() : destroyed(false) {

	}
	virtual ~Destructible(){

	}

	void destroy(){
		destroyed = true;
	}

	bool isDestroyed() const {
		return destroyed;
	}

	private:
	bool destroyed;
};