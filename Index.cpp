#include"Index.h"
//Make sure this index is unique
int Index:: ind_index_unique(char* indname)
{
         FILE *fp;
         char pathandf[30]="data/";
         strcat(pathandf,indname);

         if((fp=fopen(pathandf,"r"))!=NULL)
          {
              fclose(fp);
	      //This name should be unique
	      return -1;
         }
         else
         {
	     return 0;
         }

}


void Index:: ind_ta_att_exist(char* tablename, char* attributename)
{  
         int h=this->catalog->cata_get_table(tablename);

	this->head.indh_tableno=this->catalog->cata_get_table(tablename);


    this->head.indh_attributeno=this->catalog->cata_get_attribute(tablename,attributename);

}

int Index:: ind_check_indnum(int tableno, int attrno)
{
	int indch;
	indch=this->catalog->cata_setindex(tableno, attrno);
	return indch;
}

void Index:: ind_create_indisk(char* fname)
{
	FILE *fp;

    char pathandf[30]="data/";
    strcat(pathandf,fname);
	fp=fopen(pathandf,"w+");
	fclose(fp);
    
}

Mixblock Index:: ind_getblock(int offset, char* fname)
{
	Mixblock tempm;
	FILE *fp;
    char pathandf[30]="data/";
    strcat(pathandf,fname);
	fp=fopen(pathandf,"r");
    fseek(fp, offset*12*sizeof(int), 0);                

    for (int i=0; i<12; i++)
    {
		fread(&tempm.mx_data[i], sizeof(int), 1,fp);
    }
	fclose(fp);
	tempm.mx_locinfile=offset;
    return tempm;
}

void Index:: ind_putblock(Mixblock mm,int offset,char* fname)
{
    FILE *fp;
    char pathandf[30]="data/";
    strcat(pathandf,fname);
	fp=fopen(pathandf,"r+");

	fseek(fp, offset*12*sizeof(int), 0);
                //for(int rr=2;rr<11;rr++) { cout<<"mm_______"<<rr<<":_____"<<mm.mx_data[rr]<<"   ";}
    for (int i=0; i<12; i++)
    {
		fwrite(&mm.mx_data[i], sizeof(int), 1,fp);
    }
	fclose(fp);
}

void Index:: ind_set_debug(int de)
{
	this->ind_debug=de;
}

vector<int> Index:: ind_lookup(int key)
{
    vector<int> tempint;
	 Mixblock mbtemp;
	 int c,left=0;
                                //    cout<<"_____________lookup1111111_______"<<endl;
         	               /*  Mixblock mbin1=this->ind_getblock(this->head.indh_root_loc, this->head.indh_name);
                                 cout<<"__loo111111111__this->head.indh_root_loc_"<<this->head.indh_root_loc<<endl;
                                 for(int hh=0;hh<11; hh++) cout<<"mbtemp.mx_data["<<hh<<"]____"<<mbin1.mx_data[hh]<<endl;
                                  */
	 int offset=this->head.indh_root_loc;
                                      //   cout<<"______this->head.indh_root_loc__: "<<this->head.indh_root_loc<<endl;
                                      // cout<<"___________lo11111111111111___________"<<endl;
	 do
	 {  
		 mbtemp=ind_getblock(offset, this->head.indh_name);
                  this->mxb.push_back(mbtemp);
            // cout<<"offset:_"<<offset<<"______mbtemp.mx_data[0]: __________"<<mbtemp.mx_data[0]<<endl;
                               /*      if(key==193)   {     cout<<"__mbtemp__offset_"<<offset<<endl;
                                             for(int hh=0;hh<11; hh++) cout<<"mbtemp.mx_data["<<hh<<"]____"<<mbtemp.mx_data[hh]<<endl;     }  */
                                             // cout<<"___________lo222222222222___________"<<endl;
                                           
		 if (mbtemp.mx_data[0] == 2)
		 {
                                   //  cout<<"___________type==2*********______"<<offset<<endl;
			 for (c=1; c<5; c++)
			 {
				 if (key==mbtemp.mx_data[c])
				 {              //    if(key==193)    cout<<"___________lookup__if1111111______"<<endl;       
                                         left=1;
                                        // cout<<"_____before_____________: "<<offset<<endl;
					 offset=mbtemp.mx_data[c+5];
                                        // cout<<"_______lif1111111111____: "<<offset<<endl;            
					 break;
				 }
				 if(key<mbtemp.mx_data[c])
				 {               //        if(key==193)      cout<<"___________lookup__if__key<mbtemp.mx_data[c]______"<<endl;
				     offset=mbtemp.mx_data[c+4];

                                     break;
				 }
				 if (mbtemp.mx_data[c]==-1)
				 {                  //         if(key==193)       cout<<"___________lookup__if___mbtemp.mx_data[c]==-1______"<<endl; 
					 offset=mbtemp.mx_data[c+4]; 

                                                       //  cout<<"____________loc_________:"<<mbtemp.mx_locinfile<<endl;
                                   
                                                       // cout<<"_______offset___c==4____: "<<offset<<endl;             
					 break;
				 }
                                 if (c==4&&key>mbtemp.mx_data[c])
                                 {
                                         offset=mbtemp.mx_data[c+5];
                                         break; 
                                 }

			 }
		 }

	 } while (mbtemp.mx_data[0]!=1);
	 if (mbtemp.mx_data[0] == 1)
	 {            //   cout<<"_____________________lookupaaaaaaaaaaaaaaaaaaaaaaa___________________"<<endl;
		 if (left==0)
		 {                    //  cout<<"__________left=0  &&&&&&&&&&__________"<<endl;
		    for(c=1; c<5; c++)
		    {
                           if(key==mbtemp.mx_data[c])
			   {
				   offset=mbtemp.mx_data[c+4];
				   left=1;
				   break;
			   }

		    }
		 }
                                      //   cout<<"_____________________lookupleftleft___________________"<<left<<endl;
                 if(left ==1)
		 {
		          for(c=1; c<5; c++)
		          {
                               if(key==mbtemp.mx_data[c])
			       {     //    cout<<"___________________lookupleaf__11111_______________"<<left<<endl;
				   offset=mbtemp.mx_data[c+4];
				   break;
			       }

		         }
           int seenext=-1;
			 do
			 {	
				 mbtemp=ind_getblock(offset, this->head.indh_name);
                                 seenext=mbtemp.mx_data[11];
                                  // cout<<"___________________lookupleaf__122222222_____________"<<left<<endl;
			     for (c=2; c<11; c++)
			     {            
                                   if(mbtemp.mx_data[c]!=-1) 
                                   {
                                       tempint.push_back(mbtemp.mx_data[c]);
                                         	// return tempint;
                                   }
                                   if (mbtemp.mx_data[c] ==-1)
                                   {                                   
				         break;
                                    }

			     }
                              if(c==11&&seenext!=-1) 
                               offset=mbtemp.mx_data[11]; 
			 }
			 while (c==11&&seenext!=-1);
		 }
	 }
        //    cout<<"____lookup______setsize__: "<<tempint.size()<<endl;
	 return tempint;
}

int Index:: ind_pretest(char* indexname,int key,Catalog* cc)
{
    this->catalog=cc;
    this->head.header_disk2mom(indexname);
    vector<int> tempp;
    tempp=ind_lookup(key);
       // cout<<"_____tempp.size():_____"<<tempp.size()<<endl;
       // cout<<"_____this->head.indh_check_dup___:"<<this->head.indh_check_dup<<endl;
            this->mxb.clear();
    if(tempp.size()>0&&this->head.indh_check_dup==0)
    {          
            return -1;
    }
    else    return 1;
}

int Index:: ind_find_value(int key, int value)
{
	this->pointset=ind_lookup(key);
//cout<<"___________f11111111___________"<<endl;
	if (this->ind_debug==1)
	{
                  //   cout<<"__________mxb.size()_H________: "<<mxb.size()<<endl;
		for (int h=0; h<mxb.size(); h++)
	    {
		    cout<<"--Node: "<<mxb[h].mx_locinfile<<" Type: ";
			if (mxb[h].mx_data[0]==2)
			{
				cout<<" internal Function: find Value:"<<key<<endl;
			}
			if (mxb[h].mx_data[0]==1)
			{
				cout<<" leaf Function: find Value:"<<key<<endl;
            }
			
	    }
	}
}


