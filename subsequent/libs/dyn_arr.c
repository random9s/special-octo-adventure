#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <Block.h>
#include "../deps/dyn_arr.h"

#define MAX_STR_LEN sizeof(char)*20

void iterate (dynamic_array *dyn) {
    //Print all vals
    for (int i = 0; i < *dyn->__size; i++) {
        int offset = 0;
        if (i > 0) offset = ( MAX_STR_LEN * i );

        //Get ptr at offset
        char *ptr = (*dyn->arr + offset);
        printf ("[%d] %s\n", i, ptr);
    }
    printf("\n");

    return ;
}

void insert (dynamic_array *dyn, char* val) {
    //Check that input string isn't larger than max size
    if ( sizeof(val) >= MAX_STR_LEN-1) {
        perror("string larger than max");
        exit(1);
    }

    //Check if curr size is equal to capacity
    if (*dyn->__size == *dyn->__capacity) {
        *dyn->__capacity *= 2;
        printf("Realloc'd mem... new capacity %d\n", *dyn->__capacity);
        
        //Realloc bigger chunk of memory
        dyn->arr = realloc(dyn->arr, sizeof(*dyn->arr) * (*dyn->__capacity));
        *dyn->arr = realloc (*dyn->arr, sizeof (**dyn->arr) * MAX_STR_LEN * (*dyn->__capacity));
    }

    //Get ptr at offset
    int offset = ( MAX_STR_LEN * (*dyn->__size) );
    char *ptr = (*dyn->arr + offset);

    //Copy value to ptr
    strcpy(ptr, val);

    //Inc size counter
    *dyn->__size = *dyn->__size+1;
    return ;
}

void destroy(dynamic_array *dyn) {
    //Free pointers in 2D arr
    free(*dyn->arr);
    //Free top level pointer
    free(dyn->arr);

    //Free cap and size 
    free(dyn->__capacity);
    free(dyn->__size);

    //Free blocks
    Block_release(dyn->Iterate);
    Block_release(dyn->Insert);
    Block_release(dyn->Destroy);
    //Free struct pointer
    free(dyn);

    return ;
}

dynamic_array* Init_Array (int num_args, ...) {
    //Create new struct
    dynamic_array *dyn = malloc (sizeof (*dyn) );

    //block around iterate that passes in dyn 
    void (^Iterate)() = ^() {
       iterate(dyn); 
    };
    dyn->Iterate = Block_copy(Iterate);

    // block around insert 
    void (^Insert)(char*) = ^(char *val) {
        insert(dyn, val);
    };
    dyn->Insert = Block_copy(Insert);

    // block around destroy
    void (^Destroy)() = ^() {
        destroy(dyn);
    };
    dyn->Destroy = Block_copy(Destroy);

    //Add cap and size
    dyn->__capacity = malloc ( sizeof(*dyn->__capacity) );
    dyn->__size = malloc ( sizeof(*dyn->__size) );
    *dyn->__capacity = num_args+1;
    *dyn->__size = num_args;

    //Malloc double pointer
    dyn->arr  = malloc(sizeof(*dyn->arr) * (*dyn->__capacity));
    *dyn->arr = malloc(sizeof(**dyn->arr) * MAX_STR_LEN * (*dyn->__capacity));

    //Parse variadic args
    va_list ap;
   	va_start (ap, num_args);
   	for (int i = 0; i < num_args; i++) {
        //Figure offset
        int offset = 0;
        if (i > 0) offset = ( MAX_STR_LEN * i );

        //Get ptr at offset
        char *ptr = (*dyn->arr + offset);

        //Store variadic arg in ptr
		char *val = va_arg (ap, char*);
        strcpy(ptr, val);
    }

   va_end (ap);

   return dyn;
};
