#pragma once

namespace Library
{
	/**
	*	Functor class to define the default reserve strategy to be used by this Vector
	*/
	class DefaultReserveStrategy
	{
	public:
		/**
		*	Operaror () override for functor
		*	@param unsigned integer representinf current size of the vector
		*	@param unsigned integer representing current capacity of the vector
		*	@return unsigned integer representing new capacity for the vector
		*/
		std::uint32_t operator()(std::uint32_t size, std::uint32_t capacity) const;
	};
}
