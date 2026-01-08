// Per-vertex attributes to be assembled from bound vertex buffers.
struct AssembledVertex
{
    float3 inPos;
    float2 inUV;
    float3 inNormal;
    float3 inColor;
};

struct GeometryData
{
    uint32_t vertexCount;
    uint32_t indexCount;
};

[[vk::binding(0, 0)]]
cbuffer Uniforms
{
    float4x4 projectionMatrix;
    float4x4 modelMatrix;
    float4x4 viewMatrix;
    float3x3 normalMatrix;
};

[[vk::binding(0, 1)]]
StructuredBuffer<AssembledVertex> VertexBuffer;
[[vk::binding(1, 1)]]
StructuredBuffer<uint32_t> IndexBuffer;
//[[vk::binding(2, 1)]]
//StructuredBuffer<GeometryData> geometryData;

// Output of the vertex shader, and input to the fragment shader.
struct CoarseVertex
{
    float3 position;
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
    output.coarseVertex.position = mul(modelMatrix, float4(position, 1.0f)).xyz;
    output.coarseVertex.normal = normalize(mul(normalMatrix, normal));
    output.coarseVertex.color = color;
    output.coarseVertex.uv = uv;

    return output;
}

float3 CalculateDiffuseLighting(float3 normal, float3 lightPos, float3 fragPos)
{
    static const float3 LIGHT_COLOR = float3(1.0f);
    static const float AMBIENT_STRENGTH = 0.1f;

    float3 ambient = AMBIENT_STRENGTH * LIGHT_COLOR;
    float3 pointToLight = normalize(lightPos - fragPos);
    float diffuseFactor = saturate(dot(pointToLight, normal));
    float3 diffuse = diffuseFactor * LIGHT_COLOR;
    float3 final = ambient + diffuse;

    return final;
}

[shader("fragment")]
Fragment fragmentMain(CoarseVertex coarseVertex : CoarseVertex) : SV_Target
{
    float3 fragPos = coarseVertex.position;
    float3 color = coarseVertex.color;
    float2 uv = coarseVertex.uv;
    float3 normal = coarseVertex.normal;

    static const float3 LIGHT_POS = float3(-6.0f, 5.0f, -4.0f);

    color = CalculateDiffuseLighting(normal, LIGHT_POS, fragPos);

    Fragment output;
    output.outFragColor = float4(color, 1.0);

    return output;
}