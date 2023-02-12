#include "AvtivityCodeGen.h"

int main()
{
	AvtivityCodeGen cg;
	cg.FC2Pseudocode("examples\\input1.txt", "examples\\output1.txt");
	system("pause");
	return 0;
}