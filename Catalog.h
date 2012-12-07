#ifndef CATALOG_H
#define CATALOG_H
#include"parser.h"
#include <stdlib.h>
#include <vector>
#include <list>
#include<string.h>
#include<stdio.h>
#include<iostream>
#include<sys/stat.h>
using namespace std;
#define INDEXTOTAL 3

struct tableinfo
{
public:
   char cata_table_name[5];
   char id_content[20][8];
   int num_line;                                        //Record the num of lines
   int id_len;                                        //Record the length of id
   int num_block;
   int remain_in_block;
   int ta_status;                                     //status
   int at_index[INDEXTOTAL];                                   //which attribute has got an index
   char at_index_name[INDEXTOTAL][5];
};

class Catalog
{
public:
    struct tableinfo ta[50];                                                   // vector<tableinfo> ta;

	int ta_cur;                                       //mark the next available empty table

    void cata_makedir(char* dirname);
    void cata_disk2mom();
	void cata_mom2disk();
	void cata_createtable(create_table_statement_t* ct);
	void cata_droptable(char* drop_tablename);
    int cata_get_table(char* tablename);
	int cata_get_attribute(char* tablename,char* attrname);
	int cata_setindex(int tableno, int attrno);
	void cata_commit();
	void cata_print();

};





#endif
