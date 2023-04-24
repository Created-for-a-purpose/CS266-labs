#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int FRAME_SIZE = 5;


struct DLL_Node
{
    int data;
    struct DLL_Node *next;
    struct DLL_Node *prev;
    int freq;
};


struct DLL_Node *LRU = NULL;
struct DLL_Node *LRU_tail = NULL;
int size=0;


void insert(int page){
    struct DLL_Node *temp = (struct DLL_Node *)malloc(sizeof(struct DLL_Node));
    temp->data = page;
    temp->next = NULL;
    temp->prev = NULL;
    temp->freq = 1;
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
    struct DLL_Node *temp = LRU;
    while(temp!=NULL){
        if(temp->data == page){
            temp->freq++;
            return 0;
        }
        temp = temp->next;
    }
    // check if size is less than frame size
    if(size < FRAME_SIZE){
        insert(page);

        return 0;
    }
    // if page not found find the page with least frequency
    int min = LRU->freq;
    struct DLL_Node *min_node = LRU;
    temp = LRU;
    while(temp!=NULL){
        if(temp->freq < min){
            min = temp->freq;
            min_node = temp;
        }
        temp = temp->next;
    }
    // replace the page with least frequency
    min_node->data = page;
    min_node->freq = 1;
    return 1;
}

int main(){
    FILE *fp = fopen("random_sequence.txt", "r");
    int i;
    int pages[1000];
    for (i = 0; i < 1000; i++)
    {
        fscanf(fp, "%d ", &pages[i]);
    }
    fclose(fp);
    int page_faults = 0;
    for(int i=0;i<1000;i++){
        page_faults+=search(pages[i]);
    }
    printf("\nPage Faults: %d\n",page_faults);
    return 0;
}