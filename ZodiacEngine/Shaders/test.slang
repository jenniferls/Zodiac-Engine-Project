// Per-vertex attributes to be assembled from bound vertex buffers.
struct AssembledVertex
{
    float3 inPos    : POSITION;
    float3 inColor  : COLOR;
    float2 inUV     : TEXCOORD0;
};

cbuffer Uniforms
{
    float4x4 projectionMatrix;
    float4x4 modelMatrix;
    float4x4 viewMatrix;
}

// Output of the vertex shader, and input to the fragment shader.
struct CoarseVertex
{
    float3 color;
    float2 uv;
};

// Output of the fragment shader
struct Fragment
{
    float4 outFragColor;
};

struct VertexStageOutput
{
    CoarseVertex coarseVertex   : CoarseVertex;
    float4 sv_position          : SV_Position;
};

[shader("vertex")]
VertexStageOutput vertexMain(AssembledVertex assembledVertex)
{
    VertexStageOutput output;

    float3 position = assembledVertex.inPos;
    float3 color = assembledVertex.inColor;
    float2 uv = assembledVertex.inUV;

    float4x4 ProjectionView = mul(projectionMatrix, viewMatrix);
    float4x4 MVP = mul(ProjectionView, modelMatrix);
    output.sv_position = mul(MVP, float4(position, 1.0));
    output.coarseVertex.color = color;
    output.coarseVertex.uv = uv;

    return output;
}

[shader("fragment")]
Fragment fragmentMain(CoarseVertex coarseVertex : CoarseVertex) : SV_Target
{
    float3 color = coarseVertex.color;
    float2 uv = coarseVertex.uv;

    Fragment output;
    output.outFragColor = float4(color, 1.0);

    return output;
}