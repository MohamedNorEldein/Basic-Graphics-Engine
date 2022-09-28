
cbuffer INPUT_STRUCT
{
    int a[5] = { 0, 1, 2, 3, 4 };
    int b[5] = { 5, 4, 3, 2, 1 };
};


cbuffer OUTPUT_STRUCT
{
    int c[5];
};


[numthreads(5, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    c[DTid.r] = a[DTid.r] + b[DTid.r];

}