int Index:: ind_insert(int key, int value)
{                // if(key==94&&value==254)      cout<<"______key_____: "<<key<<"______value_____: "<<value<<endl;
	ind_find_value(key,value);
	Bucket newb;
	int buckey,xi,xkey,xptr,leafs;
        int huhu=0;
        //   if(key==94&&value==254) huhu=1;
	Mixblock newmx;
       // cout<<"___________In11111111___________"<<endl;
            /*    cout<<"_key:"<<key<<" __this->head.indh_check_dup___: "<<this->head.indh_check_dup<<endl;
                int js=this->pointset.size();
                cout<<"js________: "<<js<<endl; */
               
                   //   cout<<head.indh_name<<":_____looupsize:  "<<this->pointset.size()<<endl;
                 //     cout<<"_____________head.indh_check_dup:_____"<<head.indh_check_dup<<endl;
	if (this->head.indh_check_dup==0)
	{


		if(this->pointset.size()>0)
		{
			cout<<"Error: duplicate: Attempted to add duplicate value to `no duplicates' index"<<endl;
                         ind_drop(head.indh_name,head.indh_tablename,catalog);
		    return -1;
                }
                                                                    //cout<<"___________In222222___________"<<endl;
		newb.bu_insert(key, value);
                //for(int rr=0;rr<9;rr++) { cout<<"newb____"<<rr<<":"<<newb.bptr[rr]<<"   ";}
		this->head.indh_nodetotal++;
		buckey=this->head.indh_nodetotal;
		newmx.mx_Bucket2Mixblock(newb);
		this->ind_putblock(newmx,buckey,this->head.indh_name);
                                                                              //cout<<"___________In333333333___________"<<endl;
		if (this->ind_debug==1)
	    {//-- Node: 5 Type: pointer bucket Function: insert Value: 0
			cout<<"--Node: "<<buckey<<" Type:  pointer bucket Function: insert Value: "<<value<<endl;	
		}
		if (this->ind_debug==1)
	    {
		    for (int h=0; h<mxb.size(); h++)
	        {
		        cout<<"--Node: "<<mxb[h].mx_locinfile<<" Type: ";
			    if (mxb[h].mx_data[0]==2)
			    {
			    	cout<<" internal Function: insert Value:"<<key<<endl;
			    }
			    if (mxb[h].mx_data[0]==1)
			    {
				    cout<<" leaf Function: insert Value:"<<key<<endl;
                }
			
	        }
	    } 	
                     //   if(huhu==1)  cout<<"_________key__111111___:"<<key<<endl;
		xkey=key;
		xptr=buckey;
           // cout<<"______xkey____: "<<xkey<<"_________xptr_________: "<<xptr<<endl;
                leafs=mxb[this->mxb.size()-1].mx_locinfile;
		for (xi=(this->mxb.size()-1); xi>=0; xi--)
		{
			tempnode=mxb[xi].mx_Mixblock2Node();
			Node xnode=tempnode.node_insert(xkey, xptr);


			if (this->ind_debug==1)
	                 {
                            if(xkey!=key)
                            {	
	                           cout<<"--Node: "<<leafs<<" Type: ";
			            if (mxb[xi].mx_data[0]==2)
			             {
			    	            cout<<" internal Function: insert_here Value:"<<key<<endl;
			                }
			             if (mxb[xi].mx_data[0]==1)
			             {
				              cout<<" leaf Function: insert_here Value:"<<key<<endl;
                                      }
                            }
		            cout<<"--Node: "<<mxb[xi].mx_locinfile<<" Type: ";
			    if (mxb[xi].mx_data[0]==2)
			    {
			    	cout<<" internal Function: insert_here Value:"<<xkey<<endl;
			    }
			    if (mxb[xi].mx_data[0]==1)
			    {
				    cout<<" leaf Function: insert_here Value:"<<xkey<<endl;
                            }
		         }	
                                       //   cout<<"_________________aaaaaaaaa11111111_____________"<<endl;
			//check xnode is empty or not
			if (xnode.key[1] == -1)
			{
                           //   cout<<"___________No more___________"<<endl;
                                newmx.mx_Node2Mixblock(tempnode);
			        ind_putblock(newmx,mxb[xi].mx_locinfile,this->head.indh_name);
				mxb.clear();
                                     //     cout<<"____________________gddddd11111222222222__________"<<endl;
				this->head.header_mom2disk(this->head.indh_name);
                                      //    cout<<"____________________aaaaaaaaa22222222222_____________"<<endl;
				return 1;
			}
			else
			{//-- Node: 8 Type: internal Function: split
				if (this->ind_debug==1)
				{
					cout<<"--Node: "<<mxb[xi].mx_locinfile<<" Type: ";
			                if (mxb[xi].mx_data[0]==2)
			                 {
			    	                 cout<<" internal Function: split"<<endl;
			                 }
			                 if (mxb[xi].mx_data[0]==1)
			                 {
				                cout<<" leaf Function: split"<<endl;
                                         }

				}
				mxb.pop_back();
				xkey=xnode.rekey;
				xnode.rekey=-1;
				this->head.indh_nodetotal++;
				xptr=this->head.indh_nodetotal;

                                Mixblock sck;
                                sck.mx_Node2Mixblock(xnode); 
                                newmx.mx_Node2Mixblock(tempnode);
                                if(newmx.mx_data[0]==1)
                                {
                                     sck.mx_data[9]=newmx.mx_data[9]; 
                                     newmx.mx_data[9]=xptr;
                                }
                                xnode=sck.mx_Mixblock2Node();

			        ind_putblock(newmx,mxb[xi].mx_locinfile,this->head.indh_name);
				newmx.mx_Node2Mixblock(xnode);
			        ind_putblock(newmx,xptr,this->head.indh_name);
                                       //   cout<<"____________________gddddd11111111_____________"<<endl;
                                for( int hn=3; hn>=0;hn--)
                                {
                                    if(xnode.key[hn]!=-1)
                                    {
                                           
				            if (this->ind_debug==1)
	                                     {
		                                    cout<<"--Node: "<<xptr<<" Type: ";
			                            if (xnode.type==2)
			                              {
			    	                           cout<<" internal Function: insert_here Value:"<<xnode.key[hn]<<endl;
			                               }
			                            if (xnode.type==1)
			                            {
				                       cout<<" leaf Function: insert_here Value:"<<xnode.key[hn]<<endl;
                                                    }
                                                    if(xnode.type==2&& hn==0)
                                                    {
		                                            cout<<"--Node: "<<xptr<<" Type: ";
			                                     if (xnode.type==2)
			                                    {
			    	                                   cout<<" internal Function: insert_here Value:"<<xkey<<endl;
			                                    }
			                                    if (xnode.type==1)
			                                    {
				                                     cout<<" leaf Function: insert_here Value:"<<xkey<<endl;
                                                            }
                                                    }
			                      }	
                                     }//if(xnode.key[hn]!=-1)
                                }//for
                               
                          }
		    }
		if (xi <0)
		{
                 // cout<<"_________________________Newroot______________________"<<endl;

			Node newroot;
			newroot.type=2;
			newroot.ptr[0]=this->head.indh_root_loc;
			newroot.key[0]=xkey;
			newroot.ptr[1]=xptr;
            
			this->head.indh_nodetotal++;
			this->head.indh_root_loc=this->head.indh_nodetotal;
                      //  cout<<"this->head.indh_root_loc:"<<this->head.indh_root_loc<<endl;
                        newmx.mx_Node2Mixblock(newroot);
			ind_putblock(newmx,this->head.indh_root_loc,this->head.indh_name);
		      /*             Mixblock mb8=ind_getblock(this->head.indh_root_loc, this->head.indh_name);
                         cout<<"__xxx__this->head.indh_root_loc_"<<this->head.indh_root_loc<<endl;
                         for(int hh=0;hh<11; hh++) cout<<"mb8.mx_data["<<hh<<"]____"<<mb8.mx_data[hh]<<endl;         
                       */                   
 
                this->head.header_mom2disk(this->head.indh_name);

				if (this->ind_debug==1)
	                         {
		                         cout<<"--Node: "<<newroot.ptr[0]<<" Type: ";
			                if (newroot.type==2)
			                 {
			    	                 cout<<" internal Function: insert_here Value:"<<key<<endl;
			                  }
			                if (newroot.type==1)
			                 {
				        cout<<" leaf Function: insert_here Value:"<<key<<endl;
                                            }
			          }
                            Node leftnode=newroot;
                            Mixblock pb;

	                    while (leftnode.type !=1)
		            {
			         pb=this->ind_getblock(leftnode.ptr[0],this->head.indh_name);
			         leftnode=pb.mx_Mixblock2Node();
                            }
                                 // cout<<space<<tnode2.key[0]<<"->"<<tnode.ptr[0]<<endl;
				if (this->ind_debug==1)
	                         {
		                         cout<<"--Node: "<<this->head.indh_root_loc<<" Type: ";
			                if (newroot.type==2)
			                 {
			    	                 cout<<" internal Function: insert_here Value:"<<leftnode.key[0]<<endl;
			                  }
			                if (newroot.type==1)
			                 {
				                 cout<<" leaf Function: insert_here Value:"<<leftnode.key[0]<<endl;
                                            }
			          }
				if (this->ind_debug==1)
	                         {
		                         cout<<"--Node: "<<this->head.indh_root_loc<<" Type: ";
			                if (newroot.type==2)
			                 {
			    	                 cout<<" internal Function: insert_here Value:"<<xkey<<endl;
			                  }
			                if (newroot.type==1)
			                 {
				        cout<<" leaf Function: insert_here Value:"<<xkey<<endl;
                                            }
			          }
			return 1;
		}

	}
	if (this->head.indh_check_dup==1)
	{  //Duplicate is allowed under this situation
                //  cout<<"___________dup=1__ __11111111111_key: "<<key<<"_____value: "<<value<<endl;
		int k,bucketloc,i,lastb,orinext;
		Bucket newbuc;
		tempnode=this->mxb[this->mxb.size()-1].mx_Mixblock2Node();
              /*   if(key==193) {        cout<<"______________tempnode.type: "<<tempnode.type<<endl;
                                       for(int gi=0;gi<4;gi++)  cout<<"tempnode.key["<<gi<<"]: "<<tempnode.key[gi]<<endl;}      */
		for (k=0; k<4; k++)
		{
			if (key==tempnode.key[k])
			{     
				bucketloc=tempnode.ptr[k];
				break;
			}
		}
                    //  cout<<"__fsss____k: "<<k<<endl;
		if (k<4)
		{	                       //    if(key==193) cout<<"k < 4__key: "<<key<<" block:__"<<value<<endl;
			tempmx=this->ind_getblock(bucketloc, this->head.indh_name);
                        orinext=tempmx.mx_locinfile;    // cout<<"orinext________:"<<orinext<<endl;
		        tempb=tempmx.mx_Mixblock2Bucket();
			int lastb=bucketloc;
                           //   for(int gi=0;gi<12;gi++)  cout<<"tempmx.mx_data["<<gi<<"]: "<<tempmx.mx_data[gi]<<endl; 
                           //   cout<<"_________00_____tempb.nextb: "<<tempb.nextb<<endl;
                while (tempb.nextb!=-1)
                {
                         orinext=lastb;
                         lastb=tempb.nextb;       // cout<<"______orinext______: "<<orinext<<endl;
			bucketloc=tempb.nextb;
                           //  if(key==1) cout<<"___bucketloc____: "<<bucketloc<<endl;
			tempmx=this->ind_getblock(bucketloc, this->head.indh_name);
		        tempb=tempmx.mx_Mixblock2Bucket();
                      /*  if (tempb.nextb!=-1)

                       {
			   lastb=tempb.nextb;
                       }  */

                }
                       //   cout<<"lastb______: "<<lastb<<endl;
                int jno=0;
                while(tempb.bptr[jno]!=-1&&jno<9)
                {             //cout<<"____________judgedup___1111111_____"<<endl;
                      if(tempb.bptr[jno]==value)
                      {
                             	 if (this->ind_debug==1)
	                         {
//-- Node: 3 Type: pointer bucket Function: Pointer already present, no need to insert duplicate
			              cout<<"--Node: "<<lastb;
                                      cout<<" Type: pointer bucket Function: insert Value: "<<value<<endl;
		                         cout<<"--Node: "<<tempmx.mx_locinfile;
                                     cout<<" Type: pointer bucket Function: Pointer already present, no need to insert duplicate"<<endl;

			          }
                       mxb.clear();
                       return 3;
                       }
                       jno++;

                 }
                                           //   if(key==193) cout<<"__key__: "<<key<<" blockin indexfile__: "<<tempmx.mx_locinfile<<endl;


                newbuc=tempb.bu_insert(key, value);

			if (newbuc.bptr[0] == -1)
			{	//cout<<"lastb__22222222_____: "<<lastb<<endl;
					this->tempmx.mx_Bucket2Mixblock(tempb);
					this->ind_putblock(tempmx, lastb, this->head.indh_name);
					if (this->ind_debug==1)
	                {//-- Node: 5 Type: pointer bucket Function: insert Value: 0
			              cout<<"--Node: "<<lastb<<" Type: pointer bucket Function: insert Value: "<<value<<endl;	
		            }
                            this->head.header_mom2disk(this->head.indh_name);
                              //   if(key==193) cout<<"__key: "<<key<<" block:__"<<value<<endl;
                            mxb.clear();
                            return 1;
                    
			}
			if (newbuc.bptr[0] != -1)
			{
                                orinext=lastb;

				this->head.indh_nodetotal++;
				lastb=this->head.indh_nodetotal;
                                                     //  cout<<"_________if(!=-1)case__________"<<endl;
                                      //   cout<<"_____this->head.indh_nodetotal___: "<<this->head.indh_nodetotal<<endl;
				this->tempb.nextb=lastb; 
				this->tempmx.mx_Bucket2Mixblock(tempb);
                                                      // cout<<"______tempb______leastb____: "<<lastb<<endl;
                                                  //       for(int qq=0;qq<12;qq++)      cout<<" _____tempb["<<qq<<"]"<<tempmx.mx_data[qq]<<endl;
                                      //   cout<<"___orinext__: "<<orinext<<endl;
				this->ind_putblock(tempmx, orinext, this->head.indh_name);
				this->tempmx.mx_Bucket2Mixblock(newbuc);
                                           //    cout<<"_________newbuc_____lastb:_____"<<lastb<<endl;
                                          //       for(int qq=0;qq<12;qq++)      cout<<" _____newbuc["<<qq<<"]"<<tempmx.mx_data[qq]<<endl;
				this->ind_putblock(tempmx, lastb, this->head.indh_name);
				if (this->ind_debug==1)
	                         {//-- Node: 5 Type: pointer bucket Function: insert Value: 0
			              cout<<"-- Node: "<<lastb<<" Type: pointer bucket Function: insert Value: "<<value<<endl;	
		                }
                    this->head.header_mom2disk(this->head.indh_name);
                                           
                 /*   if(key==193)
                    { 
                                       cout<<"create new bucket***************"<<
                                       cout<<"__key: "<<key<<" block:__"<<value<<" bucket__: "<<lastb<<endl;
                    }   */
                    mxb.clear();
                    return 1; 
			}

		}
		if (k==4)
		{// this leaf node doesn't contain this key
                    //  cout<<"___________k==4____________"<<endl;
                    //  cout<<"___awkey______: "<<key<<"____awvalue______: "<<value<<endl;

                     //    if(key==193) cout<<"k = 4__key: "<<key<<" block:__"<<value<<endl;
		newb.bu_insert(key, value);
                //for(int rr=0;rr<9;rr++) { cout<<"newb____"<<rr<<":"<<newb.bptr[rr]<<"   ";}
		this->head.indh_nodetotal++;
		buckey=this->head.indh_nodetotal;
                  //   cout<<"___buckey_______: "<<buckey<<"  _____this->head.indh_nodetotal_____: "<<this->head.indh_nodetotal<<endl;
		newmx.mx_Bucket2Mixblock(newb);
		this->ind_putblock(newmx,buckey,this->head.indh_name);
//cout<<"___________In333333333___________"<<endl;
		if (this->ind_debug==1)
	    {//-- Node: 5 Type: pointer bucket Function: insert Value: 0
			cout<<"--Node: "<<buckey<<" Type:  pointer bucket Function: insert Value: "<<value<<endl;	
		}
		if (this->ind_debug==1)
	    {
		    for (int h=0; h<mxb.size(); h++)
	        {
		        cout<<"--Node: "<<mxb[h].mx_locinfile<<" Type: ";
			    if (mxb[h].mx_data[0]==2)
			    {
			    	cout<<" internal Function: insert Value:"<<key<<endl;
			    }
			    if (mxb[h].mx_data[0]==1)
			    {
				    cout<<" leaf Function: insert Value:"<<key<<endl;
                }
			
	        }
	    } 	
                 //   if(huhu==1)  cout<<"_________222222_______key________: "<<key<<endl;
		xkey=key;
		xptr=buckey;
                leafs=mxb[this->mxb.size()-1].mx_locinfile;
		for (xi=(this->mxb.size()-1); xi>=0; xi--)
		{
			tempnode=mxb[xi].mx_Mixblock2Node();
			Node xnode=tempnode.node_insert(xkey, xptr);


			if (this->ind_debug==1)
	                 {
                            if(xkey!=key)
                            {	
	                           cout<<"--Node: "<<leafs<<" Type: ";
			            if (mxb[xi].mx_data[0]==2)
			             {
			    	            cout<<" internal Function: insert_here Value:"<<key<<endl;
			                }
			             if (mxb[xi].mx_data[0]==1)
			             {
				              cout<<" leaf Function: insert_here Value:"<<key<<endl;
                                      }
                            }
		            cout<<"--Node: "<<mxb[xi].mx_locinfile<<" Type: ";
			    if (mxb[xi].mx_data[0]==2)
			    {
			    	cout<<" internal Function: insert_here Value:"<<xkey<<endl;
			    }
			    if (mxb[xi].mx_data[0]==1)
			    {
				    cout<<" leaf Function: insert_here Value:"<<xkey<<endl;
                            }
		         }	

			//check xnode is empty or not
			if (xnode.key[1] == -1)
			{
                           //   cout<<"___________No more___________"<<endl;
                                newmx.mx_Node2Mixblock(tempnode);
			        ind_putblock(newmx,mxb[xi].mx_locinfile,this->head.indh_name);
				mxb.clear();
				this->head.header_mom2disk(this->head.indh_name);
				return 1;
			}
			else
			{//-- Node: 8 Type: internal Function: split
				if (this->ind_debug==1)
				{
					cout<<"--Node: "<<mxb[xi].mx_locinfile<<" Type: ";
			                if (mxb[xi].mx_data[0]==2)
			                 {
			    	                 cout<<" internal Function: split"<<endl;
			                 }
			                 if (mxb[xi].mx_data[0]==1)
			                 {
				                cout<<" leaf Function: split"<<endl;
                                         }

				}
				mxb.pop_back();
				xkey=xnode.rekey;
				xnode.rekey=-1;
				this->head.indh_nodetotal++;
				xptr=this->head.indh_nodetotal;
                                Mixblock sck;
                                sck.mx_Node2Mixblock(xnode); 
                                newmx.mx_Node2Mixblock(tempnode);
                                if(newmx.mx_data[0]==1)
                                {
                                     sck.mx_data[9]=newmx.mx_data[9]; 
                                     newmx.mx_data[9]=xptr;
                                }
                                xnode=sck.mx_Mixblock2Node();
			        ind_putblock(newmx,mxb[xi].mx_locinfile,this->head.indh_name);
				newmx.mx_Node2Mixblock(xnode);
			        ind_putblock(newmx,xptr,this->head.indh_name);
                                for( int hn=3; hn>=0;hn--)
                                {
                                    if(xnode.key[hn]!=-1)
                                    {
                                           
				            if (this->ind_debug==1)
	                                     {
		                                    cout<<"--Node: "<<xptr<<" Type: ";
			                            if (xnode.type==2)
			                              {
			    	                           cout<<" internal Function: insert_here Value:"<<xnode.key[hn]<<endl;
			                               }
			                            if (xnode.type==1)
			                            {
				                       cout<<" leaf Function: insert_here Value:"<<xnode.key[hn]<<endl;
                                                    }
                                                    if(xnode.type==2&& hn==0)
                                                    {
		                                            cout<<"--Node: "<<xptr<<" Type: ";
			                                     if (xnode.type==2)
			                                    {
			    	                                   cout<<" internal Function: insert_here Value:"<<xkey<<endl;
			                                    }
			                                    if (xnode.type==1)
			                                    {
				                                     cout<<" leaf Function: insert_here Value:"<<xkey<<endl;
                                                            }
                                                    }
			                      }	
                                     }
                                }
                               
                          }
		    }
                   // if(huhu==1)  cout<<"_________333333333_______key________: "<<key<<endl;
		if (xi <0)
		{
                  //cout<<"_________________________Newroot______________________"<<endl;

			Node newroot;
			newroot.type=2;
			newroot.ptr[0]=this->head.indh_root_loc;
			newroot.key[0]=xkey;
			newroot.ptr[1]=xptr;
                                //    cout<<"____11111_____xkey___: "<<xkey<<endl;
			this->head.indh_nodetotal++;
			this->head.indh_root_loc=this->head.indh_nodetotal;
                           //  cout<<"key: "<<key<<endl;
                      if(huhu==94)  cout<<"this->head.indh_root_loc:"<<this->head.indh_root_loc<<endl;
                        newmx.mx_Node2Mixblock(newroot);
			ind_putblock(newmx,this->head.indh_root_loc,this->head.indh_name);
		      /*             Mixblock mb8=ind_getblock(this->head.indh_root_loc, this->head.indh_name);
                         cout<<"__xxx__this->head.indh_root_loc_"<<this->head.indh_root_loc<<endl;
                         for(int hh=0;hh<11; hh++) cout<<"mb8.mx_data["<<hh<<"]____"<<mb8.mx_data[hh]<<endl;         
                       */                   
 
                this->head.header_mom2disk(this->head.indh_name);

				if (this->ind_debug==1)
	                         {
		                         cout<<"--Node: "<<newroot.ptr[0]<<" Type: ";
			                if (newroot.type==2)
			                 {
			    	                 cout<<" internal Function: insert_here Value:"<<key<<endl;
			                  }
			                if (newroot.type==1)
			                 {
				        cout<<" leaf Function: insert_here Value:"<<key<<endl;
                                            }
			          }
                            Node leftnode=newroot;
                            Mixblock pb;
   //cout<<"____22222_____xkey___: "<<xkey<<endl;
	                    while (leftnode.type !=1)
		            {
			         pb=this->ind_getblock(leftnode.ptr[0],this->head.indh_name);
			         leftnode=pb.mx_Mixblock2Node();
                            }

                                 // cout<<space<<tnode2.key[0]<<"->"<<tnode.ptr[0]<<endl;
				if (this->ind_debug==1)
	                         {
		                         cout<<"--Node: "<<this->head.indh_root_loc<<" Type: ";
			                if (newroot.type==2)
			                 {
			    	                 cout<<" internal Function: insert_here Value:"<<leftnode.key[0]<<endl;
			                  }
			                if (newroot.type==1)
			                 {
				                 cout<<" leaf Function: insert_here Value:"<<leftnode.key[0]<<endl;
                                            }
			          }
   //cout<<"____33333_____xkey___: "<<xkey<<endl;
				if (this->ind_debug==1)
	                         {
		                         cout<<"--Node: "<<this->head.indh_root_loc<<" Type: ";

			                if (newroot.type==2)
			                 {
			    	                 cout<<" internal Function: insert_here Value:"<<xkey<<endl;
			                  }
			                if (newroot.type==1)
			                 {
				        cout<<" leaf Function: insert_here Value:"<<xkey<<endl;
                                            }
			          }
                        mxb.clear();
			return 1;

		    }

	    }
	}
}

