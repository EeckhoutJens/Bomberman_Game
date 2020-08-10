#pragma once
#include <Material.h>
class TextureData;
class DiffuseMaterial : public Material
{
public:
	DiffuseMaterial(bool enableTransparency = false);
	~DiffuseMaterial() = default;

	void SetDiffuseTexture(const std::wstring& assetFile);
protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent);

private:
	TextureData* m_pDiffuseTexture{};
	static ID3DX11EffectShaderResourceVariable* m_pDiffuseSRVvariable;


	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	DiffuseMaterial(const DiffuseMaterial& obj);
	DiffuseMaterial& operator=(const DiffuseMaterial& obj);
};
