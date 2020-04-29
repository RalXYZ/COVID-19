#include "graphics.h"
#include "extgraph.h"
#include "strlib.h"
#include "genlib.h"
#include <Windows.h>
#include <stdio.h>

#include "imgui.h"



void Main() {
	SetWindowSize(4, 2);
	SetWindowTitle("一个程序");
	InitGraphics();

	ExitGraphics();


}
