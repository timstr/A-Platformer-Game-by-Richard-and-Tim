#include "TestUI.h"
#include "PhysicsTestUI.h"

#include "SpriteSheetStore.h"

void expectEqual(const vec2& a, const vec2& b){
	if (abs(a.x - b.x) > 1e-6f || abs(a.y - b.y) > 1e-6f){
		throw std::runtime_error("Vectors are not equal.");
	}
}

int main() {

	expectEqual(*lineSegmentIntersection({-1, 0}, {1, 0}, {0, -1}, {0, 1}), {0, 0});
	expectEqual(*lineSegmentIntersection({9, 10}, {11, 10}, {10, 9}, {10, 11}), {10, 10});
	expectEqual(*lineSegmentIntersection({-1, 5}, {1, 5}, {0, 4}, {0, 6}), {0, 5});
	if (lineSegmentIntersection({0, 0}, {1, 0}, {0, 1}, {1, 1})){
		throw std::runtime_error("Wrong.");
	}
	if (lineSegmentIntersection({0, 0}, {1, 0}, {2, 0}, {3, 0})){
		throw std::runtime_error("Wrong.");
	}
	if (!lineSegmentIntersection({0, 0}, {1, 0}, {0, 0}, {1, 0})){
		throw std::runtime_error("Wrong.");
	}
	expectEqual(*lineSegmentIntersection({0, 0}, {0, 1}, {0, 0}, {1, 0}), {0, 0});
	if (lineSegmentIntersection({413.194458f,201.850983f}, {367.000488f,220.985168f}, {527.500000f,250.000000f}, {527.500000f,450.000000f})){
		throw std::runtime_error("Wrong.");
	}
	if (lineSegmentIntersection({527.999512f,479.014832f}, {413.194458f,201.850983f}, {527.500000f,250.000000f}, {527.500000f,450.000000f})){
		throw std::runtime_error("Wrong.");
	}

	ui::init(1000, 700, "Elf Rat Prom");

	// uncomment for working map and
	// auto tsw = ui::root().add<TestSpaceWindow>();
	// tsw->grabFocus();

	// uncomment for physics test:
	auto ptui = ui::root().add<PhysicsTestUI>(vec2 { 1000, 700 });
	ptui->grabFocus();

	ui::run();

	ImageStore::clear();

	return 0;
}