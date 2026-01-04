// Per-vertex attributes to be assembled from bound vertex buffers.
struct AssembledVertex
{
    float3 inPos;
    float2 inUV;
    float3 inNormal;
    float3 inColor;
};

[[vk::binding(0, 0)]]
cbuffer Uniforms
{
    float4x4 projectionMatrix;
    float4x4 modelMatrix;
    float4x4 viewMatrix;
}

[[vk::binding(0, 1)]]
StructuredBuffer<AssembledVertex> VertexBuffer;

// Output of the vertex shader, and input to the fragment shader.
struct CoarseVertex
{
    float3 color;
    float2 uv;
    float3 normal;
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
VertexStageOutput vertexMain(uint vertexID: SV_VertexID)
{
    AssembledVertex assembledVertex = VertexBuffer[vertexID];
    VertexStageOutput output;

    float3 position = assembledVertex.inPos;
    float3 color = assembledVertex.inColor;
    float2 uv = assembledVertex.inUV;
    float3 normal = assembledVertex.inNormal;

    float4x4 ProjectionView = mul(projectionMatrix, viewMatrix);
    float4x4 MVP = mul(ProjectionView, modelMatrix);

    output.sv_position = mul(MVP, float4(position, 1.0));
    output.coarseVertex.color = color;
    output.coarseVertex.uv = uv;
    output.coarseVertex.normal = normal;

    return output;
}

[shader("fragment")]
Fragment fragmentMain(CoarseVertex coarseVertex : CoarseVertex) : SV_Target
{
    float3 color = coarseVertex.color;
    float2 uv = coarseVertex.uv;
    float3 normal = coarseVertex.normal;

    Fragment output;
    output.outFragColor = float4(color, 1.0);

    return output;
}