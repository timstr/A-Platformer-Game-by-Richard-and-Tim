#include "TestUI.h"
#include "PhysicsTestUI.h"

#include "SpriteSheetStore.h"

int main(){
	
	ui::init(1000, 700, "Elf Rat Prom");
	
	/*TestSpaceWindow* tsw = new TestSpaceWindow();
	ui::root()->addChildWindow(tsw);
	tsw->grabFocus();*/

	PhysicsTestUI* ptui = new PhysicsTestUI({1000, 700});
	ui::root()->addChildWindow(ptui);
	ptui->grabFocus();

	ui::run();

	ImageStore::clear();

	return 0;
}