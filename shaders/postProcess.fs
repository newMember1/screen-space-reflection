#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColor;
uniform sampler2D gBackDepth;

uniform mat4 projection;
uniform mat4 inverseProjection;

bool intersect(float currentDepth,float prevDepth, float depth, float backDepth)
{
    if((currentDepth > depth && currentDepth < backDepth) || (prevDepth > depth && prevDepth < backDepth))
        return true;

    return false;
}

bool rayMarching(vec3 startPos, vec3 direc, out vec3 outColor)
{
    int maxSteps = 64;
    float rayLength = 10.0;

    float currentDepth;
    float prevDepth;
    for(int i = 0; i < maxSteps; ++i)
    {
        vec3 p = startPos + 1.0f * i / maxSteps * rayLength * direc;
        vec4 screenCoord = projection * vec4(p, 1.0f);//clip space coordinate
        screenCoord /= screenCoord.w;//ndc space
        prevDepth = currentDepth;
        currentDepth = screenCoord.z;

        vec2 sampleCoord = screenCoord.xy / 2.0f + 0.5f;
        float dep = texture(gColor, sampleCoord).w;
        float backDep = texture(gBackDepth, sampleCoord).x;

        if(screenCoord.x < -1 || screenCoord.x > 1 || screenCoord.y < -1 || screenCoord.y > 1 || screenCoord.z > 1)
            return false;

        if(intersect(currentDepth, prevDepth, dep, backDep))
        {
            outColor = texture(gColor, sampleCoord).xyz;
            return true;
        }
    }

    return false;
}

void main()
{
    vec4 pos = texture(gPosition, TexCoords);//pos in view space and depth in back face render
    vec3 nor = normalize(texture(gNormal, TexCoords).rgb);//normal in view space
    vec3 col = texture(gColor, TexCoords).rgb;//col in screen space
    float dep = texture(gColor, TexCoords).w;//dep in screen space
    float backDep = texture(gBackDepth, TexCoords).x;//backDep in screen space

    vec3 viewDir = normalize(pos.xyz);
    vec3 reflectDir = normalize(reflect(viewDir, nor));

    vec3 outColor = vec3(0.0f);
    if(rayMarching(pos.xyz, reflectDir, outColor))
        FragColor = vec4(outColor, 1.0f);
    else
        FragColor = vec4(col, 1.0f);
}