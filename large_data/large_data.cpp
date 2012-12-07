#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <sys/stat.h>
#include <cstdlib>

using namespace std;

#define ROW1 1000
#define ROW2 100
#define COL 4

int main(int argc, char* argv[] )
{
	int b;
	char FileName[50]="a00";
	ofstream fp(FileName);
	for(int i=0; i<ROW1; i++)
	{
		for(int j=0; j<ROW2; j++)
		{
			fp<<"insert into a00 values(";
			for (int k=0; k<COL; k++)
			{
				b=rand()%1000+1;
					fp<<b;
				if(k!=3) fp<<",";				
			}
			fp<<");\n";
		}
	}
//	fclose(fp);
}
