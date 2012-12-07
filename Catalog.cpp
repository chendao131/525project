#include"Catalog.h"

void Catalog:: cata_makedir(char* dirname)
{
      int y;
	//if(!PathFileExists( dirname)) 
      //y=mkdir("/data",S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
       mkdir("data",0777);
}

void Catalog:: cata_disk2mom()
{
   int i=0;
   int j=0;
   FILE *fp;
   char* filename="cat.dat";

   char pathandf[30]="data/";
   strcat(pathandf,filename);
   
   fp=fopen(pathandf,"r");

   struct tableinfo ta_temp;

   if(fp==NULL)
	{   fp=fopen(pathandf,"w+");       //if this file does not exist, then create this new file
        this->ta_cur=0;
	}

   else
	{
	   while(fread(&ta_temp,sizeof(tableinfo),1,fp)>0)
		{
            strcpy(this->ta[i].cata_table_name,ta_temp.cata_table_name);
			   for(j=0;j<20;j++)
			   {
                   strcpy(this->ta[i].id_content[j],ta_temp.id_content[j]);                            
			   }
             this->ta[i].id_len=ta_temp.id_len;
	         this->ta[i].num_line=ta_temp.num_line;                                //num_sta(tab->tablenum,id_len);                               
             this->ta[i].num_block=ta_temp.num_block;
			 this->ta[i].remain_in_block=ta_temp.remain_in_block; 
			 this->ta[i].ta_status=0;
			 for (j=0; j<INDEXTOTAL; j++)
			 {
				 this->ta[i].at_index[j]=ta_temp.at_index[j];
				 strcpy(this->ta[i].at_index_name[j],ta_temp.at_index_name[j]);
			 }
			 i++;
		}
		this->ta_cur=i;
	}
	fclose(fp);
}

void Catalog:: cata_mom2disk()
{
    int i=0;
	int j;
    FILE *fp;

    char* filename="cat.dat";
    char pathandf[30]="data/";
    strcat(pathandf,filename);
    fp=fopen(pathandf,"r+");
    struct tableinfo ta_temp;
    if(fp==NULL) fp=fopen(pathandf,"w+");
    while(i<this->ta_cur)
	{
     /*   if(this->ta[i].ta_status!=0)
		{      */
			strcpy(ta_temp.cata_table_name,this->ta[i].cata_table_name);
			   for(j=0;j<20;j++)
			   {
                              strcpy(ta_temp.id_content[j],this->ta[i].id_content[j]);                            
			   }
                           ta_temp.id_len=this->ta[i].id_len;
	                   ta_temp.num_line=this->ta[i].num_line;     
                            ta_temp.num_block=this->ta[i].num_block;
			   ta_temp.remain_in_block=this->ta[i].remain_in_block; 
			   this->ta[i].ta_status=0;
			   for (j=0; j<INDEXTOTAL; j++)
			   {
				 ta_temp.at_index[j]=this->ta[i].at_index[j];
				 strcpy(ta_temp.at_index_name[j],this->ta[i].at_index_name[j]);
			   }

			 fwrite(&ta_temp,sizeof(tableinfo),1,fp);
		/* }
		 else
			fseek(fp,i*sizeof(tableinfo),SEEK_SET);        */
			 i++;
	}
	fclose(fp);
}

void Catalog:: cata_createtable(create_table_statement_t* ct)
{
     //    cout<<"_________createtable______n000000000____:"<<this->ta[0].cata_table_name<<endl;
      //   cout<<"_________createtable______n111111111____:"<<this->ta[1].cata_table_name<<endl;

//check whether this table has established.
    int judgetable=0;
	int judgecol=0;
    int column_num=0;
	int j=0;

    char temp[8];
     for(j=0;j<this->ta_cur;j++)
     {		
        strcpy(temp,this->ta[j].cata_table_name);

        if(strcmp(ct->table,temp)==0)                     //this table name exists, then stop the program
         {
			   judgetable=-1;
               break;
		 }

     }

//check the column names
    if(judgetable==0)
	{
         id_list_t* k=ct->columns;
		 int cbit=0;
         while(k)
         {

             while(k->id[cbit]!='\0')
              {
				   
                if(k->id[cbit]==' ')
                {
                cout<<"id name can not include ' ' "<<endl;
                judgecol=-1;
                break;  
                }
                cbit++;
              }
		   if(judgecol==-1) break;
           k=k->next;
		   column_num++;

		 }
	}

	if(judgetable==0&&judgecol==0)
	{
          int new_tab_no=this->ta_cur;
     	  ++this->ta_cur;                                 //It means the num of table +1

          //establish a new file of this new table in disk
		  FILE *fp;
		  char* filename=ct->table;
          char pathandf[30]="data/";
          strcat(pathandf,filename);
          fp=fopen(pathandf,"w+");
          int cp=0;

		  //put the ids to catalog's table_information 
		    strcpy(this->ta[new_tab_no].cata_table_name,ct->table);//input the new table name

			while(ct->columns)
		    {
	    	     strcpy(this->ta[new_tab_no].id_content[cp],ct->columns->id); //input the names of the columns

				 ct->columns=ct->columns->next;
				 cp++;
		    }
	    	this->ta[new_tab_no].id_len=column_num;                                // the num of column in this new table		
	        this->ta[new_tab_no].num_line=0;                                       //num_sta(tab->tablenum,id_len);                                 
                this->ta[new_tab_no].num_block=0;
	    	this->ta[new_tab_no].remain_in_block=0; 
	    	this->ta[new_tab_no].ta_status=1;                                      //mark whether this table is "dead"
			for (int si=0; si<INDEXTOTAL; si++)
			 {
				 this->ta[new_tab_no].at_index[si]=-1;
			 }
	      fclose(fp);
    //cout<<"createtablename[0]: _______"<<this->ta[0].cata_table_name<<endl;
	}
         //cout<<"_________createtable______n000000000____:"<<this->ta[0].cata_table_name<<endl;
}