void Index:: ind_printnode(int offset,string space)
{
	Mixblock pb=this->ind_getblock(offset, this->head.indh_name);
	Node tnode=pb.mx_Mixblock2Node();
        int gt=tnode.type;
              /*     cout<<"__________block4_______"<<endl;
                                Mixblock tk=this->ind_getblock(4, this->head.indh_name);
                                     for (int g=0;g<12;g++)
                              cout<<"___mk["<<g<<"]:  "<<tk.mx_data[g]<<endl;  */
              /*      for (int g=0;g<12;g++)
                              cout<<"___pb["<<g<<"]:  "<<pb.mx_data[g]<<endl;         */
       int ts,ti,tj;
                    //    cout<<"________________p1_____________"<<endl;
	if (tnode.type==1)
	{
		for (ts=0; ts<4; ts++)
		{       // cout<<"____ts______:"<<ts<<"_____tnode.key["<<ts<<"]: "<<tnode.key[ts]<<endl;
                        if (tnode.key[ts]==-1)
                        {
                            break;
                        }
			cout<<space<<tnode.key[ts]<<"->(";
			pb=this->ind_getblock(tnode.ptr[ts],this->head.indh_name);
			Bucket tbuc=pb.mx_Mixblock2Bucket();
                     //    cout<<"tnode.ptr["<<ts<<"]:_____"<<tnode.ptr[ts]<<endl;
                       /*   for (int g=0;g<12;g++)
                              cout<<tnode.ptr[ts]<<"___pb["<<g<<"]:  "<<pb.mx_data[g]<<endl;  */
                        while(1)
	                {     
		             for(ti=0; ti<9; ti++)
		             {
			        if (tbuc.bptr[ti]==-1)
			        {
				        break;
			        }
			        if (ti!=0)
			        {
				         cout<<",";
			        }
					cout<<tbuc.bptr[ti];
			     }

		             if (tbuc.nextb==-1)
		             {
			          break;
	        	     } 

                             pb=this->ind_getblock(tbuc.nextb,this->head.indh_name);
		             tbuc=pb.mx_Mixblock2Bucket();
		             cout<<",";
	                 }
	                 cout<<")"<<endl;

	         }
        }
	else
	{
		//If current node is internal type
                      //  cout<<"________________p2_____________"<<endl;
        Node tnode2=tnode;
	    while (tnode2.type !=1)
		{
			pb=this->ind_getblock(tnode2.ptr[0],this->head.indh_name);
			tnode2=pb.mx_Mixblock2Node();
                }
                    //    cout<<"________________p3_____________"<<endl;
                        cout<<space<<tnode2.key[0]<<"->"<<tnode.ptr[0]<<endl;

            this->ind_printnode(tnode.ptr[0],space+" ");

		for(tj=0; tnode.key[tj]!=-1 && tj<4; tj++)
		{               //   cout<<"____________________7_________"<<endl;
	    	        cout<<space<<tnode.key[tj]<<"->"<<tnode.ptr[tj+1]<<endl;
			this->ind_printnode(tnode.ptr[tj+1], space+" ");
		}
                 // cout<<"____________________8_________"<<endl;
	}
}

