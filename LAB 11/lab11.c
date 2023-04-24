#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<malloc.h>
#include<unistd.h>

int TLB[32][2];
int pageTable[1<<8][2];
int physicalMemory[256][256];


//this function fills the physical memory with random values (for simulation)
void fillPhysicalMemory(){
    int i,j;
    for(i=0;i<256;i++){
        for(j=0;j<256;j++){
            physicalMemory[i][j] = rand()%(1<<20);
        }
    }
}

//this function generates 100 random addresses and stores them in a file

void generateAddresses(){
    FILE* f = fopen("address.txt","w");
    int i;
    //reset the random number generator
    srand(time(NULL));
    //generate 100 random addresses, each of 4 bytes
    for(i=0;i<100;i++){
        fprintf(f,"%d\n",rand()%(1<<16));
    }
    fclose(f);
}

//this function finds page number in TLB in O(logn) time

int TLB_lookup(int pageNumber){
    int i=0,j=31;
    while(i<=j){
        int mid = (i+j)/2;
        if(TLB[mid][0]==pageNumber){
            return mid;
        }
        else if(TLB[mid][0]<pageNumber){
            i = mid+1;
        }
        else{
            j = mid-1;
        }
    }
    return -1;
}



// this function finds the address in page table and returns the frame number (if TLB miss occurs)

int pageTable_lookup(int pageNumber){
    int i = 0,j = 1<<8;
    while(i<=j){
        int mid = (i+j)/2;
        if(pageTable[mid][0]==pageNumber){
            return mid;
        }
        else if(pageTable[mid][0]<pageNumber){
            i = mid+1;
        }
        else{
            j = mid-1;
        }
    }
    return -1;
}

// this function fills page table (for simulation)
void fillPageTable(){
    int i;
    for(i=0;i<(1<<8);i++){
        pageTable[i][0] = i;
        pageTable[i][1] = rand()%(1<<8);
    }
}

// this function fills TLB with random values (for simulation)
void fillTLB(){
    int i;
    for(i=0;i<32;i++){
        TLB[i][0] = rand()%(1<<8);
        TLB[i][1] = rand()%(1<<8);
    }
}

int main(){
    int TLB_hits = 0,pageTable_hits = 0;
    int i,j;

    //generate 100 random addresses
    generateAddresses();


    
    //fill the page table and TLB with random values
    fillPageTable();
    fillTLB();
    fillPhysicalMemory();
    // so that all write has happened before proceeding
    sleep(1);

    //read the addresses from the file
    FILE* f = fopen("address.txt","r");
    FILE* of = fopen("output.txt","w");
    for(i=0;i<100;i++){
        int mask1 = 0xFF;
        int mask2 = 0xFF00;
        int addr=0;
        fscanf(f,"%d",&addr);
        int pageNumber = addr & mask2;
        pageNumber = pageNumber>>8;
        int offset = addr & mask1;
        //check if the page number is in TLB
        int TLB_index = TLB_lookup(pageNumber);
        if(TLB_index!=-1){
            TLB_hits++;

            //get the frame number from TLB
            int frameNumber = TLB[TLB_index][1];

            fprintf(of,"Virtual address: %6d is available at frame number: %6d and having value: %6d\n",addr,(frameNumber<<8) + offset,physicalMemory[frameNumber][offset]);
            
        }
        else{
            //check if the page number is in page table
            int frameNumber = pageTable_lookup(pageNumber);
            if(frameNumber!=-1){
                pageTable_hits++;

                //get the frame number from page table
                frameNumber = pageTable[frameNumber][1];
                fprintf(of,"Virtual address: %6d is available at frame number: %6d and having value: %6d\n",addr,(frameNumber<<8) + offset,physicalMemory[frameNumber][offset]);
            }
            else{
                //page fault
            }
        }
    }
    // page fault rate = (100 - (TLB_hits + pageTable_hits))/100
    printf("\n\nPage fault rate = %f",(100 - (TLB_hits + pageTable_hits))/100.0);

    // TLB hit rate = TLB_hits/100
    printf("\n\nTLB hit rate = %f\n\n",TLB_hits/100.0);


    /*
        The TLB hit rate is 0.0 (in most cases) because the TLB is filled with random values and the addresses generated are also random.
        So, the chances of a TLB hit are very low.

        The page fault rate will always be 0.0 because the page table is filled with all possible page numbers.

            There are 256 frames in our memory each of size 256 bytes. So, the total memory size is 256*256 = 65536 bytes.
            No, for each frame, there is a page associated with it. So, the total number of pages are 256.
            Also, our page table has 2^8 = 256 entries. So, each entry in the page table will have a page number and a frame number.
            Hence, there will never be a page fault.
    */
    return 0;
}