#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

const int Max=20000;
const int P=10;
const int MaxT=Max/P;
typedef int TIndice[P];
typedef int TTab[MaxT];
TTab T,T1;

void charger(FILE *f1,TTab T,int posIndice,int taille)
{
    fseek(f1,posIndice*sizeof(int),SEEK_SET);
    for(int i=0;i<taille;i++)
    {
            fread(&T[i],sizeof(int),1,f1);
    }
}

void sauvegarder(FILE *f1,TTab T2,int posIndice,int taille)
{
    fseek(f1,posIndice*sizeof(int),SEEK_SET);
    for(int i=0;i<taille;i++)
    {
            fwrite(&T2[i],sizeof(int),1,f1);
    }
}

bool tout_moins_un(TIndice indice)
{
    int cp=0;
    for(int i=0;i<P;i++)
    {
        if(indice[i]==-1)
        {
            cp++;
        }
    }
    if(cp==P)
    {
        return true;
    }
    else
    {
        return false;
    }

}

void chercher_min(FILE *f1,TIndice indice,int &min,int &pos)
{
    int i,start,valeur;
    start=0;
    while(indice[start]==-1 && start<P)
    {
        start=start+1;
    }
    fseek(f1,indice[start]*sizeof(int),SEEK_SET);
    pos=start;
    fread(&min,sizeof(int),1,f1);
    for(i=start+1;i<P;i++)
    {
        if(indice[i]!=-1)
        {
            fseek(f1,indice[i]*sizeof(int),SEEK_SET);
            fread(&valeur,sizeof(int),1,f1);
            if(valeur<min)
            {
                min=valeur;
                pos=i;
            }
        }
    }
}

void permuter(int &a,int &b)
{

      int x=a;
       a=b;
       b=x;
}

void Tri_Permutation(TTab T,int taille)
{
	int i,j,min,place;
     for(i=0;i<taille-1;i++)
	 {
        min=T[i];
        place=i;
        for(j=i+1;j<taille;j++)
		{
           if(T[j]<min)
		   {
              min=T[j];
              place=j;
           }
        }
		permuter(T[i],T[place]);
     }
}
void Tri_Bulles(TTab T,int taille)
{
      int i=0,j;
      bool arret=true;
      while( i<=taille-1 && arret==true)
      {
          arret=false;
          for(j=taille-1;j>=i+1;j--)
		  {
             if(T[j]<T[j-1])
			 {
               permuter(T[j],T[j-1]);
                arret=true;
             }
          }
          i++;
      }
}
void insert_trie( TTab T,int elem,int &n)
{
    int i,j;
    bool arret;
    int pos;
    arret=false;
    i=0;
    pos=n;
	while((i<=n-1)&&(arret==false))
	{
		if(T[i]>elem)
		{
			arret=true;
			pos=i;
			for(j=n;j>=i+1;j--)
			{
				T[j]=T[j-1];
			}
		}
		i++;
	}
	n++;
	T[pos]=elem;
}
void Tri_Insertion(TTab T1,TTab T2,int taille)
{
    int i,nbelem;
    nbelem=0;
    for(i=0;i<=taille-1;i++)
    {
    	insert_trie(T2,T1[i],nbelem);
	}
}
void segmentation(TTab T,int bi,int bs,int &place)
{
    int pivot,i,j;
    pivot=T[bi];
    i=bi+1;
    j=bs;

       while(i<=j)
        {
       			if(T[i]<pivot)
                {
				   	i++;
                }
                else if(T[i]>pivot)
                {
                    j--;
                }
                else if(i<=j)
                {
                    permuter(T[i],T[j]);
                    i++;
                    j--;
				}
	   }

       permuter(T[bi],T[j]);
	   place=j;
}
void Tri_QuickSort(TTab T,int bi,int bs)
{
     int place;
     if(bi<bs)
	 {
        segmentation(T,bi,bs,place);
        Tri_QuickSort(T,bi,place-1);
        Tri_QuickSort(T,place+1,bs);
     }
}
void IntercMultiple(char nom[],int taille,int typeTri)
{
    TIndice indice;
    int pos,min,i;
    bool arret;
    FILE *f1,*f2;
    f1=fopen(nom,"rb+");
    f2=fopen("tutu.bin","wb");

    for(int i=0;i<P;i++)
    {
        indice[i]=i*taille;
    }

    for(int i=0;i<P;i++)
    {
        charger(f1,T,indice[i],taille);
        switch(typeTri)
        {
            case 1:Tri_Permutation(T,taille);
                    break;
            case 2:Tri_Bulles(T,taille);
                    break;
            case 3:Tri_Insertion(T,T1,taille);
                    break;
            case 4:Tri_QuickSort(T,0,taille-1);
                    break;
        }
        switch(typeTri)
        {
            case 1:sauvegarder(f1,T,indice[i],taille);
                break;
            case 2:sauvegarder(f1,T,indice[i],taille);
                break;
            case 3: sauvegarder(f1,T1,indice[i],taille);
                break;
            case 4: sauvegarder(f1,T,indice[i],taille);
                break;
        }
    }
    arret=false;
    while(!arret)
    {
        chercher_min(f1,indice,min,pos);
        indice[pos]++;
        if(indice[pos]==(pos+1)*taille)
        {
            indice[pos]=-1;
        }
        fwrite(&min,sizeof(int),1,f2);
        arret=tout_moins_un(indice);
    }
    fclose(f1);
    fclose(f2);
    remove(nom);
    rename("tutu.bin",nom);
}