void Index:: ind_print(char* indname)
{


	this->head.header_disk2mom(indname);
        int key,ptr,type,nextno,maxcount,i,num=0,intp=1;
	cout<<"header->"<<this->head.indh_root_loc<<endl;
	string space=" ";
        //cout<<"this->head.indh_root_loc"<<this->head.indh_root_loc<<endl;
	this->ind_printnode(this->head.indh_root_loc,space);
}
    
void Index:: ind_q_print(char* indexname, char* tablename)
{
  /* cout<<"___________________qprint______:"<<endl;
           Mixblock mk=this->ind_getblock(2, indexname);
           for (int g=0;g<12;g++)
             cout<<"___mk["<<g<<"]:  "<<mk.mx_data[g]<<endl;
 */
	head.header_disk2mom(indexname);
	if (strcmp(this->head.indh_tablename,tablename)==0)
	{
		ind_print(indexname);
	}
	else
       {
		cout<<"This index is worng"<<endl;
	}
}
			

void Index:: ind_create(char* indname, char* tablename, char* attributename,int checkd, Catalog* cata,Buffer* buff)
{       this->catalog=cata;    
        this->buffer=buff;
                         //cout<<"______________100000000_________________"<<endl;
	int checkunique;
	int checkindunique;
	//First Step: Checking whether this table has got an index. If so, this function should go out. 


/*	checkindunique=ind_index_unique(indname);
	if (checkindunique == -1)
	{
                  cout<<"This index name is used already".<<endl;
        return; //This name of index has been used
	}          */
      //   cout<<"_______________ic11111111111111____________:  "<<endl;

    //Second step: Make sure the table and attribute exist
    ind_ta_att_exist(tablename, attributename);

	if (this->head.indh_tableno == -1)
	{
		cout<<"This table doesn't exist."<<endl;
		return; //This table doesn't exist
	}

	if (this->head.indh_attributeno == -1)
	{
		cout<<"This attribute of this table doesn't exist."<<endl;
		return; //This attribute doesn't exist
	}

    //Third step: check whether this table has got two indeces.
    int checkind;
	checkind=ind_check_indnum(this->head.indh_tableno, this->head.indh_attributeno);

        if(checkind == -2)
        {
             cout<<"-- Error: twoindexes: Cannot define two indices on the same column."<<endl;
             return;
        }
	if(checkind == -3)
	{
		cout<<"This table has got full indices."<<endl;
		return ;
    }

    else
	{
		strcpy(this->catalog->ta[this->head.indh_tableno].at_index_name[checkind],indname);
	}
	//Iniatialize the header indexinfo
       this->catalog->ta[this->head.indh_tableno].ta_status=-1;
    strcpy(this->head.indh_name,indname);
    strcpy(this->head.indh_tablename,tablename);
    strcpy(this->head.indh_attributename,attributename);

	this->head.indh_check_dup=checkd; //Mark whether this index allows duplicate

    this->head.indh_root_loc=1;
    this->head.indh_nodetotal=1;

    ind_create_indisk(indname);
	this->head.header_mom2disk(indname);
	Mixblock mb;
	mb.mx_data[0]=1;
	ind_putblock(mb,1,indname);
                       //      cout<<"____________cre__1111111______"<<endl;
	//put rows to the index
        // cout<<"this->catalog->ta[this->head.indh_tableno].num_block____:"<<this->catalog->ta[this->head.indh_tableno].num_block<<endl;
 /* cout<<"__catalog->ta[this->head.indh_tableno].at_index[0]: "<<catalog->ta[this->head.indh_tableno].at_index[0]<<endl;
      cout<<" __ta[this->head.indh_tableno].at_index_name[0]: "<<catalog->ta[this->head.indh_tableno].at_index_name[0]<<endl;
  cout<<"__catalog->ta[this->head.indh_tableno].at_index[1]: "<<catalog->ta[this->head.indh_tableno].at_index[1]<<endl;
      cout<<" __ta[this->head.indh_tableno].at_index_name[1]: "<<catalog->ta[this->head.indh_tableno].at_index_name[1]<<endl;
 */
    if (this->catalog->ta[this->head.indh_tableno].num_block>0)
    {//this->head->indh_attributeno
		int id_in_blo=12/this->catalog->ta[this->head.indh_tableno].id_len;
	    int ci,ck;
 //cout<<"________ffbeginggg_____"<<endl;


	    for(ci=0; ci<this->catalog->ta[this->head.indh_tableno].num_block; ci++)                                             

		{        
                        Block* temptable = this->buffer->buf_get_blo(tablename, ci);             //ind_getblock(ci,this->head->indh_tablename);
                                                 //    cout<<"____________cre__1112222222___ci___: "<<ci<<endl; 
                     //   for(int dj=0;dj<12;dj++) cout<<"temptable->bl_data[dj]:___ "<<temptable->bl_data[dj]<<endl;         
		   for (ck=0; ck<id_in_blo; ck++)
		   {    
			   int movep=ck*this->catalog->ta[this->head.indh_tableno].id_len;
                          // cout<<"____________cre__111333333333_____"<<endl; 
            
			   if(temptable->bl_data[movep]==0&&temptable->bl_data[movep+1]==0)
			   {               // cout<<"____________cre__222222222______"<<endl;
				             //cout<<"Already existed data has been copied to the index."<<endl;
				   return ;
			   }
                                          //     cout<<"________________ffssssssssssssss____________"<<endl;

                                  /*   if(temptable->bl_data[movep+this->head.indh_attributeno]==94) 
                                               cout<<"___key___: "<<temptable->bl_data[movep+this->head.indh_attributeno]<<endl;  */
    
			   int yresult=ind_insert(temptable->bl_data[movep+this->head.indh_attributeno], ci);
                               this->mxb.clear();
                                                   //     cout<<"____________cre__222222220000000000_____"<<endl;   
                               if(yresult==-1) return;
		   }  
                                 //  cout<<"____________cre__22222222333333_____"<<endl; 
		   this->buffer->buf_put_blo(temptable);
                                                 //     cout<<"____________cre__2222222222____4444444____"<<endl;   
		}
            //  cout<<"____________cre__333333___end______"<<endl;   
    }

}


