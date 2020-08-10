//=============================================================================
//// Shader uses position and texture
//=============================================================================
SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Mirror;
    AddressV = Mirror;
};

Texture2D gTexture;

/// Create Depth Stencil State (ENABLE DEPTH WRITING)
DepthStencilState DepthWriting
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;

};

/// Create Rasterizer State (Backface culling) 
RasterizerState Culling
{
    FillMode = SOLID;
    CullMode = BACK;
};

//IN/OUT STRUCTS
//--------------
struct VS_INPUT
{
    float3 Position : POSITION;
	float2 TexCoord : TEXCOORD0;

};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD1;
};


//VERTEX SHADER
//-------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	// Set the Position
    output.Position = float4(input.Position, 1);
	// Set the TexCoord
    output.TexCoord = input.TexCoord;
	
	return output;
}


//PIXEL SHADER
//------------
float4 PS(PS_INPUT input): SV_Target
{
	// Step 1: find the dimensions of the texture (the texture has a method for that)	
    float height;
    float width;
    float4 finalColor;
    gTexture.GetDimensions(width, height);
    
	// Step 2: calculate dx and dy (UV space for 1 pixel)	
    float dx = 1.f / width;
    float dy = 1.f / height;
    
	// Step 3: Create a double for loop (5 iterations each)
	//		   Inside the loop, calculate the offset in each direction. Make sure not to take every pixel but move by 2 pixels each time
	//			Do a texture lookup using your previously calculated uv coordinates + the offset, and add to the final color
    for (int i = 0; i < 5; ++i)
    {
        float xOffset = dx * 2;
        float yOffset = dy * 2;
        finalColor += gTexture.Sample(samPoint, float2(input.TexCoord.x + xOffset, input.TexCoord.y + yOffset));
        finalColor += gTexture.Sample(samPoint, float2(input.TexCoord.x - xOffset, input.TexCoord.y + yOffset));
        finalColor += gTexture.Sample(samPoint, float2(input.TexCoord.x + xOffset, input.TexCoord.y - yOffset));
        finalColor += gTexture.Sample(samPoint, float2(input.TexCoord.x - xOffset, input.TexCoord.y - yOffset));
        finalColor += gTexture.Sample(samPoint, float2(input.TexCoord.x, input.TexCoord.y));
    }
    
	// Step 4: Divide the final color by the number of passes (in this case 5*5)	
    finalColor /= 25.f;
    finalColor.w = 1.f;
	// Step 5: return the final color
    return finalColor;
       
}


//TECHNIQUE
//---------
technique11 Blur
{
    pass P0
    {
		// Set states...
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
        SetRasterizerState(Culling);
        SetDepthStencilState(DepthWriting, 0);
    }
}