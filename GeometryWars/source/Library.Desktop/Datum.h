#pragma once

#include "glm\mat4x4.hpp"
#include "glm\vec4.hpp"
#include "RTTI.h"

namespace Library
{
	class Scope;
	/**
	 *	Defines a run-time polymorphic data type
	 */
	class Datum
	{
		public:

			/**
			 *	Enumerated list of datatypes supported by Datum
			 */
			enum class DatumType
			{
				UNKNOWN,
				INTEGER,
				FLOAT,
				VECTOR4,
				MATRIX4x4,
				TABLE,
				STRING,
				POINTER,
				REFERENCE,
				BOOLEAN,
				END
			};

			/**
			 *	Enumerated list of storage types supported by Datum
			 */
			enum class DatumStorageType
			{
				UNKNOWN,
				INTERNAL,
				EXTERNAL,
			};

			/**
			 *	Constructor - initializes Datum to UNKNOWN type, with 0 size and capacity
			 */
			Datum();

			/**
			 *	Copy Constructor
			 *	@param constant reference to the Datum being copieds
			 */
			Datum(const Datum& rhs);

			/**
			 *	Move Constructor
			 *	@param R value reference to the Datum being moved
			 */
			Datum(Datum&& rhs);

			/**
			 *	Destructor
			 */
			~Datum();

			/**
			 *	Copy assignment operator - makes a deep copy of the provided datum
			 *	@param constant reference to the datum being copied
			 *	@return reference to current modififed datum
			 */
			Datum& operator=(const Datum& rhs);

			/**
			 *	Move assignment operator
			 *	@param reference to the datum being moved
			 *	@return reference to current modififed datum
			 */
			Datum& operator=(Datum&& rhs);

			/**
			 *	Assignment operator overloads to set index 0 of supported data types
			 *	@param reference to the supported data type value to be assigned to this Datum
			 *	@return reference to current modififed datum
			 */
			Datum& operator=(const std::int32_t& rhs);
			Datum& operator=(const std::float_t& rhs);
			Datum& operator=(const std::string& rhs);
			Datum& operator=(const char* rhs);
			Datum& operator=(const glm::vec4& rhs);
			Datum& operator=(const glm::mat4x4& rhs);
			Datum& operator=(const bool& rhs);
			Datum& operator=(RTTI* rhs);
			Datum& operator=(Scope& rhs);
			Datum& operator=(Datum* rhs);

			Datum operator+(const Datum& rhs) const;
			Datum operator-(const Datum& rhs) const;
			Datum operator*(const Datum& rhs) const;
			Datum operator/(const Datum& rhs) const;

			Datum operator||(const Datum& rhs) const;
			Datum operator&&(const Datum& rhs) const;
			Datum operator<(const Datum& rhs) const;
			Datum operator>(const Datum& rhs) const;
			Datum operator<=(const Datum& rhs) const;
			Datum operator>=(const Datum& rhs) const;

			/**
			 *	Gets the DatumType of this Datum
			 *	@return current DatumType
			 */
			DatumType Type() const;

			/**
			 *	Sets the DatumType of this Datum
			 *	@param DatumType to be set
			 */
			void SetType(DatumType type);

			/**
			 *	Gets the StorageType of this Datum
			 *	@return current StorageType
			 */
			DatumStorageType StorageType() const;

			/**
			 *	Gets the number of elements stored in the Datum
			 *	@return size
			 */
			std::uint32_t Size() const;

			/**
			 *	Gets the number of elements for which memory has been reserved in this Datum
			 *	@return capacity
			 */
			std::uint32_t Capacity() const;

			/**
			 *	If memory is INTERNAL, and new size of more than current size, it reservers the required memory and increases the size; 
			 *	destructs the elements if new size if less than current size
			 *	@exception cannot set size of a Datum with EXTERNAL storage
			 *	@param the new number of constructed elements in the Datum;
			 */
			void SetSize(std::uint32_t newSize);

			/**
			 *	If memory is INTERNAL, allocated required amount of memory
			 *	@exception cannot reserve for a Datum with EXTERNAL storage; cannot shrink capacity, cannot resever for UNKNOWN Datum type
			 *	@param the new capacity
			 */
			void Reserve(std::uint32_t newCapacity);

			/**
			 *	destroys all elements if storage type is INTERNAL, returns all member variables to initial state
			 */
			void Clear();

