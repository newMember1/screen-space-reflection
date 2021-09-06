#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColor;
uniform sampler2D gBackDepth;

uniform mat4 projection;
uniform mat4 inverseProjection;

bool intersect(float pDepth, vec2 screenCoord)
{
    vec2 realCoord = screenCoord * 0.5 + 0.5;
    float backDepth = texture(gBackDepth, realCoord).x;
    float frontDepth = texture(gColor, realCoord).w;
    if(pDepth > frontDepth && pDepth < backDepth)
        return true;
    return false;
}

//单独来获取采样点 
bool getHitPosUV(vec3 p, inout vec2 lastScreenCoord, inout vec2 hitUV)
{
    vec4 screenCoord = projection * vec4(p, 1.0f);
    screenCoord = screenCoord / screenCoord.w;
    float pDepth = screenCoord.z;
    vec2 delta = screenCoord.xy - lastScreenCoord.xy;

    if(abs(delta.x) >= 1.0 / 800 || abs(delta.y) >= 1 / 600)
    {
        if(screenCoord.x < -1 || screenCoord.x > 1 || screenCoord.y < -1 || screenCoord.y > 1 || screenCoord.z > 1)
            return false;
        lastScreenCoord = screenCoord.xy;
        if(intersect(pDepth, screenCoord.xy))
        {
            hitUV = screenCoord.xy * 0.5 + 0.5;
            return true;
        }
    }
    return false;
}

int maxSteps = 256;
int maxSplitCounts = 64;
float rayLength = 10.0;
bool rayMarching(vec3 startPos, vec3 direc, out vec2 hitUV, out vec3 debugColor)
{
    vec4 screenStartPos = projection * vec4(startPos, 1.0f);
    screenStartPos = screenStartPos / screenStartPos.w;
    vec2 lastScreenCoord = screenStartPos.xy;
    vec3 deltaDir = 1.0f / maxSteps * rayLength * direc;

    for(int i = 0; i < maxSteps; ++i)
    {
        vec3 p = startPos + i * deltaDir;

        if(getHitPosUV(p, lastScreenCoord, hitUV))
        {
            float multi = 0.5;
            int cnt = 0;
            for(int j = 0; j < maxSplitCounts; ++j)
            {
                if(getHitPosUV(p - multi * deltaDir, lastScreenCoord, hitUV))
                {   ++cnt;
                    p = p - multi * deltaDir;
                }
                multi *= 0.5;
            }
            debugColor = vec3(1.0 * cnt / maxSplitCounts );
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

    vec2 hitUV = vec2(0.0f);
    vec3 outColor = vec3(0.0f);
    if(rayMarching(pos.xyz, reflectDir, hitUV, outColor))
    {
        FragColor = texture(gColor, hitUV);
        // FragColor = vec4(outColor, 1.0f);
    }
    else
        FragColor = vec4(col, 1.0f);
}