__kernel void vector_sum(__global int* a, __global int* b, __global int* c, int n)
{
    int GID = get_global_id(0);
    if (GID < n){
        c[GID] = a[GID] + b[GID];
        printf("kernel %d: %d\n",GID,c[GID]);
    }
}