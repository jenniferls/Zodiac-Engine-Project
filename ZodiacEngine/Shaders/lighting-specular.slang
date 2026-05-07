implementing lighting;

float FSchlick(float F0, float cosTheta) {
    return F0 + (1.0f - F0) * pow(1.0f - cosTheta, 5.0f);
}