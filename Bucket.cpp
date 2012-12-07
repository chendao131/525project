#include"Bucket.h"

Bucket Bucket:: bu_insert(int bukey, int buptr)
{
	int y;

	Bucket b;
     for (y=0; y<9; y++)
     {
		 if(this->bptr[y]==-1)
	     {

			 this->bptr[y]=buptr;

			 return b;
		 }
		 if (y == 8)
		 {
			 b.bkey=bukey;
			 b.bptr[0]=buptr;
		 }
     }

	return b;


}
