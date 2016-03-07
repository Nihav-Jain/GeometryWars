#include "pch.h"
#include "XmlParseMaster.h"
#include "IXmlParseHelper.h"

namespace Library
{
	XmlParseMaster::XmlParseMaster(SharedData& sharedData) :
		mSharedData(&sharedData), mHelpers(), mLastClonedHelper(0), mIsCloned(false), mFileName(std::string()), mXmlParser(XML_ParserCreate(NULL))
	{
		mSharedData->SetXmlParseMaster(this);
		InitializeXmlParser();
	}

	XmlParseMaster::~XmlParseMaster()
	{
		if (mIsCloned)
		{
			delete mSharedData;
			for (std::uint32_t i = 0; i < mLastClonedHelper; i++)
			{
				delete mHelpers[i];
				mHelpers[i] = nullptr;
			}
		}
		XML_ParserFree(mXmlParser);
	}

	XmlParseMaster* XmlParseMaster::Clone()
	{
		SharedData* clonedSharedData = mSharedData->Clone();
		XmlParseMaster* clonedParseMaster = new XmlParseMaster(*clonedSharedData);

		for (std::uint32_t i = 0; i < mHelpers.Size(); i++)
		{
			clonedParseMaster->AddHelper(*(mHelpers[i]->Clone()));
		}
		clonedParseMaster->mIsCloned = true;
		clonedParseMaster->mLastClonedHelper = mHelpers.Size();

		return clonedParseMaster;
	}

	void XmlParseMaster::AddHelper(IXmlParseHelper& helper)
	{
		mHelpers.PushBack(&helper);
	}

	void XmlParseMaster::RemoveHelper(IXmlParseHelper& helper)
	{
		mHelpers.Remove(mHelpers.Find(&helper));
	}

	bool XmlParseMaster::Parse(const char* document, std::int32_t length, bool isLastChunk, bool isFirstChunk)
	{
		if (isFirstChunk)
		{
			XML_ParserReset(mXmlParser, NULL);
			InitializeXmlParser();

			for (auto helper : mHelpers)
			{
				helper->Initialize();
			}
		}
		XML_Status parseCode = XML_Parse(mXmlParser, document, length, isLastChunk);

		if (parseCode == XML_STATUS_ERROR || parseCode == XML_STATUS_SUSPENDED)
			return false;
		return true;
	}

	bool XmlParseMaster::ParseFromFile(const std::string& fileName)
	{
		mFileName = fileName;

		std::ifstream fileInputStream;
		fileInputStream.open(mFileName);

		if (!fileInputStream.is_open())
			throw std::exception("Error in opening file");

		bool isFirstChunk = true;
		std::string fileData;
		while (fileInputStream.good())
		{
			std::getline(fileInputStream, fileData);

			std::uint32_t fileLength = static_cast<std::uint32_t>(fileData.length());

			if (!Parse(fileData.c_str(), fileLength, !fileInputStream.good(), isFirstChunk))
			{
				fileInputStream.close();
				return false;
			}
			isFirstChunk = false;
		}
		fileInputStream.close();
		return true;
	}

	const std::string& XmlParseMaster::GetFileName() const
	{
		return mFileName;
	}

	XmlParseMaster::SharedData* XmlParseMaster::GetSharedData() const
	{
		return mSharedData;
	}

	void XmlParseMaster::SetSharedData(SharedData& sharedData)
	{
		mSharedData = &sharedData;
		mSharedData->SetXmlParseMaster(this);
	}

	void XmlParseMaster::StartElementHandler(void* userData, const char* elementName, const char** attributes)
	{
		XmlParseMaster* xmlParseMaster = reinterpret_cast<XmlParseMaster*>(userData);
		assert(xmlParseMaster != nullptr);

		Hashmap<std::string, std::string> attributeMap;
		xmlParseMaster->GetSharedData()->IncrementDepth();

		std::uint32_t i = 0;
		while (attributes[i] != '\0')
		{
			attributeMap[attributes[i]] = attributes[i + 1];
			i += 2;
		}

		for (i = 0; i < xmlParseMaster->mHelpers.Size(); i++)
		{
			if (xmlParseMaster->mHelpers[i]->StartElementHandler(*xmlParseMaster->GetSharedData(), elementName, attributeMap))
			{
				break;
			}
		}
	}

	void XmlParseMaster::EndElementHandler(void* userData, const char* elementName)
	{
		XmlParseMaster* xmlParseMaster = reinterpret_cast<XmlParseMaster*>(userData);
		assert(xmlParseMaster != nullptr);

		xmlParseMaster->GetSharedData()->DecrementDepth();
		for (std::uint32_t i = 0; i < xmlParseMaster->mHelpers.Size(); i++)
		{
			if (xmlParseMaster->mHelpers[i]->EndElementHandler(*xmlParseMaster->GetSharedData(), elementName))
			{
				break;
			}
		}
	}

	void XmlParseMaster::CharDataHandler(void* userData, const char* characterStream, int length)
	{
		XmlParseMaster* xmlParseMaster = reinterpret_cast<XmlParseMaster*>(userData);
		assert(xmlParseMaster != nullptr);

		std::string characterData;
		characterData.reserve(length);
		std::uint32_t i;
		for (i = 0; i < (std::uint32_t)length; i++)
		{
			characterData.push_back(characterStream[i]);
		}
		for (i = 0; i < xmlParseMaster->mHelpers.Size(); i++)
		{
			if (xmlParseMaster->mHelpers[i]->CharDataHandler(*xmlParseMaster->GetSharedData(), characterData, length))
			{
				break;
			}
		}
	}

	void XmlParseMaster::InitializeXmlParser()
	{
		XML_SetElementHandler(mXmlParser, XmlParseMaster::StartElementHandler, XmlParseMaster::EndElementHandler);
		XML_SetCharacterDataHandler(mXmlParser, XmlParseMaster::CharDataHandler);
		XML_SetUserData(mXmlParser, this);
	}

#pragma region SharedData

	XmlParseMaster::SharedData::SharedData() :
		SharedData(nullptr, 0)
	{}

	XmlParseMaster::SharedData* XmlParseMaster::SharedData::Clone()
	{
		SharedData* clonedSharedData = new SharedData(mParseMaster, 0);

		return clonedSharedData;
	}

	XmlParseMaster* XmlParseMaster::SharedData::GetXmlParseMaster() const
	{
		return mParseMaster;
	}

	void XmlParseMaster::SharedData::SetXmlParseMaster(XmlParseMaster* parseMaster)
	{
		mParseMaster = parseMaster;
	}

	void XmlParseMaster::SharedData::IncrementDepth()
	{
		mDepth++;
	}

	void XmlParseMaster::SharedData::DecrementDepth()
	{
		mDepth--;
	}

	std::uint32_t XmlParseMaster::SharedData::Depth() const
	{
		return mDepth;
	}

	XmlParseMaster::SharedData::SharedData(XmlParseMaster* parseMaster, std::uint32_t depth) :
		mParseMaster(parseMaster), mDepth(depth)
	{}

	RTTI_DEFINITIONS(XmlParseMaster::SharedData);

#pragma endregion
}
