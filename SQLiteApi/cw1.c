#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>
#define SIZE 1000000

struct Rec {
  int id;         /* unikalny identyfikator, klucz główny */
  char name[20];  /* nazwa */
  char desc[90];  /* opis */
};

typedef struct Rec Rec;

void fillRec(Rec* r) {
    int i;
    for (i = 0; i < SIZE; i++){
        r[i].id = i;
        strcpy(r[i].name, "Name");
        strcpy(r[i].desc, "Description");
    }
}

void printRec(Rec* r) {
    int i;
    for (i = 0; i < SIZE; i++)
    {
        printf("id: %i, name: %s \n", r[i].id, r[i].name);
    }
}

Rec* find(int id, Rec* r) {
    int i;
    for(i=0;i<SIZE;i++) {
        if(r[i].id==id) {
            return &r[i];
        }
    }
   return 0;
}

void myFree(Rec* r) {
    free(r);
}

int main() {

    clock_t c1,c2,c3,c4;
    struct rusage mem;

    Rec* rDb;
    rDb = (Rec*)malloc(sizeof(Rec)*SIZE);
    if(rDb==NULL){
	printf("Error");
	return 0;
    }

    c1 = clock();
    fillRec(rDb);
    c2 = clock();

    printRec(rDb);

    c3 = clock();
    Rec* el = find(999999, rDb);
    c4 = clock();

    printf("Inserting time[s]: %f\n", ((float)c2-(float)c1)/CLOCKS_PER_SEC);
    printf("Finding time[s]: %f\n", ((float)c4-(float)c3)/CLOCKS_PER_SEC);

    printf("id: %i, name: %s \n", el->id, el->name);
    
    getrusage(RUSAGE_SELF, &mem);
    printf("Max mem usage[kB]: %ld\n", mem.ru_maxrss);

    myFree(rDb);

    return 0;
}
