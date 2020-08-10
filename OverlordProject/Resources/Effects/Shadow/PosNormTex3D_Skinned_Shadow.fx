float4x4 gWorld : WORLD;
float4x4 gView : VIEW;
float4x4 gViewInverse : VIEWINVERSE;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION; 
float4x4 gWorldViewProj_Light;
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);
float gShadowMapBias = 0.01f;
float4x4 gBones[70];

Texture2D gDiffuseMap;
Texture2D gShadowMap;

//NORMAL MAPPING
//**************
Texture2D gTextureNormal;

SamplerComparisonState cmpSampler
{
	// sampler state
	Filter = COMPARISON_MIN_MAG_MIP_LINEAR;
	AddressU = MIRROR;
	AddressV = MIRROR;

	// sampler comparison state
	ComparisonFunc = LESS_EQUAL;
};

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;// or Mirror or Clamp or Border
    AddressV = Wrap;// or Mirror or Clamp or Border
};

SamplerState gPointSamplerState
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = WRAP;
    AddressV = WRAP;
};

struct VS_INPUT
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
    float3 tangent : TANGENT;
	float2 texCoord : TEXCOORD;
	float4 BoneIndices : BLENDINDICES;
	float4 BoneWeights : BLENDWEIGHTS;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
    float3 tangent : TANGENT;
	float2 texCoord : TEXCOORD;
	float4 lPos : TEXCOORD1;
};

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

RasterizerState NoCulling
{
	CullMode = NONE;
};



float3 CalculateNormal(float3 tangent, float3 normal, float2 texCoord)
{
	//Normalmapping Logic
    float3 finalNormal = normal;

        float3 newNormal = normalize(normal);
        float3 newTangent = normalize(tangent);
        float3 biNormal = cross(tangent, normal);
        biNormal = normalize(biNormal);
        float3x3 localAxis = float3x3(tangent, biNormal, normal);
        float3 sampledNormal = gTextureNormal.Sample(gPointSamplerState, texCoord);
        sampledNormal = mul(sampledNormal, 2) - 1;
        sampledNormal = normalize(sampledNormal);
        finalNormal = normalize(mul(sampledNormal, localAxis));
	
    return finalNormal;
}


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

    
    //SKINNING
    //********
    float4 originalPosition = float4(input.pos, 1);
    float4 transformedPosition = 0;
    for (int i = 0; i < 4; i++)
    {
        float currentBoneIndex = input.BoneIndices[i];
        if (currentBoneIndex > -1)
        {
            transformedPosition += input.BoneWeights[i] * mul(originalPosition, gBones[currentBoneIndex]);

        }
        transformedPosition.w = 1;
    }
    output.lPos = mul(transformedPosition, gWorldViewProj_Light);

    float4 originalPosition2 = float4(input.pos, 1);
	float4 transformedPosition2 = 0;
	float3 transformedNormal = 0;

	//Skinning Magic...
    for (int idx{}; idx < 4; ++idx)
    {
        float index = input.BoneIndices[idx];
        if (index > -1)
        {
            transformedPosition += input.BoneWeights[idx] * mul(originalPosition, gBones[index]);
            transformedNormal += input.BoneWeights[idx] * mul(float4(input.normal,0), gBones[index]);
        }

    }
    output.normal = normalize(mul(transformedNormal, (float3x3) gWorld));
    output.tangent = mul(input.tangent, (float3x3) gWorld);
    output.texCoord = input.texCoord;
    output.pos = mul(transformedPosition, gWorldViewProj);
    return output;
}

float2 texOffset(int u, int v)
{
	//TODO: return offseted value (our shadow map has the following dimensions: 1280 * 720)
    return float2(u * 1.0f / 1280.f, v * 1.0f / 720.f);
}

float EvaluateShadowMap(float4 lpos)
{
	//TODO: complete
    lpos.xyz / lpos.w;
    if (lpos.x < -1.0f || lpos.x > 1.0f ||
        lpos.y < -1.0f || lpos.y > 1.0f ||
        lpos.z < 0.0f || lpos.z > 1.0f)
        return 0.0f;

    lpos.z -= gShadowMapBias;
    lpos.x = lpos.x / 2 + 0.5f;
    lpos.y = lpos.y / -2 + 0.5f;

    float sum = 0;
    float x, y;

    for (y = -1.5f; y < 1.5; y += 1.0f)
    {
        for (x = -1.5f; x < 1.5; x += 1.0f)
        {
            sum += gShadowMap.SampleCmpLevelZero(cmpSampler, lpos.xy + texOffset(x, y), lpos.z);
        }
    }
    
    float shadowFactor = sum / 16.0f;
    return shadowFactor;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET
    {
    
    
    input.normal = normalize(input.normal);
    input.tangent = normalize(input.tangent);
    
    //NORMAL
    float3 newNormal = CalculateNormal(input.tangent, input.normal, input.texCoord);
    
	float shadowValue = EvaluateShadowMap(input.lPos);

	float4 diffuseColor = gDiffuseMap.Sample( samLinear,input.texCoord );
	float3 color_rgb= diffuseColor.rgb;
	float color_a = diffuseColor.a;
	
	//HalfLambert Diffuse :)
	float diffuseStrength = dot(newNormal, -gLightDirection);
	diffuseStrength = diffuseStrength * 0.5 + 0.5;
	diffuseStrength = saturate(diffuseStrength);
	color_rgb = color_rgb * diffuseStrength;

	return float4( color_rgb * shadowValue, color_a );
}

//--------------------------------------------------------------------------------------
// Technique
//--------------------------------------------------------------------------------------
technique11 Default
{
    pass P0
    {
		SetRasterizerState(NoCulling);
		SetDepthStencilState(EnableDepth, 0);

		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

