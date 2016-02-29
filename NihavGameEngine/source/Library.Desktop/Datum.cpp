#include "pch.h"
#include "Datum.h"

namespace Library
{
	Datum::Datum() :
		mType(DatumType::UNKNOWN), mSize(0), mCapacity(0), mStorageType(DatumStorageType::UNKNOWN)
	{}

	Datum::Datum(const Datum& rhs) :
		Datum()
	{
		operator=(rhs);
	}

	Datum::Datum(Datum&& rhs) : 
		Datum(rhs)
	{
		rhs.Clear();
	}


	Datum::~Datum()
	{
		Clear();
	}

#pragma region Assignment operator

	Datum& Datum::operator=(const Datum& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			
			mType = rhs.mType;
			mStorageType = rhs.mStorageType;
			if (rhs.mStorageType == DatumStorageType::INTERNAL)
			{
				Reserve(rhs.mCapacity);
				for (std::uint32_t i = 0; i < rhs.mSize; i++)
				{
					switch (mType)
					{
					case DatumType::INTEGER:
						Set(rhs.mData.mInt[i], i);
						break;
					
					case DatumType::FLOAT:
						Set(rhs.mData.mFloat[i], i);
						break;
					
					case DatumType::STRING:
						Set(rhs.mData.mString[i], i);
						break;
					
					case DatumType::VECTOR4:
						Set(rhs.mData.mVec4[i], i);
						break;
					
					case DatumType::MATRIX4x4:
						Set(rhs.mData.mMat4x4[i], i);
						break;
					
					case DatumType::POINTER:
						Set(rhs.mData.mRttiPtr[i], i);
						break;

					default:
						break;
					}
				}
			}
			else
			{
				mData = rhs.mData;
			}

			mSize = rhs.mSize;
			mCapacity = rhs.mCapacity;

		}

		return *this;
	}

	Datum& Datum::operator=(Datum&& rhs)
	{
		if (this != &rhs)
		{
			operator=(rhs);
			rhs.Clear();
		}
		return *this;
	}

	Datum& Datum::operator=(const std::int32_t& rhs)
	{
		Set(rhs);
		return *this;
	}

	Datum& Datum::operator=(const std::float_t& rhs)
	{
		Set(rhs);
		return *this;
	}

	Datum& Datum::operator=(const std::string& rhs)
	{
		Set(rhs);
		return *this;
	}

	Datum& Datum::operator=(const glm::vec4& rhs)
	{
		Set(rhs);
		return *this;
	}

	Datum& Datum::operator=(const glm::mat4x4& rhs)
	{
		Set(rhs);
		return *this;
	}

	Datum& Datum::operator=(RTTI* rhs)
	{
		Set(rhs);
		return *this;
	}