int Index:: ind_q_insert(char* tablename, char* indexname, int key,int ptr, Catalog* cata)
{
	                 //this->head.indh_tableno=this->catalog->cata_get_table(tablename);
                       // this->head.indh_attributeno=this->catalog->cata_get_attribute(tablename,attributename);
        this->catalog=cata;  
        int insert_result;
	/*ind_ta_att_exist(tablename, attributename);

	char indexname[5];
	if (strcmp(this->catalog->ta[this->head.indh_tableno].cata_table_name,tablename)==0&&
		strcmp(this->catalog->ta[this->head.indh_tableno].id_content[this->head.indh_attributeno],attributename)==0 )
	{
		for (int h=0; h<2; h++)
		{
                     if (this->catalog->ta[this->head.indh_tableno].at_index[h]==this->head.indh_attributeno)
                     {
                          strcpy(indexname,this->catalog->ta[this->head.indh_tableno].at_index_name[h]);
                      }
		}
			
	}
          */
	head.header_disk2mom(indexname);

	insert_result=this->ind_insert(key, ptr);

	if(insert_result==-1) cout<<" Insert fault"<<endl;
	return insert_result;
}


    
vector<int> Index::ind_select_equal(int tablenumber, int attnumber, int key)
{                // cout<<"___________enter_equal****************____________"<<endl;
                      //  cout<<"________sel equal:__1step____"<<endl;
    int insert_result;                          
	char indexname[5];
        /*         cout<<"___________attnumber_________:"<<attnumber<<endl;
                 cout<<"___this->catalog->ta[tablenumber].at_index[0]___: "<<this->catalog->ta[tablenumber].at_index[0]<<endl;
                 cout<<"___this->catalog->ta[tablenumber].at_index[1]____: "<<this->catalog->ta[tablenumber].at_index[1]<<endl;
               cout<<"this->catalog->ta[tablenumber].at_index_name[0]:__ "<<this->catalog->ta[tablenumber].at_index_name[0]<<endl;
                 cout<<"this->catalog->ta[tablenumber].at_index_name[1]___: "<<this->catalog->ta[tablenumber].at_index_name[1]<<endl;
          */
    /*    if(this->catalog->ta[tablenumber].at_index[0]==attnumber)   strcpy(indexname,this->catalog->ta[tablenumber].at_index_name[0]);
        if(this->catalog->ta[tablenumber].at_index[1]==attnumber)   strcpy(indexname,this->catalog->ta[tablenumber].at_index_name[1]);
	*/
    for(int ju=0;ju<INDEXT;ju++)
	{
         if(this->catalog->ta[tablenumber].at_index[ju]==attnumber)   
			 strcpy(indexname,this->catalog->ta[tablenumber].at_index_name[ju]);
	}
	head.header_disk2mom(indexname);

	vector<int> pis;
	int tob;
        int kjj=0;
	pis=ind_lookup(key);
               //   cout<<"___________pis0____________"<<pis[0]<<endl;
	//Got from method Function
	if (pis.size()>0)
	{
	    int leafn=mxb.size()-1;
               //     for (int nc=(this->mxb.size()-1); nc>=0; nc--) cout<<"_mxb["<<nc<<"]: "<<mxb[leafn].mx_data[nc]<<endl;
	    for (int jj=1; jj<5; jj++)
	    {                 //   cout<<"mxb[leafn].mx_data[jj]__________: "<<mxb[leafn].mx_data[jj]<<endl;
			if(mxb[leafn].mx_data[jj]==key)
			{
                               
				tob=mxb[leafn].mx_data[jj+4];
                                break;
			}
 
	    }
	}   
        	if (pis.size()==0)       kjj=-1;  
    if (this->ind_debug==1)
	{
		for (int h=0; h<mxb.size(); h++)
	        {
		    cout<<"--Node: "<<mxb[h].mx_locinfile<<" Type: ";
			if (mxb[h].mx_data[0]==2)
			{
				cout<<" internal Function: find Value:"<<key<<endl;
			}
			if (mxb[h].mx_data[0]==1)
			{
				cout<<" leaf Function: find Value:"<<key<<endl;
                        }
			
	        }
            if(kjj!=-1)  cout<<"--Node: "<<tob<<" Type: pointer bucket Function: traverse "<<endl;

	}
           this->mxb.clear();  // cout<<"_________pis.size(): "<<pis.size()<<endl;
	return pis;  
}

