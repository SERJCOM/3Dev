#version 330
precision mediump float;

const int maxlights = 16;
const float pi = 3.14159265;

uniform sampler2D albedo;
uniform sampler2D normalmap;
uniform sampler2D ao;
uniform sampler2D metalness;
uniform sampler2D emission;
uniform sampler2D roughness;
uniform sampler2D opacity;
uniform samplerCube cubemap;

uniform vec3 nalbedo;
uniform bool nnormalmap;
uniform vec3 nemission;
uniform float nmetalness;
uniform float nroughness;
uniform bool nao;
uniform float nopacity;

in vec2 coord;
in vec3 camposout;
in vec3 mnormal;
in vec3 mpos;
in mat3 tbn;

out vec4 color;

struct Light
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    vec3 attenuation;

    bool isactive;
};

uniform Light lights[maxlights];

float GGX(float ndoth, float rough)
{
    float dn = pi * pow(pow(ndoth, 2) * (pow(rough, 4) - 1.0) + 1.0, 2);
    
    return pow(rough, 4) / dn;
}

float GeometrySchlickGGX(float ndotv, float rough)
{
    float k = pow(rough + 1.0, 2) / 8.0;
    float dn = ndotv * (1.0 - k) + k;
	
    return ndotv / dn;
}

float GeometrySmith(float ndotv, float ndotl, float rough)
{
    float ggx1  = GeometrySchlickGGX(ndotv, rough);
    float ggx2  = GeometrySchlickGGX(ndotl, rough);
	
    return ggx1 * ggx2;
}

vec3 FresnelShlick(float cosTh, vec3 f0, float rough)
{
    return f0 + (max(vec3(1.0 - rough), f0) - f0) * pow(1.0 - cosTh, 5.0);
}

vec3 CalcLight(Light light, vec3 norm)
{
    vec3 v = normalize(camposout - mpos);

    float rough = (nroughness < 0.0 ? texture(roughness, coord).x : nroughness);
    float metal = (nmetalness < 0.0 ? texture(metalness, coord).x : nmetalness);
    vec3 albedo = (nalbedo.x < 0.0 ? texture(albedo, coord).xyz : nalbedo);

    vec3 f0 = mix(vec3(0.04), albedo, vec3(metal));

    vec3 l = normalize(light.position - mpos);
    vec3 h = normalize(v + l);

    float attenuation/* = 1.0 / (light.attenuation.x + light.attenuation.y * length(l) + light.attenuation.z * pow(length(l), 2))*/;

    float ndoth = max(dot(norm, h), 0.0);
    float ndotv = max(dot(norm, v), 0.0);
    float ndotl = max(dot(norm, l), 0.0);

    attenuation = 1.0 / pow(length(l), 2);

    vec3 lcolor = light.diffuse;
    vec3 rad = lcolor * attenuation;

    float ndf = GGX(ndoth, rough);
    float g = GeometrySmith(ndotv, ndotl, rough);
    vec3 f = FresnelShlick(max(dot(h, v), 0.0), f0, rough);

    vec3 kspc = f;
    vec3 kdif = (vec3(1.0) - kspc) * (1.0 - metal);
    vec3 amb = (kdif * albedo) * (nao ? texture(ao, coord).xyz : vec3(1.0));

    vec3 nm = ndf * g * f;
    float dn = 4.0 * ndotv * ndotl;
    vec3 spc = (nm / max(dn, 0.001));
    
    vec3 lo = (amb / pi + spc) * rad * ndotl;

    return lo;
}

void main()
{
    vec3 norm;
    if(nnormalmap) norm = tbn * normalize(texture(normalmap, coord).xyz * 2.0 - 1.0);
    else norm = normalize(mnormal);

    vec3 emission = (nemission.x < 0.0 ? texture(emission, coord).xyz : nemission.xyz);

    /*vec3 reflected = reflect(normalize(mpos - camposout), normalize(norm));
    vec3 reflection = textureCube(cubemap, reflected).xyz;*/

    vec3 total = vec3(0.0);
    int i = 0;
    while(lights[i].isactive)
    {
        total += CalcLight(lights[i], norm);
        i++;
    }

    color = vec4(emission + total, (nopacity < 0.0 ? texture(opacity, coord).x : abs(nopacity)));
}