#pragma endregion

	Datum::DatumType Datum::Type() const
	{
		return mType;
	}

	void Datum::SetType(DatumType type)
	{
		if (!(mType == DatumType::UNKNOWN || mType == type))
		{
			throw std::exception("Cannot change type of Datum");
		}
		mType = type;
	
		if(mStorageType == DatumStorageType::UNKNOWN)
			mStorageType = DatumStorageType::INTERNAL;
	}

	Datum::DatumStorageType Datum::StorageType() const
	{
		return mStorageType;
	}

	std::uint32_t Datum::Size() const
	{
		return mSize;
	}

	std::uint32_t Datum::Capacity() const
	{
		return mCapacity;
	}

	void Datum::SetSize(std::uint32_t newSize)
	{
		if (mType == DatumType::UNKNOWN)
		{
			throw std::exception("Invalid operation. Datum type not set.");
		}
		if (mStorageType == DatumStorageType::INTERNAL)
		{
			if (newSize > mCapacity)
			{
				Reserve(newSize);
			}
			while (mSize < newSize)
			{
				switch (mType)
				{
				case DatumType::INTEGER:
					new (mData.mInt + mSize)std::int32_t();
					break;
				case DatumType::FLOAT:
					new (mData.mFloat + mSize)std::float_t();
					break;
				case DatumType::STRING:
					new (mData.mString + mSize)std::string();
					break;
				case DatumType::VECTOR4:
					new (mData.mVec4 + mSize)glm::vec4();
					break;
				case DatumType::MATRIX4x4:
					new (mData.mMat4x4 + mSize)glm::mat4x4();
					break;
				case DatumType::POINTER:
					new (mData.mRttiPtr + mSize)RTTI*(nullptr);
					break;

				default:
					throw std::exception("Invalid data type");
					break;
				}
				++mSize;
			}
			// shrink size if its less than the new size [dont shrink capacity]
			while (mSize > newSize)
			{
				--mSize;
				switch (mType)
				{
				case DatumType::INTEGER:
					break;

				case DatumType::FLOAT:
					break;

				case DatumType::STRING:
					mData.mString[mSize].std::string::~string();
					break;

				case DatumType::VECTOR4:
					mData.mVec4[mSize].glm::vec4::~vec4();
					break;

				case DatumType::MATRIX4x4:
					mData.mMat4x4[mSize].glm::mat4x4::~mat4x4();
					break;

				case DatumType::POINTER:
					if(mData.mRttiPtr[mSize] != nullptr)
						(*mData.mRttiPtr[mSize]).~RTTI();
					break;

				default:
					throw std::exception("Invalid data type");
					break;
				}
			}
		}
		else
		{
			throw std::exception("Cannot modify external storage.");
		}
	}

	void Datum::Reserve(std::uint32_t newCapacity)
	{
		if (mStorageType == DatumStorageType::EXTERNAL)
		{
			throw std::exception("cannot reserve memory for Datum with external storage");
		}
		if (newCapacity > mCapacity)
		{
			switch (mType)
			{
				case DatumType::INTEGER:
					mData.mInt = reinterpret_cast<std::int32_t*>(realloc(mData.mInt, newCapacity * sizeof(std::int32_t)));
					break;

				case DatumType::FLOAT:
					mData.mFloat = reinterpret_cast<std::float_t*>(realloc(mData.mFloat, newCapacity * sizeof(std::float_t)));
					break;

				case DatumType::STRING:
					mData.mString = reinterpret_cast<std::string*>(realloc(mData.mString, newCapacity * sizeof(std::string)));
					break;

				case DatumType::VECTOR4:
					mData.mVec4 = reinterpret_cast<glm::vec4*>(realloc(mData.mVec4, newCapacity * sizeof(glm::vec4)));
					break;

				case DatumType::MATRIX4x4:
					mData.mMat4x4 = reinterpret_cast<glm::mat4x4*>(realloc(mData.mMat4x4, newCapacity * sizeof(glm::mat4x4)));
					break;

				case DatumType::POINTER:
					mData.mRttiPtr = reinterpret_cast<RTTI**>(realloc(mData.mRttiPtr, newCapacity * sizeof(RTTI*)));
					break;

				default:
					throw std::exception("Invalid data type");
					break;
			}
			mCapacity = newCapacity;
		}
		else if(newCapacity < mCapacity)
		{
			throw std::exception("Cannot shrink capacity");
		}
	}

	void Datum::Clear()
	{
		if (mStorageType == DatumStorageType::INTERNAL)
		{
			switch (mType)
			{
			case DatumType::INTEGER:
				free(mData.mInt);
				break;

			case DatumType::FLOAT:
				free(mData.mFloat);
				break;

			case DatumType::STRING:
				for (std::uint32_t i = 0; i < mSize; i++)
					mData.mString[i].std::string::~string();
				free(mData.mString);
				break;

			case DatumType::VECTOR4:
				for (std::uint32_t i = 0; i < mSize; i++)
					mData.mVec4[i].glm::vec4::~vec4();
				free(mData.mVec4);
				break;

			case DatumType::MATRIX4x4:
				for (std::uint32_t i = 0; i < mSize; i++)
					mData.mMat4x4[i].glm::mat4x4::~mat4x4();
				free(mData.mMat4x4);
				break;

			case DatumType::POINTER:
				for (std::uint32_t i = 0; i < mSize; i++)
				{
					if(mData.mRttiPtr[i] != nullptr)
						(*mData.mRttiPtr[i]).~RTTI();
				}
				free(mData.mRttiPtr);
				break;

			default:
				break;
			}
		}

		mData.mInt = nullptr;
		mType = DatumType::UNKNOWN;
		mStorageType = DatumStorageType::UNKNOWN;
		mSize = 0;
		mCapacity = 0;
	}

