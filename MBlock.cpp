#include "MBlock.h"

Node Mixblock:: mx_Mixblock2Node()
{
    Node newnode;
	newnode.type=this->mx_data[0];
    for (int i=0; i<4; i++)
    {
			newnode.key[i]=this->mx_data[i+1];
    }

    for (int j=0; j<5; j++)
    {
			newnode.ptr[j]=this->mx_data[j+5];
    }
    newnode.rekey=this->mx_data[10];
	newnode.reptr=this->mx_data[11];

    return newnode;
}

Bucket Mixblock:: mx_Mixblock2Bucket()
{

    Bucket bc;
    bc.type=this->mx_data[0];
	bc.bkey=this->mx_data[1];
	for (int t=0; t<9; t++)
	{
		bc.bptr[t]=this->mx_data[t+2];
	}
	bc.nextb=this->mx_data[11];
    return bc;
}
void Mixblock:: mx_Node2Mixblock(Node nd)
{
	this->mx_data[0]=nd.type;

    for (int i=0; i<4; i++)
    {
			this->mx_data[i+1]=nd.key[i];
    }

    for (int j=0; j<5; j++)
    {
			this->mx_data[j+5]=nd.ptr[j];
    }
    this->mx_data[10]=nd.rekey;
	this->mx_data[11]=nd.reptr;
}
void Mixblock:: mx_Bucket2Mixblock(Bucket bc)
{

    this->mx_data[0]=bc.type;
	this->mx_data[1]=bc.bkey;
	for (int t=0; t<9; t++)
	{
		this->mx_data[t+2]=bc.bptr[t];
	}
	this->mx_data[11]=bc.nextb;

}
