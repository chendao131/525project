#ifndef SMAMAGER_H
#define SMAMAGER_H
#include"Block.h"
#include"Catalog.h"
#include"parser.h"
#include <stdlib.h>
#include <vector>
#include <list>
#include<string.h>
#include<stdio.h>
#include<iostream>
#include<stdio.h>
using namespace std;


class Tablestorage
{
public:
    void tablestorage_init(Catalog* cata);
	void tableopen(char* tablename_file);       //Open the required file of the table
	void tableclose();      //close the current file of the table
	Block* tab_get_blo(char* tablename, int blo_no);
    void tab_put_blo(char* tablename, int blo_no,Block* bb);
       FILE* tfp;
private:
	Block blo_from_filetab;
    Catalog* catalog;

};
#endif
