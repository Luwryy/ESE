#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAB_SIZE 30

int max(int * tab){
	int max = tab[0];
	int i, max_index;
	for(i=0;i<TAB_SIZE;i++)
	{
		if(tab[i] > max){
			max = tab[i];
			max_index = i;
			//printf("max = %d\n",max);
		}
	}
	return max_index;
}

int min(int * tab){
        int max = tab[0];
        int i, max_index;

        for(i=0;i<TAB_SIZE;i++)
	{
                if(tab[i]<max)
		{
                        max = tab[i];
                        max_index = i;
                        //printf("min = %d\n",max);
                }
//		printf("%d %d\n",tab[i],i);
        }

        return max_index;
}

void min2max(int * tab){
	int sorted_tab[TAB_SIZE];

	memset(sorted_tab,0,TAB_SIZE*sizeof(int));

	int min_index;

        for(int i=0;i<TAB_SIZE;i++)
	{
		min_index=min(tab);
      		sorted_tab[i] = tab[min_index];
		tab[min_index]=100;

		printf("%d %d\n", tab[i], sorted_tab[i]);
        }
	

	memcpy(tab, sorted_tab, TAB_SIZE*sizeof(int));
}

int main(){
	int tab[TAB_SIZE];
	srand(time(NULL));
	memset(tab, 0, TAB_SIZE * sizeof(int));
	int index = 0;
	for(index = 0;index < TAB_SIZE;index ++)
	{
		tab[index]=rand()%100;
		printf("tab value %d is %d \n",index,tab[index]);
	}

	printf("val max %d et index val max %d \n",tab[max(tab)], max(tab));
	printf("val min %d et index val min %d \n",tab[min(tab)], min(tab));

	/*min2max(tab);

	for(index = 0;index < TAB_SIZE;index ++)
	{
                tab[index]=rand()%100;
                printf("sorted value %d is %d \n",index,tab[index]);
        }*/
	printf("ok\n");
	return 0;
}
