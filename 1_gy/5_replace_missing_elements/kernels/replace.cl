typedef struct __attribute__ ((packed)) float_order_s{
    float order[100];
    int n;
}float_order_t;

__kernel void replace(__global float_order_t * o)
{
    int GID = get_global_id(0);
    if ((GID < o->n) && (GID % 2 != 0)){
      o->order[GID] = (o->order[GID-1] + o->order[GID+1]) / 2;
    }
}