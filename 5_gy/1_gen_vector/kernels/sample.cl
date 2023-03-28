__kernel void rand_vector(__global int* out, int n)
{
    size_t GID = get_global_id(0);
    uint2 s;
    s.x = 6;
    s.y = 9;
    if (GID < n){
        // 'randoms' is ulong passed to kernel
        uint seed = s.x + GID;
        uint t = seed ^ (seed << 11);  
        uint result = s.y ^ (s.y >> 19) ^ (t ^ (t >> 8));
        out[GID] = result % 50;
    }
}