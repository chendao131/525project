
#include <stdlib.h>
#include <vector>
#include <list>
#include<string.h>
#include<stdio.h>
#include<iostream>
using namespace std;

class Node
{
    public:
	Node(){
		this->type=-1;              //1 means leaf, 2 means internal

	    
		for(int i=0;i<4;i++)
	    	{
			this->key[i]=-1;
		}
		
        	for(int j=0;j<5;j++)
	    	{
			this->ptr[j]=-1;
		}
		//rekey = -1
		this->rekey = -1;
		this->reptr = -1;
	}
	~Node(){}
	Node node_insert(int nkey, int nptr);

        int type;
	int key[4];
	int ptr[5];
	int rekey;
	int reptr;
        
        
};
