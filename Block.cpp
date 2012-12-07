#include"Block.h"
void Block:: blo_init(char* table_name,int block_intab)
{
    strcpy(this->tablename, table_name);
    this->blockno_in_ta=block_intab;
	this->blockno_in_bu=-1;
	for (int i=0; i<12;i++)
	{         bl_data[i]=0;
	}                                                     
	this->blo_remain=12;
    this->blo_dirty=0;
}
void Block:: insertdata(int idl,num_list_t* i)
{ 
	int j=0;
	//cout<<"pre this->blo_remain:  "<<this->blo_remain<<endl;
	//cout<<"idl:  "<<idl<<endl;

    if(this->blo_remain>=idl)  
		j=12-this->blo_remain;

		do
		{ 
		     this->bl_data[j]=i->num;

			 j++;
			 i=i->next;
		}
		while (i);

		this->blo_remain-=idl;
        
	    
		//cout<<"after this->blo_remain:  "<<this->blo_remain<<endl;
		this->blo_dirty=-1;
		
}	
	
