#ifndef BMANAGER_H
#define BMANAGER_H
#include"Catalog.h"
#include"SManager.h"
#include <stdlib.h>
#include <vector>
#include <list>
#include<string.h>
#include<stdio.h>
#include<iostream>
using namespace std;

class Buffer
{
	public:
	list <Block> scanb;  
    int blonum;
	int hit;
	int hit_right;

    void buf_init(int idnum, Catalog* catalog, Tablestorage* ts);
    Block* buf_get_blo(char tablename[], int blo_no);
    void buf_put_blo(Block* bll);
	void buf_commit_blo();
    void buf_print();
    void buf_printright();

	private:
		Catalog* catalog;
	    Tablestorage tstor;
};

#endif
