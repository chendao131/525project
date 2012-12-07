#include "Header.h"

void Header:: header_disk2mom(char* indexname)
{
	FILE *fp;
    char pathandf[30]="data/";
    strcat(pathandf,indexname);
	fp=fopen(pathandf,"r+");

	fread(&this->indh_name, 5*sizeof(char),1,fp);
    fread(&this->indh_tablename, 5*sizeof(char),1,fp);
	fread(&this->indh_attributename, 8*sizeof(char),1,fp);
	fread(&this->indh_tableno,sizeof(int),1,fp);
	fread(&this->indh_attributeno,sizeof(int),1,fp);
	fread(&this->indh_check_dup,sizeof(int),1,fp);
	fread(&this->indh_root_loc,sizeof(int),1,fp);
        fread(&this->indh_nodetotal,sizeof(int),1,fp);
        fread(&this->left,10*sizeof(char),1,fp); // char left[10];  

        fclose(fp);
}

void Header:: header_mom2disk(char* indexname)
{
	FILE *fp;
    char pathandf[30]="data/";
    strcat(pathandf,indexname);
	fp=fopen(pathandf,"r+");

	fwrite(&this->indh_name, 5*sizeof(char),1,fp);

    fwrite(&this->indh_tablename, 5*sizeof(char),1,fp);
	fwrite(&this->indh_attributename, 8*sizeof(char),1,fp);
	fwrite(&this->indh_tableno,sizeof(int),1,fp);
	fwrite(&this->indh_attributeno,sizeof(int),1,fp);
	fwrite(&this->indh_check_dup,sizeof(int),1,fp);
	fwrite(&this->indh_root_loc,sizeof(int),1,fp);
        fwrite(&this->indh_nodetotal,sizeof(int),1,fp);
        fwrite(&this->left,10*sizeof(char),1,fp);

    fclose(fp);

}
void Header:: header_moveroot(char* indexname, int newloc)
{
	FILE *fp;
    char pathandf[30]="data/";
    strcat(pathandf,indexname);
	fp=fopen(pathandf,"r+");

	fseek(fp, 18*sizeof(char)+3*sizeof(int), 0);
	fwrite(&newloc,sizeof(int),1,fp);
    
	fclose(fp);
}

void Header:: header_ini(char* indexname)
{
	FILE *fp;
    char pathandf[30]="data/";
    strcat(pathandf,indexname);
	fp=fopen(pathandf,"r+");

	fwrite(&this->indh_name, 5*sizeof(char),1,fp);
        fwrite(&this->indh_tablename, 5*sizeof(char),1,fp);
	fwrite(&this->indh_attributename, 8*sizeof(char),1,fp);
	fwrite(&this->indh_tableno,sizeof(int),1,fp);
	fwrite(&this->indh_attributeno,sizeof(int),1,fp);
	fwrite(&this->indh_check_dup,sizeof(int),1,fp);
	fwrite(&this->indh_root_loc,sizeof(int),1,fp);
        fwrite(&this->indh_nodetotal,sizeof(int),1,fp);

    int d=-1;
	for(int i=0; i<12; i++)
	{
		fwrite(&d,sizeof(int),1,fp);
	}
    fclose(fp);
}
    
