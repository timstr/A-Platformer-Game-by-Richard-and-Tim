#include "TestUI.h"
#include "PhysicsTestUI.h"

#include "SpriteSheetStore.h"

int main() {

	ui::init(1000, 700, "Elf Rat Prom");

	// uncomment for working map and
	//auto tsw = ui::root().add<TestSpaceWindow>();
	//tsw->grabFocus();

	// uncomment for physics test:
	auto ptui = ui::root().add<PhysicsTestUI>(vec2 { 1000, 700 });
	ptui->grabFocus();

	ui::run();

	ImageStore::clear();

	return 0;
}