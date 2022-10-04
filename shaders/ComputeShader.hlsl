
struct output
{
    float data;
};

RWStructuredBuffer<output> OutputBuf : register(u0);

[numthreads(5, 1, 1)]
void main(uint3 DispatchThreadID : SV_DispatchThreadID)
{
    OutputBuf[DispatchThreadID.x].data = 5.0f * OutputBuf[DispatchThreadID.x].data;
}