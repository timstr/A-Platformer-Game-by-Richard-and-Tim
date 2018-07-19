#include "TestUI.h"
#include "PhysicsTestUI.h"

#include "SpriteSheetStore.h"

int main() {

	ui::init(1000, 700, "Elf Rat Prom");

	/*TestSpaceWindow* tsw = new TestSpaceWindow();
	ui::root()->addChildWindow(tsw);
	tsw->grabFocus();*/

	auto ptui = ui::root().add<PhysicsTestUI>(vec2 { 1000, 700 });
	ptui->grabFocus();

	ui::run();

	ImageStore::clear();

	return 0;
}