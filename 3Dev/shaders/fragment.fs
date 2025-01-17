#version 330
precision mediump float;

const int maxLights = 32;
const int maxLodLevel = 7;
const float pi = 3.14159265;

uniform sampler2D albedo;
uniform sampler2D normalMap;
uniform sampler2D ao;
uniform sampler2D metalness;
uniform sampler2D emission;
uniform sampler2D roughness;
uniform sampler2D opacity;
uniform samplerCube irradiance;
uniform samplerCube prefilteredMap;
uniform sampler2D lut;

uniform vec3 nalbedo;
uniform bool nnormalMap;
uniform vec3 nemission;
uniform float nmetalness;
uniform float nroughness;
uniform bool nao;
uniform float nopacity;
uniform vec3 nirradiance;

uniform bool drawTransparency = false;

in vec2 coord;
in vec3 camposout;
in vec3 mnormal;
in vec3 mpos;
in mat3 tbn;
in vec4 lspaceout;

out vec4 color;

struct Shadow
{
	//sampler2DShadow shadowmap; Doesn't work on Windows?
	vec3 sourcepos;
	sampler2D shadowmap;
    bool perspective;
};

struct Light
{
    vec3 color;
    vec3 position;
    vec3 direction;
    vec3 attenuation;

    float cutoff;
    float outerCutoff;

    bool isactive;
};

uniform Light lights[maxLights];
uniform Shadow shadow;

float LinearizeDepth(float depth)
{
    if(shadow.perspective)
    {
        float z = depth * 2.0 - 1.0;
        return (2.0 * 0.01 * 1000.0) / (1000.0 + 0.01 - z * (1000.0 - 0.01));
    }
    return depth;
}

float CalcShadow()
{
    if(1.0 - dot(mnormal, normalize(shadow.sourcepos - mpos)) >= 1.0) return 0.0;
    vec3 pcoord = lspaceout.xyz / lspaceout.w;
    pcoord = pcoord * 0.5 + 0.5;
    if(pcoord.z > 1.0)
        return 0.0;
    /* Doesn't work on Windows?
    pcoord.z -= bias;
    return 1.0 - texture(shadows.shadowmap, pcoord);
    */
    float current = LinearizeDepth(pcoord.z);
    float ret = 0.0;
    vec2 pixelsize = 1.0 / textureSize(shadow.shadowmap, 0);
	for(int x = -0; x <= 1; ++x)
	{
		for(int y = -0; y <= 1; ++y)
		{
		    float pcf = LinearizeDepth(texture(shadow.shadowmap, pcoord.xy + vec2(x, y) * pixelsize).x);
		    ret += float(current - (shadow.perspective ? 0.05 : 0.00001) > pcf);
		}
	}
    return ret / 4.0;//float(currentcc > LinearizeDepth(texture(shadow.shadowmap, pcoord.xy).x));
}

float GGX(float ndoth, float rough)
{
    float dn = pi * pow(pow(ndoth, 2.0) * (pow(rough, 4.0) - 1.0) + 1.0, 2.0);

    return pow(rough, 4.0) / dn;
}

float GeometrySchlick(float ndotv, float rough)
{
    float k = pow(rough + 1.0, 2.0) / 8.0;
    float dn = ndotv * (1.0 - k) + k;

    return ndotv / dn;
}

float GeometrySmith(float ndotv, float ndotl, float rough)
{
    float ggx1  = GeometrySchlick(ndotv, rough);
    float ggx2  = GeometrySchlick(ndotl, rough);

    return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTh, vec3 f0, float rough)
{
    return f0 + (max(vec3(1.0 - rough), f0) - f0) * pow(1.0 - cosTh, 5.0);
}

