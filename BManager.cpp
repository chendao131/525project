#include"BManager.h"

void Buffer:: buf_init(int idnum,Catalog* catalog,Tablestorage* ts)
{

 //   list<Block> L;
//	list<Block> :: iterator it=this->scanb.begin();
//	it->blockno_in_bu=0;
	this->blonum=idnum;
	this->hit=0;
	this->hit_right=0;
	this->catalog=catalog;
    this->tstor=*ts;
	this->tstor.tablestorage_init(catalog);
//	this->scanb=L;

}


Block* Buffer:: buf_get_blo(char* tablename, int blo_no)
{
	
    //firstly, check the buffer where this block exists
	this->hit++;
	int j;
	int countinbuffer=0;                 //this parameter will record the length of current buffer
	int whether_fit=0;                   //this parameter will check whether the required block has been found. 0 means not while 1 means 
	Block* ba;
	ba=(Block*)malloc(sizeof(Block));
	Block* tb;
	list<Block> :: iterator it;
            it=this->scanb.begin();
	                       //      it=this->scanb.begin();

                          
	for(it=this->scanb.begin();it!=this->scanb.end();++it)
	{              
		   if(strcmp(tablename,(*it).tablename)==0&&blo_no==(*it).blockno_in_ta)   
                   {      //cout<<"________--1111111_________"<<endl;
                                             //  cout<<"_______________bbbbbbb111111111_aaaaaaaa______:  "<<endl; 
			this->hit_right++;
                        strcpy(ba->tablename,it->tablename);
                        ba->blockno_in_ta=it->blockno_in_ta;
                        ba->blockno_in_bu=it->blockno_in_bu;
			for(j=0;j<12;j++)
                              ba->bl_data[j]=it->bl_data[j];
		             ba->blo_remain=it->blo_remain;

			ba->blo_dirty=it->blo_dirty;
                        whether_fit=1;
			break;
		   }
                                //  cout<<"________________cccccccccccccccbbbbbbbbcccccccc_________"<<endl;
              		countinbuffer++;
   	}
       //  cout<<"_______________bbbbbbb2222222222222222_________:  "<<endl;
   /*  if(countinbuffer<this->blonum&&whether_fit==0) 
          {
                 hit_right++;
            }
      */
	if(it==this->scanb.end())                           //this block is not in buffer, then go to check disk
    {
        tb=tstor.tab_get_blo(tablename,blo_no);                   //return a pointer to the block I pick up

        strcpy(ba->tablename,tb->tablename);
        ba->blockno_in_ta=tb->blockno_in_ta;
        ba->blockno_in_bu=tb->blockno_in_bu;
    	for(j=0;j<12;j++)
            ba->bl_data[j]=tb->bl_data[j];
			int y=catalog->cata_get_table(tablename);
		ba->blo_remain=catalog->ta[y].remain_in_block;
            // cout<<"_______________bbbbbbbb333333333333__________:  "<<endl;
	}
      // cout<<"_______hit:________"<<hit<<"_______hitright______: "<<hit_right<<endl;
      //  for(int rr=0;rr<12;rr++)
		//	cout<<rr<<"Buffer_________ block is_________:  "<<ba->bl_data[rr]<<endl;
	return ba;

}

