//Color Grading Shader with LUTs, TheKosmonaut 2017 ( kosmonaut3d@googlemail.com )

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  VARIABLES
////////////////////////////////////////////////////////////////////////////////////////////////////////////

float Size = 16;
float SizeRoot = 4;

Texture2D InputTexture;
Texture2D LUT;

SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Mirror;
    AddressV = Mirror;
};

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


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  STRUCTS
////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct VS_INPUT
{
    float3 Position : POSITION0;
    float2 TexCoord : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  VERTEX SHADER
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Only needs float2 position as input.
PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;

    output.Position = float4(input.Position, 1);
    output.TexCoord = input.TexCoord;

    return output;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  PIXEL SHADER
////////////////////////////////////////////////////////////////////////////////////////////////////////////

float4 PS(PS_INPUT input) : SV_Target
{

	//Our input
    float4 baseTexture = InputTexture.Load(int3(input.Position.xy, 0));

	//Manual trilinear interpolation

	//We need to clamp since our values go, for example, from 0 to 15. But with a red value of 1.0 we would get 16, which is on the next table already.

	//OBSOLETE: We also need to shift half a pixel to the left, since our sampling locations do not match the storage location (see CreateLUT)
	//float halfOffset = 0.5f;

    float red = baseTexture.r * (Size - 1);

    float redinterpol = frac(red);

    float green = baseTexture.g * (Size - 1);
    float greeninterpol = frac(green);

    float blue = baseTexture.b * (Size - 1);
    float blueinterpol = frac(blue);

	//Blue base value

    float row = trunc(blue / SizeRoot);
    float col = trunc(blue % SizeRoot);

    float2 blueBaseTable = float2(trunc(col * Size), trunc(row * Size));

    float4 b0r1g0;
    float4 b0r0g1;
    float4 b0r1g1;
    float4 b1r0g0;
    float4 b1r1g0;
    float4 b1r0g1;
    float4 b1r1g1;

	/*
	We need to read 8 values (like in a 3d LUT) and interpolate between them.
	This cannot be done with default hardware filtering so I am doing it manually.
	Note that we must not interpolate when on the borders of tables!
	*/

	//Red 0 and 1, Green 0

    float4 b0r0g0 = LUT.Load(int3(blueBaseTable.x + red, blueBaseTable.y + green, 0));

	[branch]
    if (red < Size - 1)
        b0r1g0 = LUT.Load(int3(blueBaseTable.x + red + 1, blueBaseTable.y + green, 0));
    else
        b0r1g0 = b0r0g0;

	// Green 1

	[branch]
    if (green < Size - 1)
    {
		//Red 0 and 1

        b0r0g1 = LUT.Load(int3(blueBaseTable.x + red, blueBaseTable.y + green + 1, 0));

		[branch]
        if (red < Size - 1)
            b0r1g1 = LUT.Load(int3(blueBaseTable.x + red + 1, blueBaseTable.y + green + 1, 0));
        else
            b0r1g1 = b0r0g1;
    }
    else
    {
        b0r0g1 = b0r0g0;
        b0r1g1 = b0r0g1;
    }

	[branch]
    if (blue < Size - 1)
    {
        blue += 1;
        row = trunc(blue / SizeRoot);
        col = trunc(blue % SizeRoot);

        blueBaseTable = float2(trunc(col * Size), trunc(row * Size));

        b1r0g0 = LUT.Load(int3(blueBaseTable.x + red, blueBaseTable.y + green, 0));

		[branch]
        if (red < Size - 1)
            b1r1g0 = LUT.Load(int3(blueBaseTable.x + red + 1, blueBaseTable.y + green, 0));
        else
            b1r1g0 = b0r0g0;

		// Green 1

		[branch]
        if (green < Size - 1)
        {
			//Red 0 and 1

            b1r0g1 = LUT.Load(int3(blueBaseTable.x + red, blueBaseTable.y + green + 1, 0));

			[branch]
            if (red < Size - 1)
                b1r1g1 = LUT.Load(int3(blueBaseTable.x + red + 1, blueBaseTable.y + green + 1, 0));
            else
                b1r1g1 = b0r0g1;
        }
        else
        {
            b1r0g1 = b0r0g0;
            b1r1g1 = b0r0g1;
        }
    }
    else
    {
        b1r0g0 = b0r0g0;
        b1r1g0 = b0r1g0;
        b1r0g1 = b0r0g0;
        b1r1g1 = b0r1g1;
    }

    float4 result = lerp(lerp(b0r0g0, b0r1g0, redinterpol), lerp(b0r0g1, b0r1g1, redinterpol), greeninterpol);
    float4 result2 = lerp(lerp(b1r0g0, b1r1g0, redinterpol), lerp(b1r0g1, b1r1g1, redinterpol), greeninterpol);

    result = lerp(result, result2, blueinterpol);

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  TECHNIQUES
////////////////////////////////////////////////////////////////////////////////////////////////////////////

technique11 ApplyLUT
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_4_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PS()));
        SetRasterizerState(Culling);
        SetDepthStencilState(DepthWriting, 0);
    }
}
