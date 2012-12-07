#ifndef BLOCK_H
#define BLOCK_H

#include"Catalog.h"
#include"parser.h"
#include <stdlib.h>
#include <vector>
#include <list>
#include<string.h>
#include<stdio.h>
#include<iostream>
using namespace std;






class Block
{
public:
	char tablename[5];
    int blockno_in_ta;
	int blockno_in_bu;
	int bl_data[12];                                                     //48 bytes for storing data
	int blo_remain;
	int blo_dirty;                                                       //whether this block is changed, 0 means clean while 1 means dirty

    void blo_init(char* table_name,int block_intab);
    void insertdata(int idl, num_list_t* i);
};



#endif