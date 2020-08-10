#include "stdafx.h"
#include "UberMaterial.h"
#include "ContentManager.h"
#include "TextureData.h"

ID3DX11EffectShaderResourceVariable* UberMaterial::m_pDiffuseSRVvariable = nullptr;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pEnvironmentSRVvariable = nullptr;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pNormalMappingSRVvariable = nullptr;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pOpacitySRVvariable = nullptr;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pSpecularLevelSRVvariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pReflectionStrengthVariable{};
ID3DX11EffectScalarVariable* UberMaterial::m_pRefractionStrengthVariable{};
ID3DX11EffectScalarVariable* UberMaterial::m_pUseBlinnVariable{};
ID3DX11EffectScalarVariable* UberMaterial::m_pUseDiffuseTextureVariable{};
ID3DX11EffectScalarVariable* UberMaterial::m_pUseEnvironmentMappingVariable{};
ID3DX11EffectScalarVariable* UberMaterial::m_pUseFresnelFalloffVariable{};
ID3DX11EffectScalarVariable* UberMaterial::m_pUseNormalMappingVariable{};
ID3DX11EffectScalarVariable* UberMaterial::m_pUseOpacityMapVariable{};
ID3DX11EffectScalarVariable* UberMaterial::m_pUsePhongVariable{};
ID3DX11EffectScalarVariable* UberMaterial::m_pUseSpecularLevelTextureVariable{};
ID3DX11EffectScalarVariable* UberMaterial::m_pShininessVariable{};
ID3DX11EffectScalarVariable* UberMaterial::m_pAmbientIntensityVariable{};
ID3DX11EffectScalarVariable* UberMaterial::m_pFlipGreenChannelVariable{};
ID3DX11EffectScalarVariable* UberMaterial::m_pFresnelHardnessVariable{};
ID3DX11EffectScalarVariable* UberMaterial::m_pRefractionIndexVariable{};
ID3DX11EffectScalarVariable* UberMaterial::m_pFresnelMultiplierVariable{};
ID3DX11EffectScalarVariable* UberMaterial::m_pOpacityVariable{};
ID3DX11EffectScalarVariable* UberMaterial::m_pFresnelPowerVariable{};
ID3DX11EffectVectorVariable* UberMaterial::m_pAmbientColorVariable{};
ID3DX11EffectVectorVariable* UberMaterial::m_pDiffuseColorVariable{};
ID3DX11EffectVectorVariable* UberMaterial::m_pFresnelColorVariable{};
ID3DX11EffectVectorVariable* UberMaterial::m_pLightDirectionVariable{};
ID3DX11EffectVectorVariable* UberMaterial::m_pSpecularColorVariable{};

UberMaterial::UberMaterial() : Material(L"./Resources/Effects/UberShader.fx")
{

}

UberMaterial::~UberMaterial()
{
}

void UberMaterial::SetLightDirection(DirectX::XMFLOAT3 direction)
{
	m_LightDirection = direction;
	
}

void UberMaterial::EnableDiffuseTexture(bool enable)
{
	m_bDiffuseTexture = enable;
}

void UberMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
}

void UberMaterial::SetDiffuseColor(DirectX::XMFLOAT4 color)
{
	m_ColorDiffuse = color;
}

void UberMaterial::SetSpecularColor(DirectX::XMFLOAT4 color)
{
	m_ColorSpecular = color;
}

void UberMaterial::EnableSpecularLevelTexture(bool enable)
{
	m_bSpecularLevelTexture = enable;
}

void UberMaterial::SetSpecularLevelTexture(const std::wstring& assetFile)
{
	m_pSpecularLevelTexture = ContentManager::Load<TextureData>(assetFile);
}

void UberMaterial::SetShininess(int shininess)
{
	m_Shininess = shininess;
}

void UberMaterial::SetAmbientColor(DirectX::XMFLOAT4 color)
{
	m_ColorAmbient = color;
}

void UberMaterial::SetAmbientIntensity(float intensity)
{
	m_AmbientIntensity = intensity;
}

void UberMaterial::FlipNormalGreenCHannel(bool flip)
{
	m_bFlipGreenChannel = flip;
}

void UberMaterial::EnableNormalMapping(bool enable)
{
	m_bNormalMapping = enable;
}

void UberMaterial::SetNormalMapTexture(const std::wstring& assetFile)
{
	m_pNormalMappingTexture = ContentManager::Load<TextureData>(assetFile);
}

void UberMaterial::EnableEnvironmentMapping(bool enable)
{
	m_bEnvironmentMapping = enable;
}

void UberMaterial::SetEnvironmentCube(const std::wstring& assetFile)
{
	m_pEnvironmentCube = ContentManager::Load<TextureData>(assetFile);
}

