/*
    Second chance algorithm
*/

#include <stdio.h>
#include <stdlib.h>


int FRAME_SIZE = 10;


struct DLL_Node
{
    int data;
    struct DLL_Node *next;
    struct DLL_Node *prev;
    int ref_bit;
};


struct DLL_Node *LRU = NULL;
struct DLL_Node *LRU_tail = NULL;
int size=0;

void insert(int data)
{
    struct DLL_Node *temp = (struct DLL_Node *)malloc(sizeof(struct DLL_Node));
    temp->data = data;
    temp->next = NULL;
    temp->prev = NULL;
    temp->ref_bit = 0;
    if (LRU == NULL)
    {
        LRU = temp;
        LRU_tail = temp;
    }

    else
    {
        temp->next = LRU;
        LRU->prev = temp;
        LRU = temp;
    }
    size++;
}

void display()
{
    struct DLL_Node *temp = LRU;
    printf("LRU: ");
    while (temp != NULL)
    {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

void reset(){
    size=0;
    struct DLL_Node *temp = LRU;
    while(temp!=NULL){
        LRU = LRU->next;
        free(temp);
        temp = LRU;
    }
    LRU_tail = NULL;
}

int search(int page){
    // search for the page
    struct DLL_Node *temp = LRU;
    while(temp!=NULL){
        if(temp->data == page){
            temp->ref_bit = 1;
            return 0;
        }
        temp = temp->next;
    }
    // if not found, check if the frame is full
    if(size == FRAME_SIZE){
        // if full, find the page with ref_bit = 0
        temp = LRU;
        while(temp!=NULL){
            if(temp->ref_bit == 0){
                temp->data = page;
                temp->ref_bit = 1;
                return 1;
            }
            else{
                temp->ref_bit = 0;
            }
            temp = temp->next;
        }
        temp = LRU;
        while(temp!=NULL){
            if(temp->ref_bit == 0){
                temp->data = page;
                temp->ref_bit = 1;
                return 1;
            }
            temp = temp->next;
        }
    }
    // if not full, insert the page
    else{
        insert(page);
        return 0;
    }
}

int main(){
    FILE *fp = fopen("random_sequence.txt", "r");
    int i;
    int page_fault = 0;
    int pages[1000];
    for (i = 0; i < 1000; i++)
    {
        fscanf(fp, "%d ", &pages[i]);
    }
    fclose(fp);
    int ct=0;
    for(i=0; i<1000; i++){
        ct+=search(pages[i]);
    }
    printf("Page Faults: %d\n", ct);
    return 0;
}