#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAB_SIZE 30

int max(int * tab){
	int max = * tab;
	int i, max_index;
	for(i=0;i<TAB_SIZE;i++){
		if((* tab)>max){
			max = * tab;
			max_index = i;
			//printf("max = %d\n",max);
		}
		tab++;
	}
	return max_index;
}

int min(int * tab){
        int max = * tab;
        int i, max_index;
        for(i=0;i<TAB_SIZE;i++){
                if((* tab)<max){
                        max = * tab;
                        max_index = i;
                        //printf("max = %d\n",max);
                }
                tab++;
        }
        return max_index;
}

void min2max(int * tab){
	int sorted_tab[TAB_SIZE];
}

int main(){
	int tab[TAB_SIZE];
	srand(time(NULL));
	memset(tab,0,sizeof(int));
	int index = 0;
	for(index = 0;index < TAB_SIZE;index ++){
		tab[index]=rand()%100;
		printf("tab value %d is %d \n",index,tab[index]);
	}
	printf("val max %d et index val max %d \n",tab[max(tab)], max(tab));
	printf("val min %d et index val min %d \n",tab[min(tab)], min(tab));
	printf("ok\n");
	return 0;
}
