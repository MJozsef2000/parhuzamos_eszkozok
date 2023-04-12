typedef struct __attribute__ ((packed)) vector{
    int a;
    int b;
    int c;
    int n;
}Vector;

__kernel void vector_sum(__global Vector * vs, __global Vector * vs2)
{
    int GID = get_global_id(0);
    if (GID < vs[0].n){
        vs[GID].c = vs[GID].a + vs[GID].b;
        printf("kernel %d: %d\n",GID,vs[GID].c);
    }
    if ( vs[0].n <= GID && GID < vs[0].n*2){
        vs2[GID-5].c = vs2[GID-5].a + vs2[GID-5].b;
        printf("kernel %d: %d\n",GID,vs2[GID-5].c);
    }
}