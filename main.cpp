#include "AvtivityCodeGen.h"

int main()
{
	AvtivityCodeGen cg;
	cg.FC2Pseudocode("examples\\input2.txt", "examples\\output2.txt");
	system("pause");
	return 0;
}