vector<int> Index::ind_select_greater(int tablenumber, int attnumber, int key)
{
   	//ind_ta_att_exist(tablename, attributename);
        int insert_result;
	char indexname[5];
	int tob;
        int kjj=0;              //    cout<<"________sel equal:______"<<endl;
   /*     if(this->catalog->ta[tablenumber].at_index[0]==attnumber)   strcpy(indexname,this->catalog->ta[tablenumber].at_index_name[0]);
        if(this->catalog->ta[tablenumber].at_index[1]==attnumber)   strcpy(indexname,this->catalog->ta[tablenumber].at_index_name[1]);
   */
    for(int ju=0;ju<INDEXT;ju++)
	{
         if(this->catalog->ta[tablenumber].at_index[ju]==attnumber)   
			 strcpy(indexname,this->catalog->ta[tablenumber].at_index_name[ju]);
	}

	this->head.header_disk2mom(indexname);

	vector<int> pis;
	vector<int> conc;
	int gv,offset,of;
	Mixblock forb;
 	pis=ind_lookup(key);   
        Mixblock xhblock;
	int leafn=mxb.size()-1;
	xhblock=mxb[leafn];
           
        do
        {            //cout<<"________do___while_________"<<endl;
                                //  for(int huu=0; huu<12; huu++)  cout<<"__leafn__"<<leafn<<"___xhblock["<<huu<<"]____: "<<xhblock.mx_data[huu]<<endl;
	     for (int jj=1; jj<5; jj++)
	     {
                             //cout<<"______________for__loop_____"<<endl;
	         if(xhblock.mx_data[jj]>key)
	         {
	              of=xhblock.mx_data[jj+4];
                      forb=ind_getblock(of, this->head.indh_name);
			do
			{                 
			    for ( int ii=2; ii<11; ii++)
                            {
			        if (forb.mx_data[ii]!=-1)
				{
					gv=forb.mx_data[ii];
					conc.push_back(gv);    //  cout<<"conc.size():_____ "<<conc.size()<<endl;        

				}
				if (forb.mx_data[ii]==-1)
				{
					break;
				}
							
                            }
		            if (forb.mx_data[11]!=-1)
			    {
				   of=forb.mx_data[11];
				   forb=forb=ind_getblock(of, this->head.indh_name);
			    }
			}
			while (forb.mx_data[11]!=-1);
		  }
		  if(xhblock.mx_data[jj+1]==-1)
		  {
		      break;
		  }

	     }
	     leafn=xhblock.mx_data[9];
	     if(xhblock.mx_data[9]!=-1) xhblock=ind_getblock(leafn,this->head.indh_name);
           
    }
    while (leafn!=-1);
    if (pis.size()==0)       kjj=-1;    //cout<<"kjj:_____"<<kjj<<endl;
    /*if (this->ind_debug==1)
	{
		for (int h=0; h<mxb.size(); h++)
	        {
		    cout<<"--Node: "<<mxb[h].mx_locinfile<<" Type: ";
			if (mxb[h].mx_data[0]==2)
			{
				cout<<" internal Function: find Value:"<<key<<endl;
			}
			if (mxb[h].mx_data[0]==1)
			{
				cout<<" leaf Function: find Value:"<<key<<endl;
                        }
			
	        }
            if(kjj!=-1)  cout<<"--Node: "<<tob<<" Type: pointer bucket Function: traverse "<<endl;

	}  */
           this->mxb.clear();
    return conc;
}

