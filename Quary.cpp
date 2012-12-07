#include"Quary.h"

int Quary::check_sum(insert_statement_t* i,Catalog* cc){

	int op_i; 
	int op_j=0;
	int table_num=cc->cata_get_table(i->table);//get the number of table
	op_i = cc->ta[table_num].id_len;  //get the id_len of the table


	while(i->values){
		  ++op_j;
		  i->values=i->values->next;
	}
	if(op_i!=op_j)
	{
		cout<<"id num does not equal to n"<<endl;
		return 0;
	}
	return 1;
}


int Quary::calculate_conds(select_statement_t* i)
{
	int num=0;
	while(i->conditions)
	{
		
		num++;
		i->conditions=i->conditions->next;
	}
	//cout<<"num"<<num<<endl;
	return num;
}
 

void Quary::do_insert_statement(insert_statement_t* i, Catalog* cc, Buffer* b,Index* index)
{     //cout<<"here2"<<endl;
      num_list_t* sbegin=&*(i->values);
	  //cout<<"here1"<<endl;
	  Block block1;

	  Block* block;
      //used to return a pointer which could help to insert data;
	  
	  int op=cc->cata_get_table(i->table);// to check if there is such table exists or not
	       
  

      if( op==-1 ){
		  cout<<"there is no such file"<<endl; 
		  return;
	  }
 
      if(0==check_sum(i,cc))// to check if the number of ids equals to the number of numbers;
	  {
		  //cout<<check_sum(i,cc)<<endl;
		  return;
	  }
       //num_list_t* sbegin=&*(i->values);
           i->values=&(*sbegin);
      int table_number=cc->cata_get_table(i->table); // this is the number of table in the catalog

	  //cout<<table_number<<endl;
	  //cout<<"cc->ta[0].num_block: "<<cc->ta[table_number].num_block<<endl;
      int last_number=cc->ta[table_number].num_block-1; 

	  //cout<<"last_number: "<<last_number<<endl;
	  //int new_in=0;   //chenk whether a new block is established, 1 means yes


      // get the block;
      int remain_in_block=cc->ta[table_number].remain_in_block;
      //cout<<"cc->ta["<<table_number<<"].remain_in_block:  "<<cc->ta[table_number].remain_in_block<<endl;
      //cout<<remain_in_block<<endl;
	  //cout<<cc->ta[table_number].id_len<<endl;
      //cout<<cc->ta[table_number].num_block<<endl;
         int attoffset;
                     /*  int js=74;
                 vector<int> rf;  cout<<"________________tt1____________"<<endl;
                         index->head.header_disk2mom(cc->ta[table_number].cata_table_name);
                    rf=index->ind_lookup(js);                  cout<<"________________tt22222___________"<<endl;
                 cout<<"____________lookupss______: "<<rf.size()<<endl;  */
                //   cout<<"_____________insert1111111________"<<endl;
          for(int pre=0;pre<3;pre++)
          {
                  i->values=&(*sbegin);
          	  attoffset=cc->ta[table_number].at_index[pre];  
               
/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%  For Three Indices  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	      for(int pre=0;pre<2;pre++)
          {
                  i->values=&(*sbegin);
          	  attoffset=cc->ta[table_number].at_index[pre];  
	  */	
                
		  if (attoffset!=-1)
		  {
	               for (int j = 0; j < attoffset; j++)
	               {

		           i->values = i->values->next;

	               }  
	               int prekey=i->values->num;
                       char pttname[8];
                       strcpy(pttname, cc->ta[table_number].at_index_name[pre]);
                                        //   cout<<"___prekey_____: "<<prekey<<endl;
                                         //  cout<<"___dup:_______: "<<index->head.indh_check_dup<<endl;
                       int testcc=index->ind_pretest(pttname,prekey,cc);
                       if(testcc==-1)
                       {
			       cout<<"Error: duplicate: Attempted to add duplicate value to `no duplicates' index"<<endl;
                               return;
                       }
                  }
            }   

               
	  for (int so=0; so<3; so++)
	  {
                          i->values=&(*sbegin);
               // cout<<"____cc->ta[table_number].at_index_name["<<so<<"]:  "<<cc->ta[table_number].at_index_name[so]<<endl;
		  attoffset=cc->ta[table_number].at_index[so];  

/*  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% For three indices  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	  for (int so=0; so<2; so++)
	  {
                          i->values=&(*sbegin);zz
               // cout<<"____cc->ta[table_number].at_index_name["<<so<<"]:  "<<cc->ta[table_number].at_index_name[so]<<endl;			  
		  attoffset=cc->ta[table_number].at_index[so];  
*/

		  if (attoffset!=-1)
		  {
	               for (int j = 0; j < attoffset; j++)
	               {

		           i->values = i->values->next;

	               } //     num_list_t* sbegin=&*(i->values);
   
	               int key=i->values->num;

                       char attname[8];
                       int newv;
                       strcpy(attname, cc->ta[table_number].at_index_name[so]);

                if( remain_in_block  <  cc->ta[table_number].id_len )
	            {
		            newv=cc->ta[table_number].num_block;
	            }
	            else newv=cc->ta[table_number].num_block-1;
                        int result;

                   /*   cout<<"_______cc->ta[table_number].cata_table_name_____: "<<cc->ta[table_number].cata_table_name<<endl;
                      cout<<"_______attname________________: "<<attname<<endl;
                      cout<<"____________key_______________: "<<key<<endl;
                      cout<<"____________newv_______________: "<<newv<<endl;   */            
                           //        cout<<"____so:" <<so<<" :________key_______________: "<<key<<endl;
	              result=index->ind_q_insert(cc->ta[table_number].cata_table_name, attname, key, newv, cc); 
                         // cout<<"____so__:"<<so<<"_____________quary___result_______: "<<result<<endl;
	            if (result==-1)
	            {
		             return;
	            }
		  }
	  }

           i->values=&(*sbegin);
	 /* int attoffset=index->head.indh_attributeno;
	  for (int j = 0; j < attoffset; j++)
	  {
		  i->values = i->values->next;
	  }
	  int key=i->values;
	  char attname[8];
	  strcpy(attname, cc->ta[table_number].)
      if( remain_in_block  <  cc->ta[table_number].id_len )
	  {
		  newv=cc->num_block;
	  }
	  else newv=cc->num_block-1;
      
	  int result=index->ind_q_insert(table_name, char* attributename, int key); 
	  if (result==-1)
	  {
		  return;
	  }
	  */
      if( remain_in_block  <  cc->ta[table_number].id_len )
      {
		   //cout<<cc->ta[table_number].num_block<<endl;
           //block =(Block*)malloc(sizeof(Block));
           b->hit++;
		   b->hit_right++;

		   strcpy(block1.tablename,i->table); //insert the name to block's table name;
                            //       cout<<"______________quaryafterresult_____block1.tablename___: "<<block1.tablename<<endl;
		   block1.blockno_in_ta=cc->ta[table_number].num_block;//num_block++ after that
		   block1.blockno_in_bu=-1;
	       memset(block1.bl_data,0,12*sizeof(int)); //insert 0 to block's storage bl_data[12]
	       block1.blo_remain=12;
		   block1.blo_dirty=-1; 
            //which means we should create a new block;
           ++cc->ta[table_number].num_block;
                         // cout<<"_________________1111111_______: "<<endl;
	  }       

      else{
		   block=b->buf_get_blo(i->table,last_number);//get block

		   strcpy(block1.tablename,block->tablename);
		   block1.blockno_in_ta=block->blockno_in_ta;
	       for (int t=0;t<12 ;t++ )
           {
               block1.bl_data[t]=block->bl_data[t];
           }

	       block1.blo_remain=block->blo_remain;
		   block1.blo_dirty=-1; 
	  }

      cc->ta[op].ta_status=1;
	  //cout<<sbegin->num<<endl;
	  block1.insertdata(cc->ta[table_number].id_len,sbegin);

	  	   cc->ta[table_number].remain_in_block=block1.blo_remain;

	   cc->ta[table_number].remain_in_block=block1.blo_remain;
	   block=&block1;
                      //  cout<<"________________44444444___aaaaaaaa____: "<<endl;
            //   for(int ss=0;ss<12;ss++)
              //        cout<<"
       b->buf_put_blo(block);     //put block back  
                     //     cout<<"_______________5555555555555______: "<<endl;   

}


void Quary::do_set_debug(set_statement_t* i, Index* index)
{
	if(i->variable==CONFIG_INDEX_DEBUG)
	{
		index->ind_set_debug(i->value);
	}
}

/*void Quary::do_create_index_statement(create_index_statement_t *i,Index* index)
{
     index->ind_create(i->index,i->table,i->column,i->duplicates);
}
*/

void Quary::do_select_statement(select_statement_t* i,Catalog* cc,Buffer* b,Index* index)
{
    int h,a;
    int v=0;
    int j=0;
    int k=0;
    int vv,spt1,spt2,sv1,sv2,spttemp,svtemp;
    int m=0;
    int n=0;
	int jd;
	int tid;
	int judge_index=-1;         //-1 means no index exists, 1 means some index exists. 
	condition_t* temp_pointer;
	int conds_num_original,conds_num_followed; //used to calculate the number of condition;
	int record_num_sum;
    int table_number=cc->cata_get_table(i->table);
    int id_right_no; //used to store the right col
    int offset1,offset2;
	id_list_t* f=&*i->fields;
    Block* temp;//store temp block;
    int record_num;//the number of record in each block;
	vector<int> vec1;
	vector<int> vec2;
    vector<int> comvec;
	int opt=cc->cata_get_table(i->table);//check whether there is already a table or not;
    //cout<<op<<endl;
	if(opt==-1){
    cout<<"no such table"<<endl;
	return;
    }          //   cout<<"___select11111111111_______"<<endl;
	int offset[cc->ta[table_number].id_len];//used to store the offset of the ids
    //analyze the id_list
	if(i->fields){
		while(i->fields)
		{  
			offset[n]=cc->cata_get_attribute(i->table,i->fields->id);
			//cout<<"here3"<<endl;
			if(offset[n]==-1)
			{
				cout<<"no such id"<<endl;
				return;
			}
			n++;
			i->fields=i->fields->next;
		}
		//cout<<"n"<<n<<endl;
	}
	else{
		for(n=0; n<cc->ta[table_number].id_len; n++)
			offset[n]=n;
	}


    a = cc->ta[table_number].num_block-1;

	record_num=12/cc->ta[table_number].id_len;
		record_num_sum = record_num;
       //  cout<<"record_num :__ "<<record_num<<"  ______record_num_sum:  "<<record_num_sum<<endl;
//where judge
	if (!i->conditions)
        {           // cout<<"________!i->conditions_________________________"<<endl;
	    if(!f)
	    {
		//cout<<"if step"<<endl;
                for (int s=0;s<cc->ta[table_number].id_len;s++)
                {   
			
			cout<<cc->ta[table_number].id_content[s]<<"	";
			
                }
                cout<<endl;
	    }
            else{
		do{
			cout<<f->id<<"	";
			f=f->next;
		}while(f);
		cout<<endl;
	}// cout the list row.
        while(a>=m) //get block  m is the number of block;
       {
           temp=b->buf_get_blo(i->table,m);
	 //  cout<<"___select444444__22222222_____"<<endl; 
	   if(a==m)
           {// the record num of the last block equals to remain_in_block;
	        record_num =(12 - cc->ta[table_number].remain_in_block )/ cc->ta[table_number].id_len;
	   }
	   else
           {
			record_num=12 / cc->ta[table_number].id_len;
	   }  
	   record_num_sum = record_num;
                                                    //cout<<"_____n: "<<n<<"___record_num_sum___"<<record_num_sum<<"____record_num:"<<record_num<<endl;
	   while (record_num>0)                                                        // for ( int h=0; h<cc->ta[table_number].num_block; h++)
	   {		 //  cout<<"___select444444__333333333____"<<endl;
			for(k=0;k<n;k++)
			{      	               // cout<<"___select444444__444444444444_____"<<endl;
                                                 // cout<<"__offset["<<k<<"]: "<<offset[k]<<endl;
                                                    // cout<<"__cc->ta[table_number].id_len_____: "<<cc->ta[table_number].id_len<<endl;
                                                  //   cout<<"_for bl_data: "<<offset[k]+(record_num_sum - record_num)*cc->ta[table_number].id_len<<endl;
				cout<<temp->bl_data[offset[k]+(record_num_sum - record_num)*cc->ta[table_number].id_len]<<"	";
	                              //  cout<<"___select444444__555555555_____"<<endl;
			}

			cout<<endl;
			record_num--;
	  }
                        b->buf_put_blo(temp);
        m++;
       }
    }
// where_conditions exist
    if (i->conditions)
    {                    // cout<<"________________condi****exist________________"<<endl;
        	int cjudge;
                 cjudge=cc->cata_get_attribute(i->table,i->conditions->left_col);
                if(cjudge==-1)
                {
                    cout<<"this attribute does not exist in this table, reinput, please."<<endl;
                    return;
                }
		temp_pointer=&(*i->conditions);
		// Keep the beginning position of i
		conds_num_original=calculate_conds(i);
		conds_num_followed=conds_num_original;
		i->conditions=temp_pointer;
		// Syntax where_condition
		int fj, pos_intable,atkey;
        do
        { 
	     pos_intable=cc->cata_get_attribute(cc->ta[table_number].cata_table_name, i->conditions->left_col);
                 // cout<<"_________pos_intable________: "<<pos_intable<<endl;
	     for(fj=0;fj<3;fj++)
/*  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% For three indices  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		 for(fj=0;fj<2;fj++)
*/
             {
                      // cout<<"_______cc->ta[table_number].at_index["<<fj<<"]________: "<<cc->ta[table_number].at_index[fj]<<endl;
                  if(cc->ta[table_number].at_index[fj]==pos_intable && i->conditions->op!=6)
                  {   judge_index=1; atkey=i->conditions->right_num; break;
                  }
             }
		     i->conditions=i->conditions->next;
        }
        while (i->conditions!=NULL && judge_index!=1);
		i->conditions=temp_pointer;
		condition_t* temp_cond;

		int judgecond=1; //-1 means some condition does not match 

             //  cout<<"_________judge_index____: "<<judge_index<<endl;
	if (judge_index==-1)  // no idnex
	{
                      //  cout<<"_________judgeindex==-1_________111111___"<<endl;
	    condition_t* temp_cond=(condition_t*)malloc(sizeof(condition_t));
	    a = cc->ta[table_number].num_block-1; m=0;
	    if(!f)
	    {
		         //cout<<"if step"<<endl;
                for (int s=0;s<cc->ta[table_number].id_len;s++)
                {   
		    cout<<cc->ta[table_number].id_content[s]<<"	";
			
                }
                cout<<endl;
	    }
            else
	    {
		        do{
			           cout<<f->id<<"	";
			           f=f->next;
		        }while(f);
		        cout<<endl;
	     }// cout the list row.
             while(a>=m) //get block  m is the number of block;
             {
                 temp=b->buf_get_blo(i->table,m);

	             if(a==m)
                     {// the record num of the last block equals to remain_in_block;
	                    record_num =(12 - cc->ta[table_number].remain_in_block )/ cc->ta[table_number].id_len;
	             }
	             else
                     {
			            record_num=12 / cc->ta[table_number].id_len;
	             }  
	             record_num_sum = record_num;
			 	 while (record_num>0)                                                       
	             { //cout<<"lp:"<<temp->bl_data[offset1+(record_num_sum - record_num)*cc->ta[table_number].id_len]<<endl; 
					 while (i->conditions!=NULL && judgecond!=-1)
					 {
						 temp_cond->left_col=&(*(i->conditions->left_col));
                         temp_cond->op=i->conditions->op;
                         temp_cond->right_col=&(*(i->conditions->right_col));
                         temp_cond->right_num=i->conditions->right_num;
					     offset1=cc->cata_get_attribute(cc->ta[table_number].cata_table_name, temp_cond->left_col);
					     switch(temp_cond->op)
				         {
                             case  OP_EQUAL: 
                               if(temp->bl_data[offset1+(record_num_sum - record_num)*cc->ta[table_number].id_len] 
                                     != temp_cond->right_num)                                       
                               {
							       judgecond=-1;  
                               }break;
			     case  OP_LESS:
				if(temp->bl_data[offset1+(record_num_sum - record_num)*cc->ta[table_number].id_len] 
                                     >= temp_cond->right_num) 
				{     
								    judgecond=-1; 
				} break;
			     case OP_LESS_EQUAL:
                                if(temp->bl_data[offset1+(record_num_sum - record_num)*cc->ta[table_number].id_len] 
                                     > temp_cond->right_num) 
				{     
								    judgecond=-1;  
				}break;
	                     case OP_GREATER:
                                if(temp->bl_data[offset1+(record_num_sum - record_num)*cc->ta[table_number].id_len] 
                                     <= temp_cond->right_num) 
				{     
								    judgecond=-1; 
				} break;
	                     case OP_GREATER_EQUAL:
                                if(temp->bl_data[offset1+(record_num_sum - record_num)*cc->ta[table_number].id_len] 
                                     < temp_cond->right_num) 
				{     
								    judgecond=-1; 
				} break;
	                     case OP_NOT_EQUAL:
                                if(temp->bl_data[offset1+(record_num_sum - record_num)*cc->ta[table_number].id_len] 
                                     == temp_cond->right_num) 
				{     
								    judgecond=-1;
				}  break;

			 }
                         i->conditions=i->conditions->next;
					 }//while (i->conditions!=NULL)

                     if(judgecond==1)
		     {
                         for(k=0;k<n;k++)
			 {      	             
			     cout<<temp->bl_data[offset[k]
                                  +(record_num_sum - record_num)*cc->ta[table_number].id_len]<<"	";
			 }
                         cout<<endl;
		     }

                     judgecond=1;
                     record_num--;
		     i->conditions=temp_pointer;
			
	         }//while (record_num>0)  
                 b->buf_put_blo(temp);
                 m++;
          }//while(a>=m)                
 
     }

     if (judge_index==1) // index is needed
     {
          //cout<<"_________judgeindex==1_________111111___"<<endl;
	  temp_cond=(condition_t*)malloc(sizeof(condition_t));             //cout<<"temp_cond->op:  "<<temp_cond->op<<endl;
	  switch(i->conditions->op)
	  {
               case  OP_EQUAL: //cout<<"______1______"<<endl;
		            vec1=index->ind_select_equal(table_number, pos_intable, atkey); 
                            //vec1=do_reduce_dup(vec1); 
                             break;
	       case  OP_LESS:  //cout<<"______2______"<<endl;
		            vec1=index->ind_select_smaller(table_number, pos_intable, atkey); 
                            //vec1=do_reduce_dup(vec1); 
                            break;
               case  OP_GREATER:   //cout<<"______3______"<<endl;
		            vec1=index->ind_select_greater(table_number, pos_intable, atkey);
                            //vec1=do_reduce_dup(vec1); 
                             break;
               case  OP_LESS_EQUAL:  //cout<<"_____4______"<<endl; 
		            vec1=index->ind_select_smallerequal(table_number, pos_intable, atkey);
                            //vec1=do_reduce_dup(vec1); 
                            break;
               case  OP_GREATER_EQUAL:   //cout<<"______5_____"<<endl;
		            vec1=index->ind_select_greaterequal(table_number, pos_intable, atkey);
                            //vec1=do_reduce_dup(vec1); 
                            break;				 
            /*   case  OP_NOT_EQUAL:   //cout<<"______6______"<<endl;
		            vec1=index->ind_select_notequal(table_number, pos_intable, atkey); break;       */
	  }
			 // Get index vec1 as the checking goal

	  if(!f)
	  {
		         //cout<<"if step"<<endl;
               for (int s=0;s<cc->ta[table_number].id_len;s++)
               {   
		    cout<<cc->ta[table_number].id_content[s]<<"	";
			
               }
               cout<<endl;
	  }
          else
          {
	      do{
		  cout<<f->id<<"	";
		  f=f->next;
	      }while(f);
	      cout<<endl;
	  }// cout the list row.
	  a =vec1.size(); m=0;      
               // for(int huu=0; huu<vec1.size(); huu++)         cout<<"___vec1["<<huu<<"]__: "<<vec1[huu]<<endl;
               // cout<<"___________vec1.size(): "<<vec1.size()<<endl;
          while(a>m) //get block  m is the number of block;
          {
                 temp=b->buf_get_blo(i->table,vec1[m]);

	         if(vec1[m]==cc->ta[table_number].num_block-1)
                 {// the record num of the last block equals to remain_in_block;
	                    record_num =(12 - cc->ta[table_number].remain_in_block )/ cc->ta[table_number].id_len;
	         }
	             else
                 {
			            record_num=12 / cc->ta[table_number].id_len;
	         }  
	             record_num_sum = record_num;
		 while (record_num>0)                                                       
	         { //cout<<"lp:"<<temp->bl_data[offset1+(record_num_sum - record_num)*cc->ta[table_number].id_len]<<endl; 
		     while (i->conditions!=NULL && judgecond!=-1)
		     {                         //cout<<"_________judgeindex==1_________333333333___"<<endl;
			 temp_cond->left_col=&(*(i->conditions->left_col));
                         temp_cond->op=i->conditions->op;
                         temp_cond->right_col=&(*(i->conditions->right_col));
                         temp_cond->right_num=i->conditions->right_num;
			 offset1=cc->cata_get_attribute(cc->ta[table_number].cata_table_name, temp_cond->left_col);

			 switch(temp_cond->op)
			 {
                             case  OP_EQUAL: 
                                 //cout<<"__bb:_"<<temp->bl_data[offset1+(record_num_sum - record_num)*cc->ta[table_number].id_len]<<endl;
                               if(temp->bl_data[offset1+(record_num_sum - record_num)*cc->ta[table_number].id_len] 
                                     != temp_cond->right_num)                                       
                               {
				    judgecond=-1;  
                               }  break;
			     case  OP_LESS:
				if(temp->bl_data[offset1+(record_num_sum - record_num)*cc->ta[table_number].id_len] 
                                     >= temp_cond->right_num) 
				{     
					judgecond=-1; 
				} break;
			     case OP_LESS_EQUAL:
                                if(temp->bl_data[offset1+(record_num_sum - record_num)*cc->ta[table_number].id_len] 
                                     > temp_cond->right_num) 
				{     
					judgecond=-1;
				}  break;
	                     case OP_GREATER:
                                if(temp->bl_data[offset1+(record_num_sum - record_num)*cc->ta[table_number].id_len] 
                                     <= temp_cond->right_num) 
				{     
					judgecond=-1; 
				} break;
	                     case OP_GREATER_EQUAL:
                                if(temp->bl_data[offset1+(record_num_sum - record_num)*cc->ta[table_number].id_len] 
                                     < temp_cond->right_num) 
				{     
					judgecond=-1; 
				} break;
	                 /*      case OP_NOT_EQUAL:
                                if(temp->bl_data[offset1+(record_num_sum - record_num)*cc->ta[table_number].id_len] 
                                     == temp_cond->right_num) 
				{     
					judgecond=-1;
				}  break;  */
			 }//switch(temp_cond->op)
                         i->conditions=i->conditions->next;
		     }//while (i->conditions!=NULL)

                     if(judgecond==1)
		     {
                         for(k=0;k<n;k++)
			             {      	             
				                  cout<<temp->bl_data[offset[k]
                                         +(record_num_sum - record_num)*cc->ta[table_number].id_len]<<"	";
			             }
                         cout<<endl;
                     }
                     judgecond=1;
                     record_num--;
		     i->conditions=temp_pointer;
		     //cout<<"__record_num: "<<record_num<<endl;   
	         }//while (record_num>0) 
                 b->buf_put_blo(temp);
                 m++;
             } //while(a>=m)               
         vec1.clear();
	}// if (judge_index==1) 
    }//if (i->conditions)
}
                        
                       
                       
                                                    
void Quary::print_table( char* table, Catalog* cc, Buffer* b)
{
	int n;
	int record_num,record_num_sum;
	int k;
	int m=0; //used to compare the block;
	Block* temp; //used to store the got block;
	int table_number = cc->cata_get_table(table);//table num
	int opt=cc->cata_get_table(table);//check whether there is already a table or not;
    //cout<<op<<endl;
	if(opt==-1){
    cout<<"no such table"<<endl;
	return;
    }
	
	for (int s=0;s<cc->ta[table_number].id_len;s++) //print the names of the table;
        {   
			
			cout<<cc->ta[table_number].id_content[s]<<"	";
			
        }

	cout<<endl; //divide id from number;

	int a = cc->ta[table_number].num_block-1; // a = the number of the table block -1;
	while( a >= m )
	{
		temp=b->buf_get_blo(table,m); // get a block from buffer;
		if(a==m){// the record num of the last block equals to remain_in_block;
		record_num =(12 - cc->ta[table_number].remain_in_block )/ cc->ta[table_number].id_len;
		}//if
		else{
			record_num=12 / cc->ta[table_number].id_len;
		} //else 
		record_num_sum = record_num;
        while(record_num>0) //get record
        { 
			for(k=0;k<cc->ta[table_number].id_len;k++){
				cout<<temp->bl_data[k+(record_num_sum - record_num)*cc->ta[table_number].id_len]<<"	";
			}//for;
			cout<<endl;
			record_num--;
		}//while(record_num>0);
		m++;//until a<m;
		b->buf_put_blo(temp);//return the block back to the buffer;
	}//while
}
                           
void Quary::print_index(char* indexname, char* tablename, Index* index)
{
         FILE *fp;
         char pathandf[30]="data/";
         strcat(pathandf,indexname);

         if((fp=fopen(pathandf,"r"))==NULL)
         {
                cout<<" This index doesn't exist or has been deleted."<<endl;
                return;
         }
       index->ind_q_print(indexname, tablename);
}

vector<int> Quary:: do_equal_com(vector<int> vec1, vector<int> vec2)
{
	vector<int> resultvec;
	for (int xv=0; xv<vec1.size(); xv++)
	{
		for (int xs=0; xs<vec2.size(); xs++)
		{
			if (vec1[xv]==vec2[xs])
			{
				resultvec.push_back(vec1[xv]);
				break;
			}
		}
	}
	vec1.clear();
	vec2.clear();
         /*    if(resultvec.size()>0)
               {    cout<<"____resultvec.size:__ "<<resultvec.size()<<" ___resultvec[0]:__ "<<resultvec[0]<<endl;}     */
	return resultvec;
}

vector<int> Quary:: do_reduce_dup(vector<int> invec)
{
     
       for(int ja=(invec.size()-1);ja>=0;ja--)
       {
            for(int hw=ja-1; hw>=0; hw--)
            {
                  if(invec[ja]==invec[hw])
                  {
                       invec.pop_back();
                       break;
                  }
            }
       }
     return invec;
}
            









              
