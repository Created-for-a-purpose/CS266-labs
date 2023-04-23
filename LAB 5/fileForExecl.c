#include<stdio.h>



int main(){
    FILE *f = fopen("fileForExecl.txt","w");
    fprintf(f,"The execl command called this file!\n");
    fprintf(f,"This print statemnt is from that file!\n");
    fclose(f);
    return 0;
}