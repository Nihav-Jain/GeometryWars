#pragma once

namespace Library
{
	template <class TKey>
	class DefaultHash
	{
		public:
			std::uint32_t operator()(const TKey& key) const
			{
				std::uint32_t hash = 0;
				std::uint8_t* arr = reinterpret_cast<std::uint8_t*>(const_cast<TKey*>(&key));

				for (std::uint32_t i = 0; i < sizeof(&key); i++)
				{
					hash += arr[i];
				}
				return hash;
			}
	};

	template <class TKeyPtr>
	class DefaultHash<TKeyPtr*>
	{
	public:
		std::uint32_t operator()(TKeyPtr* key) const
		{
			std::uint32_t hash = 0;
			std::uint8_t* arr = reinterpret_cast<std::uint8_t*>(key);

			for (std::uint32_t i = 0; i < sizeof(key); i++)
			{
				hash += arr[i];
			}

			return hash;
		}
	};

	template <>
	class DefaultHash<int>
	{
	public:
		std::uint32_t operator()(const int& key) const;
	};

	template <>
	class DefaultHash<int*>
	{
	public:
		std::uint32_t operator()(int* key) const;
	};

	template <>
	class DefaultHash<std::string>
	{
	public:
		std::uint32_t operator()(const std::string& key) const;
	};

	template <>
	class DefaultHash<char*>
	{
	public:
		std::uint32_t operator()(const char* key) const;
	};

}
//#include "DefaultHash.inl"