vec3 CalcLight(Light light, vec3 norm, float rough, float metal, vec3 albedo, vec3 irr, vec3 f0)
{
    float theta = dot(normalize(light.position - mpos), normalize(-light.direction));
    float intensity = 1.0;
    if(light.cutoff != 1.0)
        intensity = clamp((theta - light.outerCutoff) / (light.cutoff - light.outerCutoff), 0.0, 1.0);
    if(theta < light.cutoff && intensity <= 0.0) return vec3(0.0);

    vec3 v = normalize(camposout - mpos);

    vec3 l = (light.cutoff == 1.0 ? normalize(light.position - mpos) : normalize(-light.direction));
    vec3 h = normalize(v + l);

    float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * length(l) + light.attenuation.z * pow(length(l), 2));

    float ndoth = max(dot(norm, h), 0.0);
    float ndotv = max(dot(norm, v), 0.0);
    float ndotl = max(dot(norm, l), 0.0);

    //attenuation = 1.0 / pow(length(l), 2.0);

    vec3 rad = light.color * attenuation * intensity;

    float ndf = GGX(ndoth, rough);
    float g = GeometrySmith(ndotv, ndotl, rough);
    vec3 f = FresnelSchlick(max(dot(h, v), 0.0), f0, rough);

    vec3 kspc = f;
    vec3 kdif = (1.0 - kspc) * (1.0 - metal);

    vec3 nm = ndf * g * f;
    float dn = 4.0 * ndotv * ndotl;
    vec3 spc = (nm / max(dn, 0.001));

    vec3 lo = (kdif * albedo / pi + spc) * rad * ndotl;

    return lo;
}

void main()
{
    float alpha = (nopacity < 0.0 ? texture(opacity, coord).x : nopacity);
    float w = texture(albedo, coord).w;
    if(w != alpha && w != 1.0)
    	alpha = w;

    if(!drawTransparency && alpha < 1.0)
        discard;
    if(drawTransparency && alpha == 1.0)
    {
        color = vec4(0.0, 0.0, 0.0, 1.0 * pow(10.0, -20.0)); // need to set gl_FragDepth to something
        return;
    }

    vec3 norm;
    if(nnormalMap) norm = normalize(tbn * normalize(texture(normalMap, coord).xyz * 2.0 - 1.0));
    else norm = normalize(mnormal);

    vec3 emission = (nemission.x < 0.0 ? texture(emission, coord).xyz : nemission);
    float rough = (nroughness < 0.0 ? texture(roughness, coord).x : nroughness);
    float metal = (nmetalness < 0.0 ? texture(metalness, coord).x : nmetalness);
    float ao = (nao ? texture(ao, coord).x : 1.0);
    vec3 alb = (nalbedo.x < 0.0 ? texture(albedo, coord).xyz : nalbedo);
    vec3 irr = (nirradiance.x < 0.0 ? texture(irradiance, norm).xyz : nirradiance);
    vec3 prefiltered = textureLod(prefilteredMap, reflect(-normalize(camposout - mpos), norm), rough * maxLodLevel).xyz;

    vec3 total = vec3(0.0);
    float shadow = 0.0;
    vec3 f0 = mix(vec3(0.04), alb, metal);
    int i = 0;
    while(lights[i].isactive)
    {
        total += CalcLight(lights[i], norm, rough, metal, alb, irr, f0);
        i++;
    }
    shadow = CalcShadow();

    vec3 f = FresnelSchlick(max(dot(norm, normalize(camposout - mpos)), 0.0), f0, rough);
    vec3 kspc = f;
    vec3 kdif = (1.0 - kspc) * (1.0 - metal);

    vec2 brdf = normalize(texture(lut, vec2(max(dot(norm, normalize(camposout - mpos)), 0.0), rough)).xy);
    vec3 spc = prefiltered * (f * brdf.x + brdf.y);

    vec3 diffuse = irr * alb;
    vec3 ambient = ((kdif * diffuse) + spc) * ao;

    total += ambient / 2;
    color = vec4((total * (length(emission) > 0.0 ? 1.0 : (1.0 - shadow)) + ambient / 2) + emission, (alpha < 1.0 ? alpha + ((total.x + total.y, + total.z) / 3.0) : 1.0));
}
