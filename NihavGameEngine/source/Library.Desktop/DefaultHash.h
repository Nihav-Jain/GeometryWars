#pragma once

namespace Library
{
	template <class TKey>
	class DefaultHash
	{
		public:
			std::uint32_t operator()(const TKey& key) const
			{
				UNREFERENCED_PARAMETER(key);
				return 0;
			}
	};

	template <class TKeyPtr>
	class DefaultHash<TKeyPtr*>
	{
	public:
		std::uint32_t operator()(TKeyPtr* key) const
		{
			UNREFERENCED_PARAMETER(key);
			return 0;
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

