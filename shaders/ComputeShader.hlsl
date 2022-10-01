
Buffer<float> InputBuf : register(t0);
RWBuffer<float> OutputBuf : register(u0);
// Group size
#define size_x 20
#define size_y 1
// Declare one thread f o r each texel of the input texture.
[numthreads(size_x, size_y, 1)]
void main(uint3 DispatchThreadID : SV_DispatchThreadID)
{
    float Value = InputBuf.Load(DispatchThreadID.x);
    OutputBuf[DispatchThreadID.x] = 2.0f * Value;
}