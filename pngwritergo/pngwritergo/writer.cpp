
#include <stdlib.h>
#include <pngwriter.h>
#include <iostream>
#include <fstream>
using namespace std;

void printBarcode(int argc, char * argv[])
{
	


	double gradstart = 0.0;
	double gradend = 0.3;

	//Creating the PNGwriter instance
	pngwriter image(400, 500, 1.0, "out.png");



	char code[] = "*BANANA*";
	char barcode[] = "barcode.ttf";
	char arial[] = "arial.ttf";
	//This writes the kanji in the lower left corner.
	image.plot_text(barcode, 80, 10, 350, 0.0, code, 0.0, 0.0, 0.0);
	char codetext[] = "CODE:";
	image.plot_text(arial, 40, 50, 460, 0.0, codetext, 0.0, 0.0, 0.0);


	char bin[] = "*BANANA*";
	//This writes the kanji in the lower left corner.
	image.plot_text(barcode, 80, 10, 180, 0.0, bin, 0.0, 0.0, 0.0);
	char bintext[] = "BIN:";
	image.plot_text(arial, 40, 50, 290, 0.0, bintext, 0.0, 0.0, 0.0);


	char rev[] = "*BANANA*";

	//This writes the kanji in the lower left corner.
	image.plot_text(barcode, 80, 10, 10, 0.0, rev, 0.0, 0.0, 0.0);
	char revtext[] = "REV:";
	image.plot_text(arial, 40, 50, 120, 0.0, revtext, 0.0, 0.0, 0.0);





	image.close();
	system("out.png");
}