void Catalog:: cata_droptable(char* drop_tablename)
{

	int i;
	char* filename=drop_tablename;
	char pathandf[30]="data/";
        strcat(pathandf,filename);
        char pat[30]="data/";

    for (i=0;i<this->ta_cur;i++)
    {
		if (strcmp(drop_tablename,this->ta[i].cata_table_name)==0)
		{

               memset(this->ta[i].cata_table_name,0,5);
			   memset(this->ta[i].id_content,0,160);                 // fill up 0 into id_content;

             this->ta[i].id_len=0;
	         this->ta[i].num_line=0;                                       //num_sta(tab->tablenum,id_len);                                 
             this->ta[i].num_block=0;
			 this->ta[i].remain_in_block=0; 
			 this->ta[i].ta_status=-1;
			 for (int si=0; si<INDEXTOTAL; si++)
			 {
                              if(this->ta[i].at_index[si]!=-1)
                              {        // cout<<"this->ta[i].at_index_name[si]:___________"<<this->ta[i].at_index_name[si]<<endl;
                                 strcat(pat,this->ta[i].at_index_name[si]);
                                 remove(pat);
                                 memset(pat,0,30);
                                 strcpy(pat,"data/");
                              }
				 this->ta[i].at_index[si]=-1;
     		                 memset(this->ta[i].at_index_name[si],0,5);
			 }

			 remove(pathandf);
			 cout<<"The table:"<<drop_tablename<<"has been deleted."<<endl;
	    	break; 
		}
     }
	 
	 if(i>this->ta_cur)
		 printf("This table does not exist!\n");
	 return;
}

int Catalog:: cata_get_table(char* tablename)
{     //    cout<<"_________createtable______n111111111____:"<<this->ta[0].cata_table_name<<endl;
    int i;
	int texist;

    for (i=0;i<this->ta_cur;i++)
	{
        if (strcmp(tablename,this->ta[i].cata_table_name)==0)
		break;
	}
	if(i<this->ta_cur)   texist=i;                                          //texist does not equal to -1 means this table does exist
	else                 texist=-1;                                          //equals to -1 means this table does not exist
	return texist;
}

int Catalog:: cata_get_attribute(char* tablename,char* attrname)
{
	int i;
	int h;
	int aexist;
    //cout<<"table0,attr0:  "<<this->ta[0].id_content[0]<<endl;
	//cout<<"table0,attr1:  "<<this->ta[0].id_content[1]<<endl;
	for (i=0;i<this->ta_cur;i++)
	{
        if (strcmp(tablename,this->ta[i].cata_table_name)==0)
		break;
	}
    //cout<<"attrname:  "<<attrname<<endl;
	for (h=0;h<this->ta[i].id_len;h++)
	{
		//cout<<"cata_get_attr:"<<h<<this->ta[i].id_content[h]<<endl;
		if(strcmp(attrname,this->ta[i].id_content[h])==0)  
		{
			aexist=h;
			break; 
		}
    }

    if(h==ta[i].id_len) aexist=-1;
	return aexist;
}

int Catalog:: cata_setindex(int tableno,int attrno)
{
    for (int si=0; si<INDEXTOTAL; si++)
	{
		 if (this->ta[tableno].at_index[si] == -1)
		 {
		     this->ta[tableno].at_index[si]=attrno;
			 return si;
		 }
                 if (this->ta[tableno].at_index[si]==attrno)
                 { //This column has got an index, no more is allowed
                     return -2;
                 }
		 else if (si == INDEXTOTAL-1)
		 {
             return -3;
		 } 
	}
}
void Catalog:: cata_commit()
{
	this->cata_mom2disk();
}

void Catalog:: cata_print()
{
	cout<<"print catalog;"<<endl;
	for(int i=0;i<this->ta_cur;i++)
	{

 //      cout<<"Table\t\t"<<"columns\t\t\t"<<endl;
	cout<<"table name:"<<this->ta[i].cata_table_name<<"\t*  "<<"column number:"<<ta[i].id_len<<endl;
	cout<<"column names:"<<endl;
           for(int c=0;c<this->ta[i].id_len;c++)
		{

		//	 cout<<this->ta[i].cata_table_name<<"\t\t";
			 cout<<ta[i].id_content[c]<<"  ";
	    }
	cout<<endl<<"-----------------------\n\n";
	}
}