			/**
			 *	Sets the storage type to EXTERNAL and points the datum to given array
			 *	@param pointer to data array
			 *	@param number of elements in the given array
			 */
			void SetStorage(std::int32_t* dataArray, std::uint32_t arraySize);
			void SetStorage(std::float_t* dataArray, std::uint32_t arraySize);
			void SetStorage(std::string* dataArray, std::uint32_t arraySize);
			void SetStorage(glm::vec4* dataArray, std::uint32_t arraySize);
			void SetStorage(glm::mat4x4* dataArray, std::uint32_t arraySize);
			void SetStorage(bool* dataArray, std::uint32_t arraySize);
			void SetStorage(RTTI** dataArray, std::uint32_t arraySize);
			void SetStorage(Datum** dataArray, std::uint32_t arraySize);

			/**
			 *	Equality operator, deep comparison of the given Datums
			 *	@param reference to the datum to be compared with
			 *	@return true of the type, number of elements and value of each element is same in both datums
			 */
			bool operator==(const Datum& rhs) const;

			/**
	 		 *	Equality operator overload to compare the first element of Datum against the supported data type value
			 *	@param reference to supported type value
			 *	@return boolean comparison result
			 */
			bool operator==(const std::int32_t& rhs) const;
			bool operator==(const std::float_t& rhs) const;
			bool operator==(const std::string& rhs) const;
			bool operator==(const char* rhs) const;
			bool operator==(const glm::vec4& rhs) const;
			bool operator==(const glm::mat4x4& rhs) const;
			bool operator==(const bool& rhs) const;
			bool operator==(const RTTI* rhs) const;
			bool operator==(const Scope* rhs) const;
			bool operator==(const Datum* rhs) const;

			/**
			 *	Not-Equals operator overloads
			 *	@param reference to supported type value
			 *	@return compliment of the comparison operator result
			 */
			bool operator!=(const Datum& rhs) const;
			bool operator!=(const std::int32_t& rhs) const;
			bool operator!=(const std::float_t& rhs) const;
			bool operator!=(const std::string& rhs) const;
			bool operator!=(const char* rhs) const;
			bool operator!=(const glm::vec4& rhs) const;
			bool operator!=(const glm::mat4x4& rhs) const;
			bool operator!=(const bool& rhs) const;
			bool operator!=(const RTTI* rhs) const;
			bool operator!=(const Scope* rhs) const;
			bool operator!=(const Datum* rhs) const;

			/**
			 *	Sets the given data type value to the given index
			 *	if index is more than size and storage is INTERNAL, it increases capacity; throws an exception if the storage is external
			 *	@param reference to supported type value
			 *	@param index of the element to set the value of
			 */
			void Set(std::int32_t value, std::uint32_t index = 0);
			void Set(std::float_t value, std::uint32_t index = 0);
			void Set(const std::string& value, std::uint32_t index = 0);
			void Set(const char* value, std::uint32_t index = 0);
			void Set(const glm::vec4& value, std::uint32_t index = 0);
			void Set(const glm::mat4x4& value, std::uint32_t index = 0);
			void Set(bool value, std::uint32_t index = 0);
			void Set(RTTI* value, std::uint32_t index = 0);
			void Set(Scope& value, std::uint32_t index = 0);
			void Set(Datum* value, std::uint32_t index = 0);

			void Remove(std::uint32_t index);

			/**
			 *	Gets the data type value at the given index
			 *	@param index of the element to get the value of
			 *	@return reference to the data element value
			 */
			template <typename T>
			T& Get(std::uint32_t index = 0) const;
			template<>
			std::int32_t& Get<std::int32_t>(std::uint32_t index) const;
			template<>
			std::float_t& Get<std::float_t>(std::uint32_t index) const;
			template<>
			std::string& Get<std::string>(std::uint32_t index) const;
			template<>
			glm::vec4& Get<glm::vec4>(std::uint32_t index) const;
			template<>
			glm::mat4x4& Get<glm::mat4x4>(std::uint32_t index) const;
			template<>
			bool& Get<bool>(std::uint32_t index) const;
			template<>
			Scope& Get<Scope>(std::uint32_t index) const;
			//template<>
			//Datum& Get<Datum>(std::uint32_t index) const;

			Scope& operator[](std::uint32_t index) const;


			/**
			 *	Parses the given string with a specified format and assignes the value to the given index
			 *	"%d" for integers, "%f" for floats, "%s" for strings, "(%f, %f, %f, %f)" for vectors
			 *	"{{%f, %f, %f, %f}, {%f, %f, %f, %f}, {%f, %f, %f, %f}, {%f, %f, %f, %f}}" for matrix4, currently unsupported for RTTI
			 *	@param formated string to be serialized for setting the value
			 *	@param index of the element to set the value of
			 */
			void SetFromString(const std::string& inputString, std::uint32_t index = 0);