#pragma region SetStorage

	void Datum::SetStorage(DatumType typeToSet, std::uint32_t arraySize)
	{
		if (!(mType == DatumType::UNKNOWN || mType == typeToSet))
			throw std::exception("Invalid operation. Cannot change type of Datum.");

		if (mStorageType == DatumStorageType::INTERNAL)
		{
			Clear();
		}

		mType = typeToSet;
		mStorageType = DatumStorageType::EXTERNAL;
		mSize = arraySize;
		mCapacity = arraySize;
	}

	void Datum::SetStorage(std::int32_t* dataArray, std::uint32_t arraySize)
	{
		SetStorage(DatumType::INTEGER, arraySize);
		mData.mInt = dataArray;
	}

	void Datum::SetStorage(std::float_t* dataArray, std::uint32_t arraySize)
	{
		SetStorage(DatumType::FLOAT, arraySize);
		mData.mFloat = dataArray;
	}

	void Datum::SetStorage(std::string* dataArray, std::uint32_t arraySize)
	{
		SetStorage(DatumType::STRING, arraySize);
		mData.mString = dataArray;
	}

	void Datum::SetStorage(glm::vec4* dataArray, std::uint32_t arraySize)
	{
		SetStorage(DatumType::VECTOR4, arraySize);
		mData.mVec4 = dataArray;
	}

	void Datum::SetStorage(glm::mat4x4* dataArray, std::uint32_t arraySize)
	{
		SetStorage(DatumType::MATRIX4x4, arraySize);
		mData.mMat4x4 = dataArray;
	}

	void Datum::SetStorage(RTTI ** dataArray, std::uint32_t arraySize)
	{
		SetStorage(DatumType::POINTER, arraySize);
		mData.mRttiPtr = dataArray;
	}

#pragma endregion

#pragma region Equality operators

	bool Datum::operator==(const Datum& rhs) const
	{
		bool areEqual = false;
		if (mType == rhs.mType && mSize == rhs.mSize)
		{
			areEqual = true;
			for (std::uint32_t i = 0; i < mSize; i++)
			{
				switch (mType)
				{
				case DatumType::INTEGER:
					if (mData.mInt[i] != rhs.mData.mInt[i])
					{
						areEqual = false;
						break;
					}
					break;
				
				case DatumType::FLOAT:
					if (mData.mFloat[i] != rhs.mData.mFloat[i])
					{
						areEqual = false;
						break;
					}
					break;
				
				case DatumType::STRING:
					if (mData.mString[i].compare(rhs.mData.mString[i]) != 0)
					{
						areEqual = false;
						break;
					}
					break;

				case DatumType::VECTOR4:
					if (mData.mVec4[i] != rhs.mData.mVec4[i])
					{
						areEqual = false;
						break;
					}
					break;
				case DatumType::MATRIX4x4:
					if (mData.mMat4x4[i] != rhs.mData.mMat4x4[i])
					{
						areEqual = false;
						break;
					}
					break;
				case DatumType::POINTER:
					if (mData.mRttiPtr[i] != rhs.mData.mRttiPtr[i])
					{
						areEqual = false;
						break;
					}
					break;
				default:
					throw std::exception("Invalid data type");
					break;
				}
			}

		}
		return areEqual;
	}

	bool Datum::operator==(const std::int32_t& rhs) const
	{
		CurrentTypeCheck(DatumType::INTEGER);
		OutOfBoundsCheck(0);
		return (*mData.mInt == rhs);
	}

	bool Datum::operator==(const std::float_t& rhs) const
	{
		CurrentTypeCheck(DatumType::FLOAT);
		OutOfBoundsCheck(0);
		return (*mData.mFloat == rhs);
	}

	bool Datum::operator==(const std::string & rhs) const
	{
		CurrentTypeCheck(DatumType::STRING);
		OutOfBoundsCheck(0);
		return (rhs.compare(*mData.mString) == 0);
	}

	bool Datum::operator==(const glm::vec4& rhs) const
	{
		CurrentTypeCheck(DatumType::VECTOR4);
		OutOfBoundsCheck(0);
		return *mData.mVec4 == rhs;
	}

	bool Datum::operator==(const glm::mat4x4& rhs) const
	{
		CurrentTypeCheck(DatumType::MATRIX4x4);
		OutOfBoundsCheck(0);
		return *mData.mMat4x4 == rhs;
	}

	bool Datum::operator==(const RTTI* rhs) const
	{
		CurrentTypeCheck(DatumType::POINTER);
		OutOfBoundsCheck(0);
		return ((**mData.mRttiPtr).Equals(rhs));
	}


	bool Datum::operator!=(const Datum & rhs) const
	{
		return !operator==(rhs);
	}

	bool Datum::operator!=(const std::int32_t& rhs) const
	{
		return !(operator==(rhs));
	}

	bool Datum::operator!=(const std::float_t& rhs) const
	{
		return !(operator==(rhs));
	}

	bool Datum::operator!=(const std::string& rhs) const
	{
		return !(operator==(rhs));
	}

	bool Datum::operator!=(const glm::vec4& rhs) const
	{
		return !(operator==(rhs));
	}

	bool Datum::operator!=(const glm::mat4x4& rhs) const
	{
		return !(operator==(rhs));
	}

	bool Datum::operator!=(const RTTI* rhs) const
	{
		return !(operator==(rhs));
	}

