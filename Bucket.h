
#include <stdlib.h>
#include <vector>
#include <list>
#include<string.h>
#include<stdio.h>
#include<iostream>
using namespace std;

class Bucket
{
    public:
	Bucket(){
           this->type=3;
           this->bkey=-1;


	   for(int i=0; i<9; i++)
	   {
               this->bptr[i]=-1;
           }
	   this->nextb=-1;
       }
       ~Bucket(){}
       Bucket bu_insert(int bukey, int buptr); 

       int type;
       int bkey;
       int bptr[9];
       int nextb;

    
};
