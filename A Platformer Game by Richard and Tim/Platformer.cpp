#include "TestUI.h"

int main(){
	
	ui::init({1000, 700}, "Elf Rat Prom");
	
	TestSpaceWindow* tsw = new TestSpaceWindow();
	ui::root()->addChildWindow(tsw);
	tsw->grabFocus();

	ui::run();

	return 0;
}