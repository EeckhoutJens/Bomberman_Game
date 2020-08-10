#include "stdafx.h"
#include "ModelAnimator.h"

ModelAnimator::ModelAnimator(MeshFilter* pMeshFilter):
m_pMeshFilter(pMeshFilter),
m_Transforms(std::vector<DirectX::XMFLOAT4X4>()),
m_IsPlaying(false), 
m_Reversed(false),
m_ClipSet(false),
m_TickCount(0),
m_AnimationSpeed(1.0f)
{
	SetAnimation(0);
}

void ModelAnimator::SetAnimation(UINT clipNumber, bool playOnce)
{
	//TODO: complete
	//Set m_ClipSet to false
	//Check if clipNumber is smaller than the actual m_AnimationClips vector size
	//If not,
		//	Call Reset
		//	Log a warning with an appropriate message
		//	return
	//else
		//	Retrieve the AnimationClip from the m_AnimationClips vector based on the given clipNumber
		//	Call SetAnimation(AnimationClip clip)
	m_ClipSet = false;
	if (!(clipNumber < m_pMeshFilter->m_AnimationClips.size()))
	{
		Reset();
		Logger::LogWarning(L"OVERLORD ENGINE>MODELANIMATOR>SetAnimation: Clipnumber is larger than the size of the animationclip vector");
		return;
	}
	AnimationClip clip = m_pMeshFilter->m_AnimationClips[clipNumber];
	SetAnimation(clip,playOnce);
}

void ModelAnimator::SetAnimation(std::wstring clipName, bool playOnce)
{
	//TODO: complete
	//Set m_ClipSet to false
	m_ClipSet = false;
	bool foundClip{ false };
	AnimationClip clip{};
	//Iterate the m_AnimationClips vector and search for an AnimationClip with the given name (clipName)
	for (int i{}; i < int(m_pMeshFilter->m_AnimationClips.size()); ++i)
	{
		if (clipName == m_pMeshFilter->m_AnimationClips[i].Name)
		{
			foundClip = true;
			clip = m_pMeshFilter->m_AnimationClips[i];
			break;
		}
	}

	if (foundClip)
	{
		SetAnimation(clip,playOnce);
	}
	else
	{
		Reset();
		Logger::LogWarning(L"OVERLORD ENGINE => MODELANIMATOR => SetAnimation: Given clipname was not found");
	}
}

void ModelAnimator::SetAnimation(AnimationClip clip, bool playOnce)
{
	//TODO: complete
	//Set m_ClipSet to true
	m_ClipSet = true;
	//Set m_CurrentClip
	m_CurrentClip = clip;
	//Call Reset(false)
	Reset(false);
	//Set PlayOnce
	m_PlayOnce = playOnce;
}

void ModelAnimator::Reset(bool pause)
{
	//TODO: complete
	//If pause is true, set m_IsPlaying to false
	if (pause)
	{
		m_IsPlaying = false;
	}
	//Set m_TickCount to zero
	m_TickCount = 0;
	//Set m_AnimationSpeed to 1.0f
	m_AnimationSpeed = 1.0f;
	//If m_ClipSet is true
	if (m_ClipSet)
	{
		std::vector<DirectX::XMFLOAT4X4>::iterator it{ m_CurrentClip.Keys[0].BoneTransforms.begin() };
		m_Transforms.assign(m_CurrentClip.Keys[0].BoneTransforms.begin(), m_CurrentClip.Keys[0].BoneTransforms.end());
	}
	else
	{
		DirectX::XMMATRIX matrix{ DirectX::XMMatrixIdentity() };
		DirectX::XMFLOAT4X4 convertMatrix{};
		DirectX::XMStoreFloat4x4(&convertMatrix, matrix);
		m_Transforms.assign(m_pMeshFilter->m_BoneCount, convertMatrix);
	}
	//	Retrieve the BoneTransform from the first Key from the current clip (m_CurrentClip)
	//	Refill the m_Transforms vector with the new BoneTransforms (have a look at vector::assign)
	//Else
	//	Create an IdentityMatrix 
	//	Refill the m_Transforms vector with this IdenityMatrix (Amount = BoneCount) (have a look at vector::assign)
}

