#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cgraph/cgraph.h"
#include "cgraph/cgen/s2i.h"
#include "cgraph/cgen/svector.h"

typedef struct _congdan
{
	char ten[60];
	char cmnd[20];
	char sdt[20];
}congdan;

bn_tree_t s2i = NULL;
vector_t i2s = NULL;
int g_id = 0;

int get_save_id(char *key) {
	int res = s2i_value(s2i, key);    //search
	if(res != k_s2i_invalid) {		  //Nếu tìm thấy -> return
		return res;					
	} 					
	s2i_insert(s2i, key, g_id);		  //Nếu không tìm thấy -> insert
	svec_push_back(&i2s, key);
	res = g_id;
	++g_id;
	return res;
}

int main()
{
	s2i = rb_create_node();  //String to int
	i2s = gtv_create();	 //Int to string
//Doc file .txt
	FILE *tiepxuc=fopen("TIEPXUC.txt","r");
	FILE *dancu=fopen("DANCU.txt","r");
	if(tiepxuc == NULL||dancu==NULL) 
	{
	  printf("Loi doc file !!!");
	  return 0;
	}
	int n;
	fscanf(dancu,"%d",&n);
	congdan *dscongdan=(congdan*)calloc(100,sizeof(congdan));
	for(int i=0;i<n;i++)
	{
		fscanf(dancu,"%s %s %s",dscongdan[i].cmnd,dscongdan[i].sdt,dscongdan[i].ten);
		get_save_id(dscongdan[i].cmnd);
	}
	cgraph_ivec_t edges = cgraph_ivec_create();
	cgraph_ivec_t dsthoigian = cgraph_rvec_create();
	int m;
	fscanf(tiepxuc,"%d",&m);
	char tx1[20],tx2[20],thoigian[20];
	for(int i=0;i<m;i++)
	{
		fscanf(tiepxuc,"%s %s %s",tx1,tx2,thoigian);
		int id1=get_save_id(tx1);
		int id2=get_save_id(tx2);
		int idthoigian=get_save_id(thoigian);
		cgraph_ivec_push_back(&edges, id1);
		cgraph_ivec_push_back(&edges, id2);
		cgraph_ivec_push_back(&dsthoigian, idthoigian);
	}
	cgraph_t g = cgraph_create(edges, 0, CGRAPH_UNDIRECTED);
	printf("Tong so dinh cua do thi: %d\n",cgraph_vcount(g));
	printf("Tong so canh cua do thi: %d\n",cgraph_ecount(g));
//menu
	int lenh=0;
	do
	{
		printf("==Menu chuong trinh\n");
		printf("1. Dang ky cong dan moi\n2. Ghi nhan tiep xuc\n3. Tra cuu cong dan\n4. Tra cuu thong tin tiep xuc\n5. Tra cuu mang luoi tiep xuc\n6. Thoat\n");
		scanf("%d",&lenh);
		if(lenh<1||lenh>6) printf("LENH SAI! NHAP LAI!\n");
		else if(lenh==1)
		{
//task1
			char cmnd[20];
			printf("CMND: ");getc(stdin);scanf("%s",cmnd);
			int kiemtra=0;
			for(int i=0;i<n;i++)
			{
				if(strcmp(dscongdan[i].cmnd,cmnd)==0)
				{
					printf("Cap nhat thong tin\n");
					printf("Ten: ");getc(stdin);scanf("%s",dscongdan[i].ten);
					printf("Sdt: ");getc(stdin);scanf("%s",dscongdan[i].sdt);
					kiemtra++;
				}
			}
			if(kiemtra==0)
			{
				printf("Them cong dan\n");
				printf("Ten: ");getc(stdin);scanf("%s",dscongdan[n].ten);
				printf("Sdt: ");getc(stdin);scanf("%s",dscongdan[n].sdt);
				n++;
			}
		}
		else if(lenh==2)
		{
//task2
			char cmnd1[20],cmnd2[20];
			while(1)
			{
				printf("Nhap cmnd nguoi 1: ");getc(stdin);scanf("%s",cmnd1);
				int kiemtra = s2i_value(s2i, cmnd1);
				if(kiemtra != k_s2i_invalid) break;
				printf("CMND khong ton tai!NHAP LAI\n");
			}
			while(1)
			{
				printf("Nhap cmnd nguoi 2: ");getc(stdin);scanf("%s",cmnd2);
				int kiemtra = s2i_value(s2i, cmnd2);
				if(kiemtra != k_s2i_invalid) break;
				printf("CMND khong ton tai!NHAP LAI\n");
			}
			int eid;
			cgraph_get_eid(g, &eid,get_save_id(cmnd1), get_save_id(cmnd2),CGRAPH_UNDIRECTED);
			if(eid<0)
			{
				printf("Them tiep xuc\n");
				cgraph_ivec_t newedges = cgraph_ivec_create();
				cgraph_ivec_push_back(&newedges, get_save_id(cmnd1));
				cgraph_ivec_push_back(&newedges, get_save_id(cmnd2));
				cgraph_add_edges(g, newedges);
				char newtg[20];
				printf("Nhap thoi gian: ");getc(stdin);scanf("%s",newtg);
				cgraph_ivec_push_back(&dsthoigian, get_save_id(newtg));
			}else 
			{
				printf("Cap nhat tiep xuc");
				char newtg[20];
				printf("Nhap thoi gian: ");getc(stdin);scanf("%s",newtg);
				dsthoigian[eid]=get_save_id(newtg);
			}
			printf("Tong so tiep xuc: %d\n",cgraph_ecount(g));
		}
		else if(lenh==3)
		{
//task 3
			char cmndfind[20];
			printf("Nhap cmnd: ");getc(stdin);scanf("%s",cmndfind);
			int kiemtra = s2i_value(s2i, cmndfind);
			if(kiemtra == k_s2i_invalid) printf("CMND khong ton tai!\n");
			else if(kiemtra != k_s2i_invalid)
			{
				for(int i=0;i<n;i++)
				{
					if(strcmp(dscongdan[i].cmnd,cmndfind)==0)
					{
						printf("Ten: %s\n",dscongdan[i].ten);
						printf("Sdt: %s\n",dscongdan[i].sdt);
					}
				}
			}
		}
		else if(lenh==4)
		{
//task4
			char cmndfind[20];
			printf("Nhap cmnd: ");getc(stdin);scanf("%s",cmndfind);
			int kiemtra = s2i_value(s2i, cmndfind);
			if(kiemtra == k_s2i_invalid) printf("CMND khong ton tai!\n");
			else
			{
				cgraph_ivec_t neis = cgraph_ivec_create();
				cgraph_neighbors(g,&neis,get_save_id(cmndfind),CGRAPH_ALL);
				printf("Danh sach tiep xuc: \n");
				if (cgraph_ivec_size(neis)==0) printf("Khong co tiep xuc\n");
				else
				{
					for(int j=0; j<cgraph_ivec_size(neis);j++)
					{
						printf("%s ",i2s[neis[j]]);
					}
					printf("\n");
				}
			}
		}
		else if(lenh==5)
		{
//task5
			char cmndfind[20];
			printf("Nhap cmnd: ");getc(stdin);scanf("%s",cmndfind);
			int kiemtra = s2i_value(s2i, cmndfind);
			cgraph_ivec_t giantiep = cgraph_ivec_create();
			if(kiemtra == k_s2i_invalid) printf("CMND khong ton tai!\n");
			else
			{
				cgraph_ivec_t neis = cgraph_ivec_create();
				cgraph_neighbors(g,&neis,get_save_id(cmndfind),CGRAPH_ALL);
				printf("Danh sach tiep xuc truc tiep: \n");
				if (cgraph_ivec_size(neis)==0) printf("Khong co tiep xuc\n");
				else
				{
					for(int j=0; j<cgraph_ivec_size(neis);j++)
					{
						printf("%s \n",i2s[neis[j]]);
						cgraph_ivec_t neis1 = cgraph_ivec_create();
						cgraph_neighbors(g,&neis1,neis[j],CGRAPH_ALL);
						printf("Danh sach tiep xuc gian tiep: \n");
						for(int i=0;i<cgraph_ivec_size(neis1);i++)
						{
							int check2=0;
							for(int z=0;z<cgraph_ivec_size(neis);z++)
							{
								if(neis1[i]==neis[z]||neis1[i]==get_save_id(cmndfind)) check2++;
							}
							if(check2==0) printf("%s ",i2s[neis1[i]]);
						}
						printf("\n");
					}
					printf("\n");
				}
			}
		}
	}while(lenh!=6);
	cgraph_destroy(&g);
	fclose(dancu);fclose(tiepxuc);
	return 0;
}