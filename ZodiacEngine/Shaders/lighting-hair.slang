implementing lighting;

float F0_Hair() {
    const float ior = 1.55f;
    const float F0 = pow((1.0f - ior) / (1 + ior), 2);
    return F0;
}

float3 ShiftTangent(float3 T, float3 N, float shift)
{
    return normalize(T + N * shift);
}