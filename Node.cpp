#include "Node.h"

Node Node::node_insert(int nkey, int nptr)
{
	Node newnode;
	int y,j;
    if (this->type == 1)
    {
        for (y=0; y<4; y++)
       {
		   if (nkey<key[y])
		   {     
			   this->rekey=this->key[3];
			   this->reptr=this->ptr[3];
			   for (j=2; j>=y; j--)
			   {

                               this->key[j+1]=this->key[j];
                               this->ptr[j+1]=this->ptr[j];
			   }
			   key[y]=nkey;
			   ptr[y]=nptr;
                       //   for(int tt=0; tt<4;tt++)

			   if (rekey!=-1)
			    {   //split these two nodes
                                    newnode.type=this->type;
				   newnode.key[0]=this->key[3];
                                   newnode.key[1]=this->rekey;
				   newnode.ptr[0]=this->ptr[3];
				   newnode.ptr[1]=this->reptr;
                                    newnode.rekey=newnode.key[0];

				   this->key[3]=-1;
				   this->ptr[3]=-1;
				   this->rekey=-1;
				   this->reptr=-1;
			   }
			   return newnode;
		   }
		   if(this->key[y]==-1)
	       {
			 this->key[y]=nkey;
			 this->ptr[y]=nptr;

			 return newnode;
	    	 }
           if (y==3)
           {
			 newnode.type=this->type;
			 newnode.key[0]=this->key[3];
			 newnode.key[1]=nkey;
			 newnode.ptr[0]=this->ptr[3];
			 newnode.ptr[1]=nptr;

			 newnode.rekey=this->key[3];
			 this->key[3]=-1;
			 this->ptr[3]=-1;

			 return newnode;
           }
       }
    }
    if (this->type == 2)
    {                  /*   cout<<"___________before_____________"<<endl;
                          for(int tt=0; tt<4;tt++)
                         cout<<"ptr["<<tt<<"]:__"<<ptr[tt]<<endl;              */
		if (nkey<this->key[0]&&this->key[3]!=-1&&this->ptr[0] == -1)
		{
			this->ptr[0]=nptr;
			return newnode;
		}
        for (y=0; y<4; y++)
        {
			if (nkey<this->key[y])
			{	
                           this->rekey=this->key[3];
			   this->reptr=this->ptr[4];
			   for (j=2; j>=y; j--)
			   {
                               this->key[j+1]=this->key[j];
                                this->ptr[j+2]=this->ptr[j+1];
			   }
			   key[y]=nkey;
			   ptr[y+1]=nptr;
			   if (rekey!=-1)
			    {   //split these two nodes
                                      newnode.type=this->type;
				   newnode.key[0]=this->key[3];
                                      newnode.key[1]=this->rekey;
				   newnode.ptr[0]=this->ptr[3];
				   newnode.ptr[1]=this->ptr[4];
				   newnode.ptr[2]=this->reptr;
                                    newnode.rekey=this->key[2];
                         				 //  cout<<"this->key[2]:"<<this->key[2]<<endl;
				   this->key[2]=-1;
                                                          // cout<<"this->key[2]:"<<this->key[2]<<endl;
				   this->key[3]=-1;
				   this->ptr[3]=-1;
				   this->ptr[4]=-1;
				   this->rekey=-1;
				   this->reptr=-1;
			   }
			   return newnode;
			}
		    if(this->key[y]==-1)
	        {                     
		    	 this->key[y]=nkey;
		    	 this->ptr[y+1]=nptr;

                /*     cout<<"___________After_____________"<<endl;
                          for(int tt=0; tt<4;tt++)
                          cout<<"ptr["<<tt<<"]:__"<<ptr[tt]<<endl;               */


			     return newnode;
	    	 }
             if (y==3)
             {                           
			     newnode.type=this->type;
			     newnode.key[0]=this->key[3];
			     newnode.key[1]=nkey;
			     newnode.ptr[0]=this->ptr[3];
				 newnode.ptr[1]=this->ptr[4];
			     newnode.ptr[2]=nptr;

			     newnode.rekey=this->key[2];
                             this->key[2]=-1;
			     this->key[3]=-1;
			     this->ptr[3]=-1; 
				 this->ptr[4]=-1;
			     return newnode;
             }
		 }
			
    }

     
}
   


