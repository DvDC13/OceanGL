#version 450

out vec4 FragColor;

in vec3 normal;
in vec3 worldPosition;

uniform vec3 cameraPosition;

const float M_PI = 3.14159265359;

uniform float roughness;
uniform float metallic;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightIntensity;

uniform vec3 oceanColor;

vec4 sRGBToLinear( in vec4 value ) {
	return vec4( mix( pow( value.rgb * 0.9478672986 + vec3( 0.0521327014 ), vec3( 2.4 ) ), value.rgb * 0.0773993808, vec3( lessThanEqual( value.rgb, vec3( 0.04045 ) ) ) ), value.a );
}

vec4 LinearTosRGB( in vec4 value ) {
	return vec4( mix( pow( value.rgb, vec3( 0.41666 ) ) * 1.055 - vec3( 0.055 ), value.rgb * 12.92, vec3( lessThanEqual( value.rgb, vec3( 0.0031308 ) ) ) ), value.a );
}

float D_GGX(vec3 N, vec3 H, float roughness)
{
  float a = roughness * roughness;
  float a2 = a * a;
  float NoH = max(dot(N, H), 0.0);
  float NoH2 = NoH * NoH;
  
  float nom = a2;
  float denom = (NoH2 * (a2 - 1.0) + 1.0);

  return nom / (M_PI * denom * denom);
}

vec3 F_Schlick(float cosTheta, vec3 F0)
{
    return F0 + (vec3(1.0) - F0) * pow(1.0 - cosTheta, 5.0);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float G_Smith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

void main()
{
    vec3 albedo = sRGBToLinear(vec4(oceanColor, 1.0)).rgb;
    
    vec3 L = normalize(lightPos - worldPosition);
    vec3 N = normalize(normal);
    vec3 V = normalize(cameraPosition - worldPosition);
    vec3 H = normalize(L + V);

    float D = D_GGX(N, H, roughness);
    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    vec3 F = F_Schlick(max(dot(H, V), 0.0), F0);

    float G = G_Smith(N, V, L, roughness);

    vec3 numerator = D * F * G;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);

    vec3 specular = numerator / max(denominator, 0.001);

    vec3 ks = F;
    vec3 kd = vec3(1.0) - ks;
    kd *= 1.0 - metallic;

    float cosTheta = max(dot(N, L), 0.0);

    vec3 diffuse = kd * albedo / M_PI;

    vec3 irradiance = (diffuse + specular) * lightColor * lightIntensity * cosTheta;

    FragColor.rgba = LinearTosRGB(vec4(irradiance, 1.0));
}