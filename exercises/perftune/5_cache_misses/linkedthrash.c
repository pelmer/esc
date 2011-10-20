/*

Linked list cache thrashing example

Andrzej Nowak
CERN

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <strings.h>

#define ITERATIONS 1
#define varprint(x) printf("%s: %d\n", #x, x)
#define ptrprint(x) printf("%s: %p\n", #x, x)

typedef struct _element {
    int item;
    char x[496];
    struct _element *next;
} element;

void walk(element *p) {
    long int result;
    int i;
    element *home = p;
    
    p = home;
    while(p->next) {
        p = p->next;
    }
    
    printf("Last pointer: ");
    ptrprint(p);
}

void swap(element *list, int index1, int index2) {
    element *temp = malloc(sizeof(element));
    memcpy(temp, &list[index2], sizeof(element));
    memcpy(&list[index2], &list[index1], sizeof(element));
    memcpy(&list[index1], temp, sizeof(element));
    free(temp);
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Usage: %s table_size\n", argv[0]);
        exit(-1);
    }
            
    int i = 0;
    int table_size = atoi(argv[1]);
    printf("Using table capacity %d * %d = %.3f MB\n", (int)sizeof(element), table_size, sizeof(element)*table_size/1e6);
    
    element *list = (element *)malloc(sizeof(element) * table_size);
    element *head = list;
    bzero(list, sizeof(element) * table_size);
    int indices[table_size];
    
    int temp, random_index;
    srandom(time(NULL));

    for(i=0; i<table_size; i++) {
        indices[i] = i;
    }

    // this part actually won the award for the world's dumbest randomization method
    for(i=1; i<table_size-1; i++) {
        random_index = (random() % (table_size-2)) + 1;
        temp = indices[random_index];
        indices[random_index] = indices[i];
        indices[i] = temp;
    }

    for(i=0; i<table_size-1; i++) {
        list[indices[i]].next = &list[indices[i+1]];
        list[indices[i]].item = indices[i] * .234124 + .4575;
    }
    list[table_size-1].next = NULL;
    
    walk(head);
    
    return 0;    
}
