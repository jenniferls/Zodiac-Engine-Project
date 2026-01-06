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
[[vk::binding(1, 1)]]
StructuredBuffer<uint32_t> IndexBuffer;

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
    uint32_t index = IndexBuffer[vertexID];
    AssembledVertex assembledVertex = VertexBuffer[index];
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

    float3 lightVector = normalize(float3(0.5f, 0.5f, 0.5f));
    float3 lightPos = float3(5.0f, 5.0f, 5.0f);

    color = float3(0.5f, 0.5f, 0.5f) * dot(normalize(normal), lightVector);

    Fragment output;
    output.outFragColor = float4(color, 1.0);

    return output;
}