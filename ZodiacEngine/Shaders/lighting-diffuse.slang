implementing lighting;

public float3 CalculateDiffuseLighting(float3 normal, float3 lightPos, float3 fragPos) {
    static const float3 LIGHT_COLOR = float3(1.0f);
    static const float AMBIENT_STRENGTH = 0.1f;

    float3 ambient = AMBIENT_STRENGTH * LIGHT_COLOR;
    float3 pointToLight = normalize(lightPos - fragPos);
    float diffuseFactor = saturate(dot(pointToLight, normal));
    float3 diffuse = diffuseFactor * LIGHT_COLOR;
    float3 final = ambient + diffuse;

    return final;
}