#pragma endregion

#pragma region Set

	void Datum::Set(DatumType typeToSet, std::uint32_t index)
	{
		if (!(mType == DatumType::UNKNOWN || mType == typeToSet))
			throw std::exception("Type mismatch exception.");

		if (mType == DatumType::UNKNOWN)
		{
			mType = typeToSet;
			mStorageType = DatumStorageType::INTERNAL;
		}
		if (index >= mSize)
		{
			if (mStorageType == DatumStorageType::EXTERNAL)
				throw std::exception("Index out of bounds for datum with external storage");
			SetSize(index + 1);
		}
	}

	void Datum::Set(std::int32_t value, std::uint32_t index)
	{
		Set(DatumType::INTEGER, index);
		mData.mInt[index] = value;
	}

	void Datum::Set(std::float_t value, std::uint32_t index)
	{
		Set(DatumType::FLOAT, index);
		mData.mFloat[index] = value;
	}

	void Datum::Set(const std::string& value, std::uint32_t index)
	{
		Set(DatumType::STRING, index);
		mData.mString[index] = value;
	}

	void Datum::Set(const glm::vec4& value, std::uint32_t index)
	{
		Set(DatumType::VECTOR4, index);
		mData.mVec4[index] = value;
	}

	void Datum::Set(const glm::mat4x4& value, std::uint32_t index)
	{
		Set(DatumType::MATRIX4x4, index);
		mData.mMat4x4[index] = value;
	}

	void Datum::Set(RTTI* value, std::uint32_t index)
	{
		Set(DatumType::POINTER, index);
		mData.mRttiPtr[index] = value;
	}

#pragma endregion

#pragma region Get

	template<>
	std::int32_t& Datum::Get<std::int32_t>(std::uint32_t index) const
	{
		CurrentTypeCheck(DatumType::INTEGER);
		OutOfBoundsCheck(index);
		return mData.mInt[index];
	}

	template<>
	std::float_t& Datum::Get<std::float_t>(std::uint32_t index) const
	{
		CurrentTypeCheck(DatumType::FLOAT);
		OutOfBoundsCheck(index);
		return mData.mFloat[index];
	}

	template<>
	std::string& Datum::Get<std::string>(std::uint32_t index) const
	{
		CurrentTypeCheck(DatumType::STRING);
		OutOfBoundsCheck(index);
		return mData.mString[index];
	}

	template<>
	glm::vec4& Datum::Get<glm::vec4>(std::uint32_t index) const
	{
		CurrentTypeCheck(DatumType::VECTOR4);
		OutOfBoundsCheck(index);
		return mData.mVec4[index];
	}

	template<>
	glm::mat4x4& Datum::Get<glm::mat4x4>(std::uint32_t index) const
	{
		CurrentTypeCheck(DatumType::MATRIX4x4);
		OutOfBoundsCheck(index);
		return mData.mMat4x4[index];
	}

	template<>
	RTTI*& Datum::Get<RTTI*>(std::uint32_t index) const
	{
		CurrentTypeCheck(DatumType::POINTER);
		OutOfBoundsCheck(index);
		return mData.mRttiPtr[index];
	}