vector<int> Index::ind_select_smaller(int tablenumber, int attnumber, int key)
{
   	//ind_ta_att_exist(tablename, attributename);
        int insert_result;
	char indexname[5];
	int tob;
        int kjj=0;
   /*    if(this->catalog->ta[tablenumber].at_index[0]==attnumber)   strcpy(indexname,this->catalog->ta[tablenumber].at_index_name[0]);
        if(this->catalog->ta[tablenumber].at_index[1]==attnumber)   strcpy(indexname,this->catalog->ta[tablenumber].at_index_name[1]);
   */
       for(int ju=0;ju<INDEXT;ju++)
	{
         if(this->catalog->ta[tablenumber].at_index[ju]==attnumber)   
			 strcpy(indexname,this->catalog->ta[tablenumber].at_index_name[ju]);
	}
	this->head.header_disk2mom(indexname);

	vector<int> pis;
	vector<int> conc;
	int gv,offset,of;
 	pis=ind_lookup(key);   
        int nbp;
        int goout=-1;
	//int leafn=mxb.size()-1;
	Mixblock xhblock=ind_getblock(this->head.indh_root_loc,this->head.indh_name);
    while (xhblock.mx_data[0]!=1)
    {
		of=xhblock.mx_data[5];
		xhblock=ind_getblock(of,this->head.indh_name);
    }
             //  cout<<"___xhblock___after___!=1___:"<<xhblock.mx_locinfile<<endl;
	/*of=xhblock.mx_data[5];          cout<<"_____of:_____ "<<of<<endl;
	xhblock=ind_getblock(of,this->head.indh_name);  */
          //  for(int huu=0; huu<12; huu++)   cout<<"_____xhblock["<<huu<<"]____: "<<xhblock.mx_data[huu]<<endl;
     do
    {               //   cout<<"______________do___while___"<<endl;
	    for (int jj=1; jj<5; jj++)
	    {
                                    //    cout<<"______________for___loop___"<<endl;
	         if(xhblock.mx_data[jj]<key)
	         {
	              of=xhblock.mx_data[jj+4];
                      Mixblock forb=ind_getblock(of, this->head.indh_name);  
                        //  for(int huu=0; huu<12; huu++)   cout<<"_____forb["<<huu<<"]____: "<<forb.mx_data[huu]<<endl;
				  do
				  {                 
					   for ( int ii=2; ii<11; ii++)
                                           {
					        if (forb.mx_data[ii]!=-1)
					        {
								gv=forb.mx_data[ii];
								conc.push_back(gv);           //   cout<<"conc.size():_____ "<<conc.size()<<endl;
                                                                                                                     
					        }
						if (forb.mx_data[ii]==-1)
						{
							break;
						}
							
                                           }
					   if (forb.mx_data[11]!=-1)
					   {
						   of=forb.mx_data[11];
						   forb=forb=ind_getblock(of, this->head.indh_name);
					   }
				  }
				  while (forb.mx_data[11]!=-1);
		     }
                     if(xhblock.mx_data[jj]>=key)
                     {
                         goout=1;
                         break;
                     }         
		     if(xhblock.mx_data[jj+1]==-1)
		     {    
                         break;
		     }

	     }
             if(goout==1) break;
             nbp=xhblock.mx_data[9];
             if(xhblock.mx_data[9]!=-1)      
             {
                xhblock=ind_getblock(nbp,this->head.indh_name);   
                  //for(int huu=0; huu<12; huu++)   cout<<"_____xhblock["<<huu<<"]____: "<<xhblock.mx_data[huu]<<endl;  
             }
    }
    while (nbp!=-1);
    if (pis.size()==0)       kjj=-1;    //cout<<"kjj:_____"<<kjj<<endl;
    /* if (this->ind_debug==1)
	{
		for (int h=0; h<mxb.size(); h++)
	        {
		    cout<<"--Node: "<<mxb[h].mx_locinfile<<" Type: ";
			if (mxb[h].mx_data[0]==2)
			{
				cout<<" internal Function: find Value:"<<key<<endl;
			}
			if (mxb[h].mx_data[0]==1)
			{
				cout<<" leaf Function: find Value:"<<key<<endl;
                        }
			
	        }
            if(kjj!=-1)  cout<<"--Node: "<<tob<<" Type: pointer bucket Function: traverse "<<endl;

	}  */
           this->mxb.clear();	
             return conc;		     
}

vector<int> Index::ind_select_greaterequal(int tablenumber, int attnumber, int key)
{
        int insert_result;
	char indexname[5];
	int tob;
        int kjj=0;             //    cout<<"________sel equal:______"<<endl;
    /*    if(this->catalog->ta[tablenumber].at_index[0]==attnumber)   strcpy(indexname,this->catalog->ta[tablenumber].at_index_name[0]);
        if(this->catalog->ta[tablenumber].at_index[1]==attnumber)   strcpy(indexname,this->catalog->ta[tablenumber].at_index_name[1]);
	*/
    for(int ju=0;ju<INDEXT;ju++)
	{
         if(this->catalog->ta[tablenumber].at_index[ju]==attnumber)   
			 strcpy(indexname,this->catalog->ta[tablenumber].at_index_name[ju]);
	}

	this->head.header_disk2mom(indexname);

	vector<int> pis;
	vector<int> conc;
	int gv,offset,of;
	Mixblock forb;
 	pis=ind_lookup(key);   
        Mixblock xhblock;
	int leafn=mxb.size()-1;
	xhblock=mxb[leafn];

    do
    {

	    for (int jj=1; jj<5; jj++)
	    {

	         if(xhblock.mx_data[jj]>=key)
	         {
	              of=xhblock.mx_data[jj+4];
                      forb=ind_getblock(of, this->head.indh_name);
		      do
		      {                 
			   for ( int ii=2; ii<11; ii++)
                           {
				if (forb.mx_data[ii]!=-1)
			        {
					gv=forb.mx_data[ii];
					conc.push_back(gv);

				 }
				if (forb.mx_data[ii]==-1)
				{
					break;
				}
							
                           }
			   if (forb.mx_data[11]!=-1)
			   {
				 of=forb.mx_data[11];
				 forb=forb=ind_getblock(of, this->head.indh_name);
			   }
		      }
		      while (forb.mx_data[11]!=-1);
		     }
		     if(xhblock.mx_data[jj+1]==-1)
		     {
			      break;
		     }

	     }
	     leafn=xhblock.mx_data[9];
	     if(xhblock.mx_data[9]!=-1) xhblock=ind_getblock(leafn,this->head.indh_name);
    }
    while (leafn!=-1);
    if (pis.size()==0)       kjj=-1;    //cout<<"kjj:_____"<<kjj<<endl;
    /* if (this->ind_debug==1)
	{
		for (int h=0; h<mxb.size(); h++)
	        {
		    cout<<"--Node: "<<mxb[h].mx_locinfile<<" Type: ";
			if (mxb[h].mx_data[0]==2)
			{
				cout<<" internal Function: find Value:"<<key<<endl;
			}
			if (mxb[h].mx_data[0]==1)
			{
				cout<<" leaf Function: find Value:"<<key<<endl;
                        }
			
	        }
            if(kjj!=-1)  cout<<"--Node: "<<tob<<" Type: pointer bucket Function: traverse "<<endl;

	}  */
           this->mxb.clear();
    return conc;
}
vector<int> Index::ind_select_smallerequal(int tablenumber, int attnumber, int key)
{
   	//ind_ta_att_exist(tablename, attributename);
        int insert_result;
	char indexname[5];
	int tob;
        int kjj=0;
    /*    if(this->catalog->ta[tablenumber].at_index[0]==attnumber)   strcpy(indexname,this->catalog->ta[tablenumber].at_index_name[0]);
        if(this->catalog->ta[tablenumber].at_index[1]==attnumber)   strcpy(indexname,this->catalog->ta[tablenumber].at_index_name[1]);
    */
	    for(int ju=0;ju<INDEXT;ju++)
	{
         if(this->catalog->ta[tablenumber].at_index[ju]==attnumber)   
			 strcpy(indexname,this->catalog->ta[tablenumber].at_index_name[ju]);
	}
	this->head.header_disk2mom(indexname);

	vector<int> pis;
	vector<int> conc;
	int gv,offset,of;
 	pis=ind_lookup(key);   
        int nbp;
        int goout=-1;
	//int leafn=mxb.size()-1;
	Mixblock xhblock=ind_getblock(this->head.indh_root_loc,this->head.indh_name);
    while (xhblock.mx_data[0]!=1)
    {
		of=xhblock.mx_data[5];
		xhblock=ind_getblock(of,this->head.indh_name);
    }
             //  cout<<"___xhblock___after___!=1___:"<<xhblock.mx_locinfile<<endl;
	/*of=xhblock.mx_data[5];          cout<<"_____of:_____ "<<of<<endl;
	xhblock=ind_getblock(of,this->head.indh_name);  */
            //for(int huu=0; huu<12; huu++)   cout<<"_____xhblock["<<huu<<"]____: "<<xhblock.mx_data[huu]<<endl;
     do
    {               //   cout<<"______________do___while___"<<endl;
	    for (int jj=1; jj<5; jj++)
	    {
                                    //    cout<<"______________for___loop___"<<endl;
	         if(xhblock.mx_data[jj]<=key)
	         {
	              of=xhblock.mx_data[jj+4];
                      Mixblock forb=ind_getblock(of, this->head.indh_name);  
                        //  for(int huu=0; huu<12; huu++)   cout<<"_____forb["<<huu<<"]____: "<<forb.mx_data[huu]<<endl;
				  do
				  {                 
					   for ( int ii=2; ii<11; ii++)
                                           {
					        if (forb.mx_data[ii]!=-1)
					        {
								gv=forb.mx_data[ii];
								conc.push_back(gv);           //   cout<<"conc.size():_____ "<<conc.size()<<endl;
                                                                                                                     
					        }
						if (forb.mx_data[ii]==-1)
						{
							break;
						}
							
                                           }
					   if (forb.mx_data[11]!=-1)
					   {
						   of=forb.mx_data[11];
						   forb=forb=ind_getblock(of, this->head.indh_name);
					   }
				  }
				  while (forb.mx_data[11]!=-1);
		     }
                     if(xhblock.mx_data[jj]>=key)
                     {
                         goout=1;
                         break;
                     }         
		     if(xhblock.mx_data[jj+1]==-1)
		     {    
                         break;
		     }

	     }
             if(goout==1) break;
             nbp=xhblock.mx_data[9];
             if(xhblock.mx_data[9]!=-1)      xhblock=ind_getblock(nbp,this->head.indh_name);     
    }
    while (nbp!=-1);
    if (pis.size()==0)       kjj=-1;    //cout<<"kjj:_____"<<kjj<<endl;
    /* if (this->ind_debug==1)
	{
		for (int h=0; h<mxb.size(); h++)
	        {
		    cout<<"--Node: "<<mxb[h].mx_locinfile<<" Type: ";
			if (mxb[h].mx_data[0]==2)
			{
				cout<<" internal Function: find Value:"<<key<<endl;
			}
			if (mxb[h].mx_data[0]==1)
			{
				cout<<" leaf Function: find Value:"<<key<<endl;
                        }
			
	        }
            if(kjj!=-1)  cout<<"--Node: "<<tob<<" Type: pointer bucket Function: traverse "<<endl;

	}  */
           this->mxb.clear();	
             return conc;	
}
vector<int> Index::ind_select_notequal(int tablenumber, int attnumber, int key)
{
	    int insert_result;                          
	char indexname[5];
        /*         cout<<"___________attnumber_________:"<<attnumber<<endl;
                 cout<<"___this->catalog->ta[tablenumber].at_index[0]___: "<<this->catalog->ta[tablenumber].at_index[0]<<endl;
                 cout<<"___this->catalog->ta[tablenumber].at_index[1]____: "<<this->catalog->ta[tablenumber].at_index[1]<<endl;
               cout<<"this->catalog->ta[tablenumber].at_index_name[0]:__ "<<this->catalog->ta[tablenumber].at_index_name[0]<<endl;
                 cout<<"this->catalog->ta[tablenumber].at_index_name[1]___: "<<this->catalog->ta[tablenumber].at_index_name[1]<<endl;
          */
     /*   if(this->catalog->ta[tablenumber].at_index[0]==attnumber)   strcpy(indexname,this->catalog->ta[tablenumber].at_index_name[0]);
        if(this->catalog->ta[tablenumber].at_index[1]==attnumber)   strcpy(indexname,this->catalog->ta[tablenumber].at_index_name[1]);
    */
	    for(int ju=0;ju<INDEXT;ju++)
	{
         if(this->catalog->ta[tablenumber].at_index[ju]==attnumber)   
			 strcpy(indexname,this->catalog->ta[tablenumber].at_index_name[ju]);
	}
	head.header_disk2mom(indexname);

	vector<int> pis;
    vector<int> conc;
	int tob;
        int kjj=0;
        int jj;
	pis=ind_lookup(key);
               //   cout<<"___________pis0____________"<<pis[0]<<endl;
	//Got from method Function
	if (pis.size()>0)
	{
	    int leafn=mxb.size()-1;
               //     for (int nc=(this->mxb.size()-1); nc>=0; nc--) cout<<"_mxb["<<nc<<"]: "<<mxb[leafn].mx_data[nc]<<endl;
	    for (jj=1; jj<5; jj++)
	    {                 //   cout<<"mxb[leafn].mx_data[jj]__________: "<<mxb[leafn].mx_data[jj]<<endl;
			if(mxb[leafn].mx_data[jj]==key)
			{
                               //   cout<<"_______________key
				tob=mxb[leafn].mx_data[jj+4];
                                break;
			}
 
	    }
	}   
    if (pis.size()==0)       kjj=-1;    //cout<<"kjj:_____"<<kjj<<endl;
    if (this->ind_debug==1)
	{
		for (int h=0; h<mxb.size(); h++)
	        {
		    cout<<"--Node: "<<mxb[h].mx_locinfile<<" Type: ";
			if (mxb[h].mx_data[0]==2)
			{
				cout<<" internal Function: find Value:"<<key<<endl;
			}
			if (mxb[h].mx_data[0]==1)
			{
				cout<<" leaf Function: find Value:"<<key<<endl;
                        }
			
	        }
            if(kjj!=-1)  cout<<"--Node: "<<tob<<" Type: pointer bucket Function: traverse "<<endl;

	}

    this->mxb.clear();
	int wsame=-1;
/*	for (int jh=0; jh<catalog->ta[tablenumber].num_block; jh++)
	{
	     for (jj=0; jj<pis.size(); jj++)
	     {
                 if (pis[jj]==jh)
                 {
                 wsame==1;      // this block should be discarded
				 break;
                 }
	     }
	     if (wsame==-1)
	     {
		 conc.push_back(jh);
	     }
	
	}
*/
        for (int jh=0; jh<catalog->ta[tablenumber].num_block; jh++)
        {
 		 conc.push_back(jh);
        }
    return conc;
}