			/**
			 *	Deserializes the data at the given index to a string
			 *	@param index of the element to set the value of
			 *	@return formated deerialized string representing the data value
			 */
			std::string ToString(std::uint32_t index = 0) const;

			static const Hashmap<DatumType, std::string> DatumTypeToString;

		private:
			class DatumTypeHash
			{
			public:
				std::uint32_t operator()(const DatumType& key) const;
			};

			template <DatumType T>
			Datum Add(const Datum& rhs) const;
			template<>
			Datum Add<DatumType::INTEGER>(const Datum& rhs) const;
			template<>
			Datum Add<DatumType::FLOAT>(const Datum& rhs) const;
			template<>
			Datum Add<DatumType::STRING>(const Datum& rhs) const;
			template<>
			Datum Add<DatumType::VECTOR4>(const Datum& rhs) const;
			template<>
			Datum Add<DatumType::MATRIX4x4>(const Datum& rhs) const;

			typedef Datum (Datum::*AddDatums)(const Datum&) const;
			static Hashmap<DatumType, AddDatums, DatumTypeHash> mAddDatums;

			template <DatumType T>
			Datum Subtract(const Datum& rhs) const;
			template<>
			Datum Subtract<DatumType::INTEGER>(const Datum& rhs) const;
			template<>
			Datum Subtract<DatumType::FLOAT>(const Datum& rhs) const;
			template<>
			Datum Subtract<DatumType::VECTOR4>(const Datum& rhs) const;
			template<>
			Datum Subtract<DatumType::MATRIX4x4>(const Datum& rhs) const;

			typedef Datum(Datum::*SubtractDatums)(const Datum&) const;
			static Hashmap<DatumType, SubtractDatums, DatumTypeHash> mSubtractDatums;

			template <DatumType T>
			Datum Multiply(const Datum& rhs) const;
			template<>
			Datum Multiply<DatumType::INTEGER>(const Datum& rhs) const;
			template<>
			Datum Multiply<DatumType::FLOAT>(const Datum& rhs) const;
			template<>
			Datum Multiply<DatumType::VECTOR4>(const Datum& rhs) const;
			template<>
			Datum Multiply<DatumType::MATRIX4x4>(const Datum& rhs) const;

			typedef Datum(Datum::*MultiplyDatums)(const Datum&) const;
			static Hashmap<DatumType, MultiplyDatums, DatumTypeHash> mMultiplyDatums;

			template <DatumType T>
			Datum Divide(const Datum& rhs) const;
			template<>
			Datum Divide<DatumType::INTEGER>(const Datum& rhs) const;
			template<>
			Datum Divide<DatumType::FLOAT>(const Datum& rhs) const;
			template<>
			Datum Divide<DatumType::VECTOR4>(const Datum& rhs) const;
			template<>
			Datum Divide<DatumType::MATRIX4x4>(const Datum& rhs) const;

			typedef Datum(Datum::*DivideDatums)(const Datum&) const;
			static Hashmap<DatumType, DivideDatums, DatumTypeHash> mDivideDatums;

			template <DatumType T>
			Datum LessThan(const Datum& rhs) const;
			template<>
			Datum LessThan<DatumType::INTEGER>(const Datum& rhs) const;
			template<>
			Datum LessThan<DatumType::FLOAT>(const Datum& rhs) const;
			template<>
			Datum LessThan<DatumType::STRING>(const Datum& rhs) const;

			typedef Datum(Datum::*LessThanDatums)(const Datum&) const;
			static Hashmap<DatumType, LessThanDatums, DatumTypeHash> mLessThanDatums;

			DatumType mType;
			std::uint32_t mSize;
			std::uint32_t mCapacity;
			DatumStorageType mStorageType;

			union Data
			{
				std::int32_t* mInt = nullptr;
				std::float_t* mFloat;
				std::string* mString;
				glm::vec4* mVec4;
				glm::mat4x4* mMat4x4;
				bool* mBool;
				RTTI** mRttiPtr;
				Scope** mScopePtr;
				Datum** mDatumPtr;
			};
			Data mData;

			void SetStorage(DatumType typeToSet, std::uint32_t arraySize);
			void Set(DatumType typeToSet, std::uint32_t index);
			void CurrentTypeCheck(DatumType typeToComapreTo) const;
			void OutOfBoundsCheck(std::uint32_t index) const;

	};

}