#pragma once
#include "Attributed.h"

namespace Library
{
	class EventMessageAttributed : public Attributed
	{
		RTTI_DECLARATIONS(EventMessageAttributed, Attributed);
	public:
		EventMessageAttributed();
		EventMessageAttributed(const EventMessageAttributed& rhs);
		EventMessageAttributed(EventMessageAttributed&& rhs);

		virtual ~EventMessageAttributed();

		EventMessageAttributed& operator=(const EventMessageAttributed& rhs);
		EventMessageAttributed& operator=(EventMessageAttributed&& rhs);

		void SetSubtype(const std::string& subtype);
		const std::string& GetSubtype() const;

		void SetWorldState(WorldState& world);
		WorldState& GetWorldState() const;

		static const std::string ATTRIBUTE_SUBTYPE;
		static const std::string ATTRIBUTE_ARGUMENTS;

	protected:
		virtual void DefinePrescribedAttributes() override;

	private:
		std::string mSubtype;
		WorldState* mWorldState;
	};
}