void UberMaterial::SetReflectionStrength(float strength)
{
	m_ReflectionStrength = strength;
}

void UberMaterial::SetRefractionStrength(float strength)
{
	m_RefractionStrength = strength;
}

void UberMaterial::SetRefractionIndex(float index)
{
	m_RefractionIndex = index;
}

void UberMaterial::SetOpacity(float opacity)
{
	m_Opacity = opacity;
}

void UberMaterial::EnableOpacityMap(bool enable)
{
	m_bOpacityMap = enable;
}

void UberMaterial::SetOpacityTexture(const std::wstring& assetFile)
{
	m_pOpacityMap = ContentManager::Load<TextureData>(assetFile);
}

void UberMaterial::EnableSpecularBlinn(bool enable)
{
	m_bSpecularBlinn = enable;
}

void UberMaterial::EnableSpecularPhong(bool enable)
{
	m_bSpecularPhong = enable;
}

void UberMaterial::EnableFresnelFaloff(bool enable)
{
	m_bFresnelFaloff = enable;
}

void UberMaterial::SetFresnelColor(DirectX::XMFLOAT4 color)
{
	m_ColorFresnel = color;
}

void UberMaterial::SetFresnelPower(float power)
{
	m_FresnelPower = power;
}

void UberMaterial::SetFresnelMultiplier(float multiplier)
{
	m_FresnelMultiplier = multiplier;
}

void UberMaterial::SetFresnelHardness(float hardness)
{
	m_FresnelHardness = hardness;
}

void UberMaterial::LoadEffectVariables()
{
	//Light
	m_pLightDirectionVariable = GetEffect()->GetVariableByName("gLightDirection")->AsVector();

	//Diffuse
	m_pUseDiffuseTextureVariable = GetEffect()->GetVariableByName("gUseTextureDiffuse")->AsScalar();
	m_pDiffuseSRVvariable = GetEffect()->GetVariableByName("gTextureDiffuse")->AsShaderResource();
	m_pDiffuseColorVariable = GetEffect()->GetVariableByName("gColorDiffuse")->AsVector();

	//Specular
	m_pSpecularColorVariable = GetEffect()->GetVariableByName("gColorSpecular")->AsVector();
	m_pUseSpecularLevelTextureVariable = GetEffect()->GetVariableByName("gUseTextureSpecularIntensity")->AsScalar();
	m_pSpecularLevelSRVvariable = GetEffect()->GetVariableByName("gTextureSpecularIntensity")->AsShaderResource();
	m_pShininessVariable = GetEffect()->GetVariableByName("gShininess")->AsScalar();

	//Ambient
	m_pAmbientColorVariable = GetEffect()->GetVariableByName("gColorAmbient")->AsVector();
	m_pAmbientIntensityVariable = GetEffect()->GetVariableByName("gAmbientIntensity")->AsScalar();

	//Normal Mapping
	m_pUseNormalMappingVariable = GetEffect()->GetVariableByName("gUseTextureNormal")->AsScalar();
	m_pNormalMappingSRVvariable = GetEffect()->GetVariableByName("gTextureNormal")->AsShaderResource();
	m_pFlipGreenChannelVariable = GetEffect()->GetVariableByName("gFlipGreenChannel")->AsScalar();

	//Environment Mapping
	m_pUseEnvironmentMappingVariable = GetEffect()->GetVariableByName("gUseTextureEnvironment")->AsScalar();
	m_pEnvironmentSRVvariable = GetEffect()->GetVariableByName("gCubeEnvironment")->AsShaderResource();
	m_pReflectionStrengthVariable = GetEffect()->GetVariableByName("gReflectionStrength")->AsScalar();
	m_pRefractionStrengthVariable = GetEffect()->GetVariableByName("gRefractionStrength")->AsScalar();
	m_pRefractionIndexVariable = GetEffect()->GetVariableByName("gRefractionIndex")->AsScalar();

	//Opacity
	m_pOpacityVariable = GetEffect()->GetVariableByName("gOpacityIntensity")->AsScalar();
	m_pOpacitySRVvariable = GetEffect()->GetVariableByName("gTextureOpacity")->AsShaderResource();
	m_pUseOpacityMapVariable = GetEffect()->GetVariableByName("gUseTextureOpacity")->AsScalar();

	//Specular Model
	m_pUseBlinnVariable = GetEffect()->GetVariableByName("gUseSpecularBlinn")->AsScalar();
	m_pUsePhongVariable = GetEffect()->GetVariableByName("gUseSpecularPhong")->AsScalar();

	//Fresnel Falloff
	m_pUseFresnelFalloffVariable = GetEffect()->GetVariableByName("gUseFresnelFalloff")->AsScalar();
	m_pFresnelColorVariable = GetEffect()->GetVariableByName("gColorFresnel")->AsVector();
	m_pFresnelHardnessVariable = GetEffect()->GetVariableByName("gFresnelHardness")->AsScalar();
	m_pFresnelMultiplierVariable = GetEffect()->GetVariableByName("gFresnelMultiplier")->AsScalar();
	m_pFresnelPowerVariable = GetEffect()->GetVariableByName("gFresnelPower")->AsScalar();
}

void UberMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);

	//Light Direction

	const float* pLight{};
	pLight = new float[3]{ m_LightDirection.x,m_LightDirection.y,m_LightDirection.z };
	m_pLightDirectionVariable->SetFloatVector(pLight);

	

	//Diffuse
	m_pUseDiffuseTextureVariable->SetBool(m_bDiffuseTexture);
	
	const float* pDiffuseColor;
	pDiffuseColor = new float[4]{ m_ColorDiffuse.x,m_ColorDiffuse.y,m_ColorDiffuse.z,m_ColorDiffuse.w };
	m_pDiffuseColorVariable->SetFloatVector(pDiffuseColor);

	if (m_pDiffuseTexture != nullptr)
	{
		m_pDiffuseSRVvariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());
	}

	//Specular
	m_pUseSpecularLevelTextureVariable->SetBool(m_bSpecularLevelTexture);

	const float* pSpecularColor;
	pSpecularColor = new float[4]{ m_ColorSpecular.x,m_ColorSpecular.y,m_ColorSpecular.z,m_ColorSpecular.w };
	m_pSpecularColorVariable->SetFloatVector(pSpecularColor);

	if(m_pSpecularLevelTexture != nullptr)
	{
		m_pSpecularLevelSRVvariable->SetResource(m_pSpecularLevelTexture->GetShaderResourceView());
	}
	m_pShininessVariable->SetInt(m_Shininess);


	//Ambient
	const float* pAmbientColor;
	pAmbientColor = new float[4]{ m_ColorAmbient.x,m_ColorAmbient.y,m_ColorAmbient.z,m_ColorAmbient.w };
	m_pAmbientColorVariable->SetFloatVector(pAmbientColor);
	m_pAmbientIntensityVariable->SetFloat(m_AmbientIntensity);


	//Normal Mapping
	m_pUseNormalMappingVariable->SetBool(m_bNormalMapping);
	if (m_pNormalMappingTexture != nullptr)
	{
		m_pNormalMappingSRVvariable->SetResource(m_pNormalMappingTexture->GetShaderResourceView());
	}
	m_pFlipGreenChannelVariable->SetBool(m_bFlipGreenChannel);


	//Environment Mapping
	m_pUseEnvironmentMappingVariable->SetBool(m_bEnvironmentMapping);
	if (m_pEnvironmentCube != nullptr)
	{
		m_pEnvironmentSRVvariable->SetResource(m_pEnvironmentCube->GetShaderResourceView());
	}
	m_pReflectionStrengthVariable->SetFloat(m_ReflectionStrength);
	m_pRefractionStrengthVariable->SetFloat(m_RefractionIndex);
	m_pRefractionIndexVariable->SetFloat(m_RefractionIndex);


	//Opacity
	m_pOpacityVariable->SetFloat(m_Opacity);
	m_pUseOpacityMapVariable->SetBool(m_bOpacityMap);
	if (m_pOpacityMap != nullptr)
	{
		m_pOpacitySRVvariable->SetResource(m_pOpacityMap->GetShaderResourceView());
	}
	


	//Specular Model
	m_pUseBlinnVariable->SetBool(m_bSpecularBlinn);
	m_pUsePhongVariable->SetBool(m_bSpecularPhong);


	//Fresnel Falloff
	m_pUseFresnelFalloffVariable->SetBool(m_bFresnelFaloff);

	const float* pFresnelColor;
	pFresnelColor = new float[4]{ m_ColorFresnel.x,m_ColorFresnel.y,m_ColorFresnel.z,m_ColorFresnel.w };
	m_pFresnelColorVariable->SetFloatVector(pFresnelColor);

	m_pFresnelHardnessVariable->SetFloat(m_FresnelHardness);
	m_pFresnelMultiplierVariable->SetFloat(m_FresnelMultiplier);
	m_pFresnelPowerVariable->SetFloat(m_FresnelPower);

	//Cleanup
	delete[] pAmbientColor;
	delete[] pDiffuseColor;
	delete[] pFresnelColor;
	delete[] pLight;
	delete[] pSpecularColor;
}
