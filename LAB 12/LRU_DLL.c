#include <stdio.h>
#include <stdlib.h>

int FRAME_SIZE = 5;

struct DLL_Node
{
    int data;
    struct DLL_Node *next;
    struct DLL_Node *prev;
};

struct DLL_Node *LRU = NULL;
struct DLL_Node *LRU_tail = NULL;
int size=0;


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

void insert(int data)
{
    struct DLL_Node *temp = (struct DLL_Node *)malloc(sizeof(struct DLL_Node));
    temp->data = data;
    temp->next = NULL;
    temp->prev = NULL;
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

int search(int data)
{
    struct DLL_Node *temp = LRU;
    if(size==0){
        insert(data);
        return 0;
    }    
    while (temp != NULL)
    {
        if (temp->data == data)
        {
            while (temp != LRU)
            {
                temp->data = temp->prev->data;
                temp = temp->prev;
            }
            LRU->data = data;
            return 1;
        }
        temp = temp->next;
    }
    if(size<FRAME_SIZE){
        insert(data);
        return 0;
    }
    temp = LRU_tail;
    while (temp != NULL && temp->prev != NULL)
    {
        temp->data = temp->prev->data;
        temp = temp->prev;
    }
    LRU->data = data;
    return 0;
}

int main()
{
    FILE *fp = fopen("random_sequence.txt", "r");
    int i;
    int page_fault = 0;
    int pages[100000];
    for (i = 0; i < 100000; i++)
    {
        fscanf(fp, "%d ", &pages[i]);
    }
    fclose(fp);
    FILE *fp2 = fopen("out.dat", "w");
    for(FRAME_SIZE=5;FRAME_SIZE<=5000;FRAME_SIZE++){
        page_fault=0;
        reset();
        for (i = 0; i < 1000; i++)
        {
            // display();
            if (search(pages[i]) == 0)
            {
                page_fault++;
            }
        }
        fprintf(fp2,"%d %d\n", FRAME_SIZE,page_fault);
    }
    return 0;
}