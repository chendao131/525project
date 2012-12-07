#include"SManager.h"


void Tablestorage:: tablestorage_init(Catalog* cata)
{
       this->catalog=cata;
}

void Tablestorage:: tableopen(char* tablename_file)
{ 
		  char* filename=tablename_file;
          char pathandf[30]="data/";
          strcat(pathandf,filename);
	      this->tfp=fopen(pathandf,"r+");

}

void Tablestorage:: tableclose()
{
    fclose(this->tfp);
}

Block* Tablestorage:: tab_get_blo(char* tablename, int blo_no)
{
	int j;
	//cout<<"blo_number in storage is "<<blo_no<<endl;
	Block* toblock=(Block*)malloc(sizeof(Block));

    this->tableopen(tablename);

	fseek(this->tfp,blo_no*12*(sizeof(int)),0);
    for(j=0;j<12;j++)
	{  
	   fread(&(toblock->bl_data[j]),4,1,this->tfp);
	}

    strcpy(toblock->tablename,tablename);

	toblock->blockno_in_ta=blo_no;
	this->tableclose();

	return toblock;
}

void Tablestorage:: tab_put_blo(char* tablename, int blo_no,Block* bb)
{

	int te=this->catalog->cata_get_table(tablename);

	if(te!=-1)
	{

	  this->tableopen(tablename);
	  for(int k=0;k<12;k++)
	  {
	     blo_from_filetab.bl_data[k]=bb->bl_data[k];
    	}

	  fseek(this->tfp,blo_no*12*(sizeof(int)),SEEK_SET);

      for(int j=0;j<12;j++)
	  {

	     fwrite(&(blo_from_filetab.bl_data[j]),sizeof(int),1,this->tfp);

    	}
	this->tableclose();

    }
	else return;
}
    
        

    



    
