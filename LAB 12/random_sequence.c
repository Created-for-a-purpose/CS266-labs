#include<stdio.h>
#include<stdlib.h>


int main(){
    FILE* fp = fopen("random_sequence.txt","w");
    int i;
    for(i=0;i<100000;i++){
        fprintf(fp,"%d ",rand()%10000);
    }
    fclose(fp);
    return 0;
}