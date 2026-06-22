implementing lighting;
import math;

public float3 CalculateDiffuseLighting(float3 normal, float3 lightPos, float3 fragPos, float3 cameraPos) {
    static const float3 LIGHT_COLOR = float3(1.0f);
    static const float AMBIENT_STRENGTH = 0.2f;
    static const float ROUGHNESS = 0.5f;

    float3 ambient = AMBIENT_STRENGTH * LIGHT_COLOR;

    float3 pointToLight = normalize(lightPos - fragPos);
    float3 pointToCamera = normalize(cameraPos - fragPos);
    float3 H = normalize(pointToLight + pointToCamera);

    float NdotL = saturate(dot(normal, pointToLight));
    float NdotV = saturate(dot(normal, pointToCamera));
    float LdotH = saturate(dot(pointToLight, H));

    float diffuseFactor = CalculateDiffuseBRDF(NdotL, NdotV, LdotH, ROUGHNESS);
    float3 diffuse = diffuseFactor * LIGHT_COLOR * NdotL;

    float3 final = ambient + diffuse;

    return final;
}

public float CalculateDiffuseBRDF(float NdotL, float NdotV, float LdotH, float PerceptualRoughness) {
    //return 1.0f / PI;
    return DisneyDiffuse(NdotL, NdotV, LdotH, PerceptualRoughness) / PI;
}

public float DisneyDiffuse(float NdotL, float NdotV, float LdotH, float LinearRoughness) {
    float energyBias = lerp(0.0f, 0.5f, LinearRoughness);
    float energyFactor = lerp(1.0f, 1.0f / 1.51f, LinearRoughness);
    float f90 = energyBias + 2.0f * LdotH * LdotH * LinearRoughness;
    float lightScatter = FresnelSchlick(NdotL, f90, 1.0f);
    float viewScatter = FresnelSchlick(NdotV, f90, 1.0f);

    return lightScatter * viewScatter * energyFactor;
}

public float3 FresnelSchlick(float cosTheta, float3 F90, float3 F0) {
    return F0 + (F90 - F0) * pow(1.0f - cosTheta, 5.0f);
}

public float FresnelSchlick(float cosTheta, float F90, float F0) {
    return F0 + (F90 - F0) * pow(1.0f - cosTheta, 5.0f);
}