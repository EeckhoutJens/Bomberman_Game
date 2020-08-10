#pragma once
#include <PostProcessingMaterial.h>
class ID3D11EffectShaderResourceVariable;
class TextureData;
class PostColorGrading final : public PostProcessingMaterial
{
public:
	PostColorGrading();
	virtual ~PostColorGrading() = default;

	PostColorGrading(const PostColorGrading& other) = delete;
	PostColorGrading(PostColorGrading&& other) noexcept = delete;
	PostColorGrading& operator=(const PostColorGrading& other) = delete;
	PostColorGrading& operator=(PostColorGrading&& other) noexcept = delete;

	void SetLUTTexture(const std::wstring& assetFile);

protected:
	void LoadEffectVariables() override;
	void UpdateEffectVariables(RenderTarget * pRendertarget) override;

private:
	ID3DX11EffectShaderResourceVariable* m_pTextureMapVariable;
	ID3DX11EffectShaderResourceVariable* m_pLUTTextureVariable;
	TextureData* m_pLUTTexture;
};

