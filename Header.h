
#include <stdlib.h>
#include <vector>
#include <list>
#include<string.h>
#include<stdio.h>
#include<iostream>
using namespace std;

class Header
{
    public:
        
	  char indh_name[5];       // the name of the index
	  char indh_tablename[5];          //this index is used for which table
          char indh_attributename[8];      //on which attribute
	  int indh_tableno;        // The No. of table in catalog
	  int indh_attributeno;    // The No. of attribute in the table
	  int indh_check_dup;      //if check_dup equals to 1 or -1. -1 means no duplicates, while 1 means it is allowed
	  int indh_root_loc;       // Point to the location of root node, which is also the last block
          int indh_nodetotal;
          char left[10];          

          void header_disk2mom(char* indexname);
	  void header_mom2disk(char* indexname);
	  void header_moveroot(char* indexname,int newloc);
	  void header_ini(char* indexname);

};