void creer_F(char nom[],int nb)
{
    FILE *f;
    f=fopen(nom,"wb");
    srand(time(NULL));
    for(int i=0;i<nb;i++)
    {
        int x=1+rand()%30000;
        fwrite(&x,sizeof(int),1,f);
    }
    fclose(f);
}

void copier_F(char nom[],char nom1[],int nb)
{
    FILE *f1,*f2;
    int x;
    f1=fopen(nom,"rb");
    f2=fopen(nom1,"wb");
    while(!feof(f1))
    {
        fread(&x,sizeof(int),1,f1);
        fwrite(&x,sizeof(int),1,f2);
    }
    fclose(f1);
    fclose(f2);
}

void chargeraff(char name[],int nb)
{
	int var;
	FILE *f=NULL;
	f=fopen(name,"rb");
	if(f!=NULL)
	{
	    for(int i=0;i<nb;i++)
		{
			fseek(f,i*sizeof(int),SEEK_SET);
	        fread(&var,sizeof(int),1,f);
	        printf("%d |",var);
	    }
	    printf("\n\n\n");
	}
fclose(f);

}


int main()
{
    FILE *f3=fopen("res.txt","w");
    int nb,taille;
    float t;
    clock_t t1,t2;
    for(int i=1;i<=5;i++)
    {
        nb=10*i;
        taille=nb/P;
        creer_F("original.bin",nb);

        copier_F("original.bin","permutation.bin",nb);
        t1=clock();
        IntercMultiple("permutation.bin",taille,1);
        t2=clock();
        t=(t2-t1)/(float)CLOCKS_PER_SEC;
        fprintf(f3,"%f ",t);
        //chargeraff("original.bin",nb);



        copier_F("original.bin","bulles.bin",nb);
        t1=clock();
        IntercMultiple("bulles.bin",taille,2);
        t2=clock();
        t=(t2-t1)/(float)CLOCKS_PER_SEC;
        fprintf(f3,"%f ",t);
        //chargeraff("bulles.bin",nb);

        copier_F("original.bin","insertion.bin",nb);
        t1=clock();
        IntercMultiple("insertion.bin",taille,3);
        t2=clock();
        t=(t2-t1)/(float)CLOCKS_PER_SEC;
        fprintf(f3,"%f ",t);

        copier_F("original.bin","quicksort.bin",nb);
        t1=clock();
        IntercMultiple("quicksort.bin",taille,4);
        t2=clock();
        t=(t2-t1)/(float)CLOCKS_PER_SEC;
        fprintf(f3,"%f ",t);

        fprintf(f3,"\n");
    }

	return 0;
	 }