#pragma endregion

	void Datum::SetFromString(const std::string& inputString, std::uint32_t index)
	{
		if (mType == DatumType::UNKNOWN)
			throw std::exception("Invalid operation. Datum type not set.");
		switch (mType)
		{
			case DatumType::INTEGER:
				std::int32_t intValue;
				sscanf_s(inputString.c_str(), "%d", &intValue);
				Set(intValue, index);
				break;

			case DatumType::FLOAT:
				std::float_t floatValue;
				sscanf_s(inputString.c_str(), "%f", &floatValue);
				Set(floatValue, index);
				break;

			case DatumType::STRING:
				Set(inputString, index);
				break;

			case DatumType::VECTOR4:
				std::float_t fVec1, fVec2, fVec3, fVec4;
				sscanf_s(inputString.c_str(), "(%f, %f, %f, %f)", &fVec1, &fVec2, &fVec3, &fVec4);
				Set(glm::vec4(fVec1, fVec2, fVec3, fVec4), index);
				break;

			case DatumType::MATRIX4x4:
				std::float_t fMatX1, fMatY1, fMatZ1, fMatW1;
				std::float_t fMatX2, fMatY2, fMatZ2, fMatW2;
				std::float_t fMatX3, fMatY3, fMatZ3, fMatW3;
				std::float_t fMatX4, fMatY4, fMatZ4, fMatW4;
				sscanf_s(inputString.c_str(), "{{%f, %f, %f, %f}, {%f, %f, %f, %f}, {%f, %f, %f, %f}, {%f, %f, %f, %f}}", 
					&fMatX1, &fMatY1, &fMatZ1, &fMatW1,
					&fMatX2, &fMatY2, &fMatZ2, &fMatW2,
					&fMatX3, &fMatY3, &fMatZ3, &fMatW3,
					&fMatX4, &fMatY4, &fMatZ4, &fMatW4);
				Set(glm::mat4x4(
					fMatX1, fMatY1, fMatZ1, fMatW1,
					fMatX2, fMatY2, fMatZ2, fMatW2,
					fMatX3, fMatY3, fMatZ3, fMatW3,
					fMatX4, fMatY4, fMatZ4, fMatW4), index);
				break;

			case DatumType::POINTER:
				throw std::exception("Invalid operation. Cannot set RTTI pointer from string.");
				break;

			default:
				throw std::exception("Invalid operation. Datum type not set.");
				break;
		}
	}

	std::string Datum::ToString(std::uint32_t index) const
	{
		if (mType == DatumType::UNKNOWN)
			throw std::exception("Cannot convert variable of unknown Datum type to string");
		if (index >= mSize)
			throw std::exception("Index out of bounds");

		std::string toString = "";
		switch (mType)
		{
		case DatumType::INTEGER:
			toString = std::to_string(mData.mInt[index]);
			break;

		case DatumType::FLOAT:
			toString = std::to_string(mData.mFloat[index]);
			break;

		case DatumType::STRING:
			toString = mData.mString[index];
			break;

		case DatumType::VECTOR4:
			toString.append("(");
			for (std::uint32_t i = 0; i < 4; i++)
			{
				toString.append(std::to_string(mData.mVec4[index].data[i]));
				if (i < 3)
					toString.append(", ");
			}
			toString.append(")");
			break;

		case DatumType::MATRIX4x4:
			toString.append("{");
			for (std::uint32_t i = 0; i < 4; i++)
			{
				toString.append("{");
				for (std::uint32_t j = 0; j < 4; j++)
				{
					toString.append(std::to_string(mData.mMat4x4[index][i].data[j]));
					if(j < 3)
						toString.append(", ");
				}
				toString.append("}");
				if (i < 3)
					toString.append(", ");
			}
			toString.append("}");
			break;

		case DatumType::POINTER:
			toString = (*mData.mRttiPtr[index]).ToString();
			break;

		default:
			throw std::exception("Invalid operation. Datum type not set.");
			break;
		}

		return toString;
	}

	void Datum::CurrentTypeCheck(DatumType typeToComapreTo) const
	{
		if (mType != typeToComapreTo)
			throw std::exception("Incompatible types.");
	}

	void Datum::OutOfBoundsCheck(std::uint32_t index) const
	{
		if (mSize <= index)
			throw std::exception("index out of bounds");
	}

}