void Buffer:: buf_put_blo(Block* bll)
{
	 int j;
	 int countf=0;         //count how many blocks are in this buffer;
	 int change=0;         //whether this block is changed
	 int no_inb=0;
	 bll->blo_dirty=-1;
	 list<Block> :: iterator it;
	// list<Block> :: iterator ter;
       //  cout<<"____________block11111111_______"<<endl;
               // ter=this->scanb.begin();
//1st step: check whether this block is in the current buffer
             it=this->scanb.begin();
        while(it!=this->scanb.end())
    	{
              //  cout<<"____it_____: "<<&it<<endl;
               // cout<<"end________: "<<&this->scanb.end()<<endl;

          /*      cout<<"____________block1222222222___scanb.size()___"<<scanb.size()<<endl;
                cout<<"____(strcmp((*it).tablename______"<<(*it).tablename<<endl;
                 cout<<"_____bll->tablename______________ "<<bll->tablename<<endl;
                cout<<"_____(*it).blockno_in_ta______________ "<<(*it).blockno_in_ta<<endl;
                  cout<<"_____bll->blockno_in_ta_____________ "<<bll->blockno_in_ta<<endl;     */
            if((strcmp((*it).tablename,bll->tablename)==0)&&((*it).blockno_in_ta==bll->blockno_in_ta))
			{
                  //   cout<<"____________block12sssssssss_____"<<endl;
				 this->scanb.erase(it);
                 //   cout<<"____________block12qqqqqqqqqq____"<<endl;
				 this->scanb.push_back(*bll);
                  //  cout<<"____________block12ttttttttttttt_____"<<endl;
				 change=1;
                                 countf++;
                                 break;

                         }

			countf++;
                  //  cout<<"end________: "<<&this->scanb.end()<<endl;
                 //   cout<<"____it___before++__: "<<&it<<endl;
                         it++;
              //  cout<<"____it__after++___: "<<&it<<endl;


	}
     //2nd step: check whether the block has been replaced and whether this buffer is full

	    if(change==0)                             
	 {
         if(countf<this->blonum)
		 {
			 //cout<<"the length of this block is:________"<<this->blonum<<endl;
			 this->scanb.push_back(*bll);
			 countf++;
			 //cout<<"add a new block:"<<bll->tablename<<"and no:"<<bll->blockno_in_ta<<endl;

		 }
                 else //if(countf==this->blonum)
		 {
			list<Block> :: iterator LRUblock=this->scanb.begin();
//ready to move the LRU block to disk3
			Block* ba=(Block*)malloc(sizeof(Block));
            strcpy(ba->tablename,LRUblock->tablename);
            ba->blockno_in_ta=LRUblock->blockno_in_ta;
            ba->blockno_in_bu=LRUblock->blockno_in_bu;
        	for(j=0;j<12;j++)
                ba->bl_data[j]=LRUblock->bl_data[j];
	    	ba->blo_remain=LRUblock->blo_remain;
	    	ba->blo_dirty=LRUblock->blo_dirty;
            tstor.tab_put_blo(ba->tablename, ba->blockno_in_ta, ba);
			 this->scanb.erase(LRUblock);
			 this->scanb.push_back(*bll);

		 }
       it=this->scanb.begin();
       //  cout<<"____________block44444444444_______"<<endl;
	 }
	 // write the no of blocks in buffer
     for(it=this->scanb.begin();it!=this->scanb.end();it++)
	{
       it->blockno_in_bu=no_inb;
	   no_inb++;
	}

}

void Buffer:: buf_commit_blo()
{

	Block* tb;
    tb=(Block*)malloc(sizeof(Block));
	for(list<Block> :: iterator it=this->scanb.begin();it!=this->scanb.end();it++)
	{
		if(it->blo_dirty==-1)
		{  
            strcpy(tb->tablename,it->tablename);
            tb->blockno_in_ta=it->blockno_in_ta;
            tb->blockno_in_bu=it->blockno_in_bu;
			for(int j=0;j<12;j++)
                tb->bl_data[j]=it->bl_data[j];

			tb->blo_remain=it->blo_remain;
			tb->blo_dirty=0;
			it->blo_dirty=0;

//write this block to the disk.
            tstor.tab_put_blo(tb->tablename, tb->blockno_in_ta,tb);

		}
	}
}

void Buffer:: buf_print()
{
	cout<<"least recently used"<<endl;
    cout<<"buffer id\ttable name\tBlock id"<<endl;
    
	int f=1;
     for(list<Block> :: iterator it=this->scanb.begin();it!=this->scanb.end();it++)
	{
		 
		 cout<<""<<f<<"\t\t"<<it->tablename<<"\t\t"<<it->blockno_in_ta<<endl;
		 f++;
    }
	cout<<"most recently used"<<endl;
}

void Buffer:: buf_printright()
{
	printf("The hit rate is:\t %d/%d\n", this->hit_right,this->hit); 
}