void Index::ind_drop(char* indexname,char* tablename, Catalog* cata)
{   /*   cout<<"_______________begin_______________"<<endl;
   cout<<"__catalog->ta[this->head.indh_tableno].at_index[0]: "<<catalog->ta[this->head.indh_tableno].at_index[0]<<endl;
      cout<<" __ta[this->head.indh_tableno].at_index_name[0]: "<<catalog->ta[this->head.indh_tableno].at_index_name[0]<<endl;
  cout<<"__catalog->ta[this->head.indh_tableno].at_index[1]: "<<catalog->ta[this->head.indh_tableno].at_index[1]<<endl;
      cout<<" __ta[this->head.indh_tableno].at_index_name[1]: "<<catalog->ta[this->head.indh_tableno].at_index_name[1]<<endl;
 */
        this->catalog=cata;
	char* filename=indexname;
	char pathandf[30]="data/";
    strcat(pathandf,filename);
	remove(pathandf);
	int i=this->catalog->cata_get_table(tablename);
	if (strcmp(this->catalog->ta[i].at_index_name[0],indexname)==0)
        {
     		memset(this->catalog->ta[i].at_index_name[0],0,5);
                this->catalog->ta[i].at_index[0]=-1;
		strcpy(this->catalog->ta[i].at_index_name[0],this->catalog->ta[i].at_index_name[1]);
                this->catalog->ta[i].at_index[0]=this->catalog->ta[i].at_index[1];
		memset(this->catalog->ta[i].at_index_name[1],0,5);
                this->catalog->ta[i].at_index[1]=-1;     
		strcpy(this->catalog->ta[i].at_index_name[1],this->catalog->ta[i].at_index_name[2]);
                this->catalog->ta[i].at_index[1]=this->catalog->ta[i].at_index[2];
		memset(this->catalog->ta[i].at_index_name[2],0,5);
                this->catalog->ta[i].at_index[2]=-1;    
        }
	if (strcmp(this->catalog->ta[i].at_index_name[1],indexname)==0)
	{
		memset(this->catalog->ta[i].at_index_name[1],0,5);
                this->catalog->ta[i].at_index[1]=-1;
		strcpy(this->catalog->ta[i].at_index_name[1],this->catalog->ta[i].at_index_name[2]);
                this->catalog->ta[i].at_index[1]=this->catalog->ta[i].at_index[2];
		memset(this->catalog->ta[i].at_index_name[2],0,5);
                this->catalog->ta[i].at_index[2]=-1;

	}
	if (strcmp(this->catalog->ta[i].at_index_name[2],indexname)==0)
	{
		memset(this->catalog->ta[i].at_index_name[2],0,5);
        this->catalog->ta[i].at_index[2]=-1;
	}
   /*       cout<<"______________after_____________"<<endl;
  cout<<"__catalog->ta[this->head.indh_tableno].at_index[0]: "<<catalog->ta[this->head.indh_tableno].at_index[0]<<endl;
      cout<<" __ta[this->head.indh_tableno].at_index_name[0]: "<<catalog->ta[this->head.indh_tableno].at_index_name[0]<<endl;
  cout<<"__catalog->ta[this->head.indh_tableno].at_index[1]: "<<catalog->ta[this->head.indh_tableno].at_index[1]<<endl;
      cout<<" __ta[this->head.indh_tableno].at_index_name[1]: "<<catalog->ta[this->head.indh_tableno].at_index_name[1]<<endl;
 */
}
	
void Index::ind_ini(Catalog* cc, Buffer* b)
{
     this->catalog=cc;
     this->buffer=b;
}
    



