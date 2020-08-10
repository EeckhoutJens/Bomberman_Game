#include "stdafx.h"
#include "SpriteFontLoader.h"
#include "BinaryReader.h"
#include "ContentManager.h"
#include "TextureData.h"

SpriteFont* SpriteFontLoader::LoadContent(const std::wstring& assetFile)
{
	auto pBinReader = new BinaryReader();
	pBinReader->Open(assetFile);

	if (!pBinReader->Exists())
	{
		Logger::LogFormat(LogLevel::Error, L"SpriteFontLoader::LoadContent > Failed to read the assetFile!\nPath: \'%s\'", assetFile.c_str());
		return nullptr;
	}

	//See BMFont Documentation for Binary Layout

	//Parse the Identification bytes (B,M,F)
	char b;
	char m;
	char f;

	b = pBinReader->Read<char>();
	m = pBinReader->Read<char>();
	f = pBinReader->Read<char>();

	//If Identification bytes doesn't match B|M|F,
	//Log Error (SpriteFontLoader::LoadContent > Not a valid .fnt font) &
	//return nullptr
	//...
	if ( b != 'B' || m!= 'M' || f != 'F' )
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Not a valid .fnt font ");
		return nullptr;
	}
	
	//Parse the version (version 3 required)
	char version;
	version = pBinReader->Read<char>();

	//If version is < 3,
	//Log Error (SpriteFontLoader::LoadContent > Only .fnt version 3 is supported)
	//return nullptr
	//...
	if (version < 3)
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Only .fnt version 3 is supported");
		return nullptr;
	}


	//Valid .fnt file
	auto pSpriteFont = new SpriteFont();
	//SpriteFontLoader is a friend class of SpriteFont
	//That means you have access to its privates (pSpriteFont->m_FontName = ... is valid)

	//**********
	// BLOCK 0 *
	//**********
	//Retrieve the blockId and blockSize
	char blockID_1 = pBinReader->Read<char>();
	unsigned int blockSize_1 = pBinReader->Read<unsigned int>();

	if (blockID_1 == NULL || blockSize_1 == NULL)
	{
		Logger::LogWarning(L"SpriteFontLoader::LoadContent > BlockID_1 or BlockSize_1 was not correctly read");
	}

	//Retrieve the FontSize (will be -25, BMF bug) [SpriteFont::m_FontSize]
	unsigned short int fontSize = unsigned short int(pBinReader->Read<unsigned int>());

	//Move the binreader to the start of the FontName [BinaryReader::MoveBufferPosition(...) or you can set its position using BinaryReader::SetBufferPosition(...))
	pBinReader->MoveBufferPosition(12);

	//Retrieve the FontName [SpriteFont::m_FontName]
	std::wstring fontName = pBinReader->ReadNullString();
	pSpriteFont->m_FontName = fontName;
	pSpriteFont->m_FontSize = fontSize;


	//**********
	// BLOCK 1 *
	//**********
	//Retrieve the blockId and blockSize
	char blockID_2 = pBinReader->Read<char>();
	unsigned int blockSize_2 = pBinReader->Read<unsigned int>();

	if (blockID_2 == NULL || blockSize_2 == NULL)
	{
		Logger::LogWarning(L"SpriteFontLoader::LoadContent > BlockID_2 or BlockSize_2 was not correctly read");
	}

	//Retrieve Texture Width & Height [SpriteFont::m_TextureWidth/m_TextureHeight]
	pBinReader->MoveBufferPosition(4);
	unsigned short int width = pBinReader->Read<short int>();
	pSpriteFont->m_TextureWidth = width;
	unsigned short int height = pBinReader->Read<short int>();
	pSpriteFont->m_TextureHeight = height;

	//Retrieve PageCount
	unsigned short int pages = pBinReader->Read<short int>();

	//> if pagecount > 1
	//> Log Error (SpriteFontLoader::LoadContent > SpriteFont (.fnt): Only one texture per font allowed)
	if (pages > 1)
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > SpriteFont (.fnt): Only one texture per font allowed");
		return nullptr;
	}

	//Advance to Block2 (Move Reader)
	//...
	pBinReader->MoveBufferPosition(5);

	//**********
	// BLOCK 2 *
	//**********
	//Retrieve the blockId and blockSize
	char blockID_3 = pBinReader->Read<char>();
	unsigned int blockSize_3 = pBinReader->Read<unsigned int>();

	if (blockID_3 == NULL || blockSize_3 == NULL)
	{
		Logger::LogWarning(L"SpriteFontLoader::LoadContent > BlockID_3 or BlockSize_3 was not correctly read");
	}

	//Retrieve the PageName (store Local)
	std::wstring pageName = pBinReader->ReadNullString();

	//	> If PageName is empty
	//	> Log Error (SpriteFontLoader::LoadContent > SpriteFont (.fnt): Invalid Font Sprite [Empty])
	if (pageName.empty())
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > SpriteFont (.fnt): Invalid Font Sprite[Empty]");
	}

	//>Retrieve texture filepath from the assetFile path
	//> (ex. c:/Example/somefont.fnt => c:/Example/) [Have a look at: wstring::rfind()]
	//>Use path and PageName to load the texture using the ContentManager [SpriteFont::m_pTexture]
	//> (ex. c:/Example/ + 'PageName' => c:/Example/somefont_0.png)
	//...
	std::wstring key(L"./Resources/SpriteFonts/");
	std::size_t found = assetFile.find(key);
	std::wstring textureLocation = assetFile.substr(found, key.size());
	textureLocation.append(pageName);
	pSpriteFont->m_pTexture = ContentManager::Load<TextureData>(textureLocation);

	//**********
	// BLOCK 3 *
	//**********
	//Retrieve the blockId and blockSize
	char blockID_4 = pBinReader->Read<char>();
	unsigned int blockSize_4 = pBinReader->Read<unsigned int>();

	if (blockID_4 == NULL || blockSize_4 == NULL)
	{
		Logger::LogWarning(L"SpriteFontLoader::LoadContent > BlockID_4 or BlockSize_4 was not correctly read");
	}

	//Retrieve Character Count (see documentation)
	int characterCount = blockSize_4 / 20;
	//Retrieve Every Character, For every Character:
	for (int i{}; i < characterCount; ++i)
	{
		//> Retrieve CharacterId (store Local) and cast to a 'wchar_t'
		wchar_t CharacterID = wchar_t(pBinReader->Read<long>());

		//> Check if CharacterId is valid (SpriteFont::IsCharValid), Log Warning and advance to next character if not valid
		if (!pSpriteFont->IsCharValid(CharacterID))
		{
			Logger::LogWarning(L"SpriteFontLoader::LoadContent > invalid character detected");
			continue;
		}

		//> Retrieve the corresponding FontMetric (SpriteFont::GetMetric) [REFERENCE!!!]
		//> Set IsValid to true [FontMetric::IsValid]
		//> Set Character (CharacterId) [FontMetric::Character]
		pSpriteFont->GetMetric(CharacterID).IsValid = true;
		pSpriteFont->GetMetric(CharacterID).Character = CharacterID;
		
		// > Retrieve Xposition(store Local)
		short int xPos = pBinReader->Read<short int>();

		//> Retrieve Yposition (store Local)
		short int yPos = pBinReader->Read<short int>();

		//> Retrieve & Set Width [FontMetric::Width]
		unsigned short int charWidth = pBinReader->Read<unsigned short int>();
		pSpriteFont->GetMetric(CharacterID).Width = charWidth;

		//> Retrieve & Set Height [FontMetric::Height]
		unsigned short int charHeight = pBinReader->Read<unsigned short int>();
		pSpriteFont->GetMetric(CharacterID).Height = charHeight;

		//> Retrieve & Set OffsetX [FontMetric::OffsetX]
		short int xOffset = pBinReader->Read<short int>();
		pSpriteFont->GetMetric(CharacterID).OffsetX = xOffset;

		//> Retrieve & Set OffsetY [FontMetric::OffsetY]
		short int yOffset = pBinReader->Read<short int>();
		pSpriteFont->GetMetric(CharacterID).OffsetY = yOffset;

		//> Retrieve & Set AdvanceX [FontMetric::AdvanceX]
		short int advanceX = pBinReader->Read<short int>();
		pSpriteFont->GetMetric(CharacterID).AdvanceX = advanceX;

		//> Retrieve & Set Page [FontMetric::Page]
		char page = pBinReader->Read<char>();
		pSpriteFont->GetMetric(CharacterID).Page = page;

		//> Retrieve Channel (BITFIELD!!!) 
		//> See documentation for BitField meaning [FontMetrix::Channel]
		switch (pBinReader->Read<char>())
		{
		case 1: //Blue
			pSpriteFont->GetMetric(CharacterID).Channel = 2;
			break;

		case 2:
			pSpriteFont->GetMetric(CharacterID).Channel = 1;
			break;

		case 3:
			pSpriteFont->GetMetric(CharacterID).Channel = 0;
			break;
		}

		//> Calculate Texture Coordinates using Xposition, Yposition, TextureWidth & TextureHeight [FontMetric::TexCoord]
		pSpriteFont->GetMetric(CharacterID).TexCoord.x = float(xPos) / float(width);
		pSpriteFont->GetMetric(CharacterID).TexCoord.y = float(yPos) / float(height);
	}

	//DONE :)

	delete pBinReader;
	return pSpriteFont;
}

void SpriteFontLoader::Destroy(SpriteFont* objToDestroy)
{
	SafeDelete(objToDestroy);
}
