#include <stdlib.h>
#include <vector>
#include <list>
#include<string.h>
#include<stdio.h>
#include<iostream>
#include"Header.h"
#include"Node.h"
#include"Bucket.h"
using namespace std;

class Mixblock
{
  public:
     Mixblock(){
	this->mx_count=0;          //mx_count=5 means this node is full
	this->mx_locinfile=-1;
        for (int i=0; i<12; i++)
        {
		this->mx_data[i]=-1;
        }  
    }
    ~Mixblock(){}

	Node mx_Mixblock2Node();
	Bucket mx_Mixblock2Bucket();
	void mx_Node2Mixblock(Node nd);
	void mx_Bucket2Mixblock(Bucket bc);
 
        int mx_count;
        int mx_locinfile;
        int mx_data[12];
};