void ModelAnimator::Update(const GameContext& gameContext)
{
	//TODO: complete
	//We only update the transforms if the animation is running and the clip is set
	if (m_IsPlaying && m_ClipSet)
	{
		//1. 
		//Calculate the passedTicks (see the lab document)
		auto passedTicks = gameContext.pGameTime->GetElapsed() * m_CurrentClip.TicksPerSecond * m_AnimationSpeed;
		//Make sure that the passedTicks stay between the m_CurrentClip.Duration bounds (fmod)
		passedTicks = fmod(passedTicks, m_CurrentClip.Duration);

		//2. 
		//IF m_Reversed is true
		if (m_Reversed)
		{
			m_TickCount -= passedTicks;
			if (m_TickCount < 0)
			{
				if (m_PlayOnce)
				{
					m_PlayOnce = false;
					return;
				}
				m_TickCount += m_CurrentClip.Duration;
			}
		}
		else
		{
			m_TickCount += passedTicks;
			if (m_TickCount > m_CurrentClip.Duration)
			{
				if (m_PlayOnce)
				{
					m_PlayOnce = false;
					return;
				}
				m_TickCount -= m_CurrentClip.Duration;
			}
		}
		//	Subtract passedTicks from m_TickCount
		//	If m_TickCount is smaller than zero, add m_CurrentClip.Duration to m_TickCount
		//ELSE
		//	Add passedTicks to m_TickCount
		//	if m_TickCount is bigger than the clip duration, subtract the duration from m_TickCount

		//3.
		//Find the enclosing keys
		AnimationKey keyA{}, keyB{};
		//Iterate all the keys of the clip and find the following keys:
		for (int i{}; i < int(m_CurrentClip.Keys.size()); ++i)
		{
			if (m_CurrentClip.Keys[i].Tick < m_TickCount)
			{
				if (m_CurrentClip.Keys[i].Tick > keyA.Tick || keyA.Tick == 0)
				{
					keyA = m_CurrentClip.Keys[i];
				}
			}

			if (m_CurrentClip.Keys[i].Tick > m_TickCount)
			{
				if (m_CurrentClip.Keys[i].Tick < keyB.Tick || keyB.Tick == 0)
				{
					keyB = m_CurrentClip.Keys[i];
				}
			}
		}
		//keyA > Closest Key with Tick before/smaller than m_TickCount
		//keyB > Closest Key with Tick after/bigger than m_TickCount

		//4.
		//Interpolate between keys
		//Figure out the BlendFactor (See lab document)
		float blendKeyA{};
		blendKeyA = (m_TickCount - keyA.Tick) / (keyB.Tick - keyA.Tick);
		//Clear the m_Transforms vector
		m_Transforms.clear();
		//FOR every boneTransform in a key (So for every bone)
		for (int i{}; i < int(keyA.BoneTransforms.size()); ++i)
		{
			DirectX::XMFLOAT4X4 transformA;
			DirectX::XMFLOAT4X4 transformB;
			if (!keyA.BoneTransforms.empty())
			{
				transformA = keyA.BoneTransforms[i];
				if (!keyB.BoneTransforms.empty())
				{
					transformB = keyB.BoneTransforms[i];
					DirectX::XMMATRIX transA = DirectX::XMLoadFloat4x4(&transformA);
					DirectX::XMVECTOR scaleA;
					DirectX::XMVECTOR rotationA;
					DirectX::XMVECTOR translateA;
					DirectX::XMMatrixDecompose(&scaleA, &rotationA, &translateA, transA);

					DirectX::XMMATRIX transB = DirectX::XMLoadFloat4x4(&transformB);
					DirectX::XMVECTOR scaleB;
					DirectX::XMVECTOR rotationB;
					DirectX::XMVECTOR translateB;
					DirectX::XMMatrixDecompose(&scaleB, &rotationB, &translateB, transB);

					DirectX::XMVECTOR lerpedScale = DirectX::XMVectorLerp(scaleA, scaleB, blendKeyA);
					DirectX::XMVECTOR lerpedTranslate = DirectX::XMVectorLerp(translateA, translateB, blendKeyA);
					DirectX::XMVECTOR lerpedRotation = DirectX::XMQuaternionSlerp(rotationA, rotationB, blendKeyA);

					DirectX::XMMATRIX finalScale = DirectX::XMMatrixScalingFromVector(lerpedScale);
					DirectX::XMMATRIX finalRotation = DirectX::XMMatrixRotationQuaternion(lerpedRotation);
					DirectX::XMMATRIX finalTranslate = DirectX::XMMatrixTranslationFromVector(lerpedTranslate);

					DirectX::XMMATRIX result = finalScale * finalRotation * finalTranslate;

					DirectX::XMFLOAT4X4 storedResult;
					DirectX::XMStoreFloat4x4(&storedResult, result);
					m_Transforms.push_back(storedResult);
				}
					

				
			}
			
		}
	}
}
