
struct output
{
    float data;
};

RWStructuredBuffer<output> OutputBuf : register(u0);


[numthreads(5, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    OutputBuf[DTid.x].data = 2.0f + OutputBuf[DTid.x].data;

}