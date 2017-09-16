#ifndef _DYNARR_H_
#define _DYNARR_H_

    // a type definition exported by library:
    typedef struct __dynamic_array dynamic_array;
	struct __dynamic_array{  
        char **arr;
        int  *__size;
        int  *__capacity;

        void (^Iterate)();
        void (^Insert)(char*);
        void (^Destroy)();
    };

    extern dynamic_array* Init_Array (int, ...);  

    extern void Iterate ();  
    extern void iterate (dynamic_array*);

    extern void Insert (char*);
    extern void insert (dynamic_array*, char*);

    extern void Destroy ();
    extern void destroy (dynamic_array*);
#endif
