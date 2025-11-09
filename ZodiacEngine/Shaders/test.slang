// Per-vertex attributes to be assembled from bound vertex buffers.
struct AssembledVertex
{
    float3 inPos    : POSITION;
    float3 inColor  : COLOR;
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

    float4x4 ProjectionView = mul(viewMatrix, projectionMatrix);
    float4x4 MVP = mul(modelMatrix, ProjectionView);
    output.sv_position = mul(float4(position, 1.0), MVP);
    output.coarseVertex.color = color;

    return output;
}

[shader("fragment")]
Fragment fragmentMain(CoarseVertex coarseVertex : CoarseVertex) : SV_Target
{
    float3 color = coarseVertex.color;

    Fragment output;
    output.outFragColor = float4(color, 1.0);

    return output;
}