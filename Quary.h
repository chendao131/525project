#ifndef QUARY_H
#define QUARY_H
#include"parser.h"
#include"Catalog.h"
#include"BManager.h"
#include <stdlib.h>
#include <vector>
#include <list>
#include<string.h>
#include<stdio.h>
#include<iostream>
#include"Index.h"
using namespace std;


class Quary{
	public:
		int check_sum(insert_statement_t* i, Catalog* cc);
		void do_insert_statement(insert_statement_t *i, Catalog* cc, Buffer* b,Index * index);
		void do_select_statement(select_statement_t* i,Catalog* cc,Buffer* b,Index* index);
		int calculate_conds(select_statement_t* i);
		void print_table( char* table, Catalog* cc, Buffer* b);
		void print_index(char* indexname, char* table, Index* index);
                void do_set_debug(set_statement_t* i, Index* index);
                vector<int>  do_reduce_dup(vector<int> invec);
               // void do_create_index_statement(create_index_statement_t *i,Index* index);
		vector<int> do_equal_com(vector<int> vec1, vector<int> vec2); 
};


#endif

/* "="                  {yylval.operator_v = OP_EQUAL; return OPERATOR;}
"<"                  {yylval.operator_v = OP_LESS; return OPERATOR;}
"<="                 {yylval.operator_v = OP_LESS_EQUAL; return OPERATOR;}
">"                  {yylval.operator_v = OP_GREATER; return OPERATOR;}
">="                 {yylval.operator_v = OP_GREATER_EQUAL; return OPERATOR;}
"!="                 {yylval.operator_v = OP_NOT_EQUAL; return OPERATOR;}
*/
