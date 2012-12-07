#include"print.h"
#include"parser.h"
#include"Catalog.h"
#include"BManager.h"
#include"Quary.h"

#include <sys/time.h>
#include <stdlib.h>
#include <vector>
#include <list>
#include<string.h>
#include<stdio.h>
#include<iostream>
#include<sys/stat.h>
#include <time.h>
#include <fstream>
using namespace std;

//time_t now();

//consider using the GNU Readline library instead of myreadline()
//it has a similar interface, and you'll get nice features like working
//cursor keys and (if initialized properly) history.
//run "man 3 readline" for more information
char* myreadline(char* prompt);

int main(int argc, char** argv){
   char* input=NULL;
   statement_t* parsed=NULL;
   int stillrunning=1;
   Catalog catalog;
   Buffer buffer;
   Quary quary;
   Index index;
   int timing=0;
   int ki=0;

   Tablestorage tablestorage;

//For timer
   float time_use=0;
   struct timeval start;
   struct timeval end;
   int check_time=0;

//Initialize buffer and
   char dirname[10]="data";
    catalog.cata_makedir(dirname);

   catalog.cata_disk2mom();
   //int blot= atoi(argv[1])
   int blot;
   	if(argc<2)
		blot=3;
	else
		blot=atoi(argv[1]);
   buffer.buf_init(blot,&catalog,&tablestorage);
   index.ind_ini(&catalog, &buffer);
     //  mkdir("/diskpath",0777);

   /* cout<<"cata_tablename0______: "<<catalog.ta[0].cata_table_name<<endl;
    cout<<"cata_tablename1______: "<<catalog.ta[1].cata_table_name<<endl;    */
   while (stillrunning &&  (input=myreadline(">> ")) != 0){
      parsed=parse_statement(input);//parse statement;


	 /*
	  * dispatch_print is an example of how to work with the AST to detect
	  * and run the various commands, as well as how to get at all of the
	  * information in the AST. Remove this call when you implement the
	  * logic to actually run the commands in the DBMS.
	  * (You can use the functions in print.c for debugging purposes)
	  */
	 if(parsed==NULL)
	 continue;
    if(parsed)
	{
		 dispatch_print(parsed);
       	// time_t begintime=now();    
     if(parsed->select){

			  if(check_time==1) gettimeofday(&start,NULL);
	   quary.do_select_statement(parsed->select,&catalog,&buffer,&index);
	 }
	 if(parsed->insert){
		 //cout<<"1"<<endl;
                if(check_time==1) gettimeofday(&start,NULL);
	   quary.do_insert_statement(parsed->insert,&catalog,&buffer,&index);
	 }
	      
	 if(parsed->create_table){
	       if(check_time==1) gettimeofday(&start,NULL);
	   catalog.cata_createtable(parsed->create_table);     //Par

	 }
	 if(parsed->drop_table){
                     if(check_time==1) gettimeofday(&start,NULL);
	    catalog.cata_droptable(parsed->drop_table);    //Par
	 }
	 if(parsed->create_index){
	  /*  printf("create index %s on %s (%s);\n",
		  parsed->create_index->index,
		  parsed->create_index->table,
		  parsed->create_index->column);  */
                    if(check_time==1) gettimeofday(&start,NULL);
		//quary.do_create_index_statement(parsed->create_index,&index);
                  index.ind_create(parsed->create_index->index, parsed->create_index->table, parsed->create_index->column,parsed->create_index->duplicates, &catalog,&buffer); 

	 }
	 /*if(parsed->set){
	    print_set_statement(parsed->set);              //Timer?
	 }*/

	 if(parsed->drop_index){

                if(check_time==1) gettimeofday(&start,NULL);
         index.ind_drop(parsed->drop_index->index,parsed->drop_index->table, &catalog);
	 }
	 if(parsed->print_table){

                    if(check_time==1) gettimeofday(&start,NULL);
        quary.print_table(parsed->print_table,&catalog,&buffer);

	 }
	 if(parsed->print_index){

                  if(check_time==1) gettimeofday(&start,NULL);
        quary.print_index(parsed->print_index->index, parsed->print_index->table,&index);

	 }

	 if(parsed->parameterless == CMD_PRINT_CATALOG)
	 {   
                     if(check_time==1) gettimeofday(&start,NULL); 
             catalog.cata_print();
         }
	 if(parsed->parameterless == CMD_PRINT_BUFFER)
         {
                   if(check_time==1) gettimeofday(&start,NULL);
            buffer.buf_print();
         }
/*	 if(parsed->parameterless == CMD_PRINT_HIT_RATE)
         {
                 //     if(check_time==1) gettimeofday(&start,NULL);
            buffer.buf_printright();
         }*/  //for project3

	 if(parsed->parameterless == CMD_COMMIT)
	   {
                       if(check_time==1) gettimeofday(&start,NULL);
                
		   buffer.buf_commit_blo();
		   catalog.cata_commit();
	   }
 	  if(parsed->set){
		gettimeofday(&start,NULL);
           if ( parsed->set->variable==CONFIG_INDEX_DEBUG)
           {
			   quary.do_set_debug(parsed->set, &index);
           }
		   else
		   {
               		if(parsed->set->value==0) check_time=0;
		       	else check_time=1;
		   }
	 }	

      if(parsed->parameterless== CMD_PRINT_HIT_RATE) // project3
	  {
		 if(check_time==1) gettimeofday(&start,NULL);
		   char* f_input;
		   f_input = new char[100];
		
           statement_t* f_parsed=NULL;

	char pathandf[50]="large_data/a00";
/*
           char pathandf[30]="Exdata/";
           strcat(pathandf,parsed->rf->filename);
           strcat(pathandf,".txt");
*/

           ifstream inf(pathandf);
		   string s;
		   while(getline(inf,s))
		   {     
			   strcpy(f_input,s.c_str());

			   f_parsed=parse_statement(f_input);

     	                   quary.do_insert_statement(f_parsed->insert,&catalog,&buffer,&index); 
                           ki++;
                           if(ki%10000==0)    cout<<"bi= "<<ki/10000<<endl;
		   }
                ki=0;
	  }



	 if(parsed->parameterless == CMD_EXIT){
	              stillrunning=0;
		      if(check_time==1) gettimeofday(&start,NULL);
 
		   buffer.buf_commit_blo();		   catalog.cata_commit();

	 }
	// time_t endtime=now();
	// if(timing) 
	//	  printf("Elapsed time: %dus\n", endtime-begintime);
	}//if(parsed)
       else{
	 /* There was a syntax error, and the parser has already 
	  * printed an error message, so nothing to do here.*/
      }

	  if(check_time==1)
	   {    
		  gettimeofday(&end,NULL);
		  time_use=(end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec);
		//time_use=end.tv_sec-start.tv_sec;
		  cout<<"Execution time is:  "<<time_use<<" us"<<endl;
	   }
   

      free(input);            input=NULL;
      free_statement(parsed); parsed=NULL;
   }//while
}//main

char* myreadline(char* prompt){
   char* input=NULL;
   size_t inputlength=0;
   printf("%s",prompt);
   if (getline(&input,&inputlength,stdin)!=-1)
      return input;
   else
      return input;
}

/*
time_t now(){
   struct timeval t;
   gettimeofday(&t,0);
   return t.tv_sec*1000000+t.tv_usec;
}*/

