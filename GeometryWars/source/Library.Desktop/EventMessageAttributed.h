#pragma once
#include "Attributed.h"

namespace Library
{
	/**
	 *	Acts as the payload for events triggered by Actions
	 *	@inherits Attributed
	 */
	class EventMessageAttributed : public Attributed
	{
		RTTI_DECLARATIONS(EventMessageAttributed, Attributed);
	public:
		
		/**
		 *	Constructor
		 */
		EventMessageAttributed();

		/**
		 *	Copy Constructor
		 */
		EventMessageAttributed(const EventMessageAttributed& rhs);

		/**
		 *	Move Constructor
		 */
		EventMessageAttributed(EventMessageAttributed&& rhs);

		/**
		 *	Default Destructor
		 */
		virtual ~EventMessageAttributed() = default;

		/**
		 *	Copy assignemnt operator
		 */
		EventMessageAttributed& operator=(const EventMessageAttributed& rhs);

		/**
		 *	Move assignment operator
		 */
		EventMessageAttributed& operator=(EventMessageAttributed&& rhs);

		/**
		 *	Mutator for the subtype of this EventMessage (subtype is an external prescribed attribute)
		 *	@param subtype to be assigned
		 */
		void SetSubtype(const std::string& subtype);

		/**
		 *	Accesor for the subtype of this EventMessage (subtype is an external prescribed attribute)
		 *	@return string representing the subtype
		 */
		const std::string& GetSubtype() const;

		/**
		 *	Mutator for world state pointer maintained by this EventMessage
		 *	@param subtype to be assigned
		 */
		void SetWorldState(WorldState& world);

		/**
		 *	Accesor for world state pointer maintained by this EventMessage
		 *	@return reference to the world state
		 */
		WorldState& GetWorldState() const;

		static const std::string ATTRIBUTE_SUBTYPE;
		static const std::string ATTRIBUTE_ARGUMENTS;

	private:
		std::string mSubtype;
		WorldState* mWorldState;
	};
}

