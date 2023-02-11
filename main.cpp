#include "AvtivityCodeGen.h"

int main()
{
	AvtivityCodeGen cg;
	cg.FC2Pseudocode("examples\\input.txt", "examples\\output.txt");
	return 0;
}