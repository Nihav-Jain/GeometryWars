#pragma once

#include "RTTI.h"
#include "Vector.h"
#include "Hashmap.h"
#include "Stack.h"

struct XML_ParserStruct;
typedef struct XML_ParserStruct *XML_Parser;
namespace Library
{
	class IXmlParseHelper;

	/**
	 *	Master XML parser class which wraps the Expat XML parser and conveys the parsed results to subscribed helpers for handling specific grammers
	 */
	class XmlParseMaster final
	{
	public:

		/**
		 *	Class to store the parsed data in an object shared among all helpers of the parse master
		 */
		class SharedData : public RTTI
		{
			RTTI_DECLARATIONS(SharedData, RTTI);
		public:
			friend XmlParseMaster;
			/**
			 *	Default parameterless constructor for SharedData
			 */
			SharedData();

			/**
			 *	Copy constructor for SharedData
			 *	@param reference to SharedData to be copied from
			 */
			SharedData(const SharedData& rhs) = delete;

			/**
			 *	Default destructor
			 */
			virtual ~SharedData() = default;

			/**
			 *	Copy assignement oeprator for SharedData
			 *	@param reference to SharedData to be copied from
			 *	@return reference to copied SharedData
			 */
			SharedData& operator=(const SharedData& rhs) = delete;

			/**
			 *	Default constructed SharedData
			 *	@return pointer to the newly constructed SharedData
			 */
			virtual SharedData* Clone() const;

			/**
			 *	Accessor for the parse master of this SharedData
			 *	@return pointer to the parse master of this SharedData
			 */
			XmlParseMaster* GetXmlParseMaster() const;

			/**
			 *	Increments the depth of this SharedData by 1
			 */
			void IncrementDepth();

			/**
			 *	Decrements the depth of this SharedData by 1
			 */
			void DecrementDepth();

			/**
			 *	Accessor for the depth of this shared data
			 *	@return depth
			 */
			std::uint32_t Depth() const;

		protected:
			/**
			 *	Protected constructor, works as a helper
			 */
			SharedData(XmlParseMaster* parseMaster, std::uint32_t depth);

		private:
			/**
			 * 	Mutator for the parse master of this SharedData
			 *	@param pointer to the parse master of this SharedData
			 */
			void SetXmlParseMaster(XmlParseMaster* parseMaster);

			XmlParseMaster* mParseMaster;
			std::uint32_t mDepth;
		};

		/**
		 *	Constructor
		 *	@param reference to the SharedData for this parse master
		 */
		explicit XmlParseMaster(SharedData& sharedData);

		/**
		 *	Copy Constructor
		 *	@param reference to the parse master to be copied from
		 */
		XmlParseMaster(const XmlParseMaster& rhs) = delete;

		/**
		 *	Destructor
		 */
		~XmlParseMaster();

		/**
		 *	Copy assignment operator
		 *	@param reference to the parse master to be copied from
		 */
		XmlParseMaster& operator=(const XmlParseMaster& rhs) = delete;

		/**
		 *	Recreates a parse master with clones of its SharedData and all its helpers
		 *	@return pointer to the newly cloned parse master
		 */
		XmlParseMaster* Clone() const;

		/**
		 *	Adds a parse helper to the list of helpers of this parse master
		 *	@param reference to the parse helper
		 */
		void AddHelper(IXmlParseHelper& helper);

		/**
		 *	Removes the given parse helper from the list of helpers of this parse master
		 *	@param reference to the parse helper to be removed
		 */
		void RemoveHelper(IXmlParseHelper& helper);

		/**
		 *	Parses the given character stream of XML
		 *	@param character stream containing the XML
		 *	@param number of characters in the passed character stream
		 *	@param boolean indicating whether the passed character stream is the last chunk of the XML document to be parsed
		 *	@return true if parsing was successful, false if there was an error
		 */
		bool Parse(const char* document, std::int32_t length, bool isFirstChunk = true, bool isLastChunk = true);

		/**
		 *	Parses the given character stream of XML
		 *	@param path to the file containg the XML
		 *	@return true if parsing was successful, false if there was an error
		 */
		bool ParseFromFile(const std::string& fileName);

		/**
		 *	Accessor for the file name parsed by this parse master
		 *	@return name of file parsed, empty string if no file was assigned
		 */
		const std::string& GetFileName() const;

		/**
		 *	Accessor for the SharedData linked to this parse master
		 *	@return pointer to the shared data
		 */
		SharedData* GetSharedData() const;

		/**
		 *	Mutator for the SharedData linked to this parse master
		 *	@param reference to the shared data
		 */
		void SetSharedData(SharedData& sharedData);

		/**
		 *	Returns the list of all helpers for this Parse Master
		 *	@return pointer to the shared data
		 */
		const Vector<IXmlParseHelper*>& Helpers() const;

	private:

		/**
		 *	Callback for whenever a new element is parsed by Expat
		 *	@param pointer to the user data set in expat (a pointer to the XmlParseMaster in this case)
		 *	@param string containing the element name
		 *	@param array of strings containing the attribute key and values, terminated by an empty null-terminated string
		 */
		static void StartElementHandler(void* userData, const char* elementName, const char** attributes);

		/**
		 *	Callback for whenever a closing tag for an element is parsed by Expat
		 *	@param pointer to the user data set in expat (a pointer to the XmlParseMaster in this case)
		 *	@param string containing the element name
		 */
		static void EndElementHandler(void* userData, const char* elementName);

		/**
		 *	Callback for whenever an element has a simple character stream between its opening and closing tags
		 *	@param pointer to the user data set in expat (a pointer to the XmlParseMaster in this case)
	 	 * 	@param character stream representing the data
		 *	@param length of the character stream
		 */
		static void CharDataHandler(void* userData, const char* characterStream, int length);

		/**
		 *	Resets the xml parser, sets the element handler callbacks
		 */
		void InitializeXmlParser();

		/**
		 *	trims the delimiter string from the end of given string
		 *	@param string to trim
		 * 	@param delimiter to trim, default value is a string containing all whitespace characters
		 */
		std::string& TrimRightInplace(std::string& s, const std::string& delimiters = " \f\n\r\t\v");

		/**
		 *	trims the delimiter string from the start of given string
		 *	@param string to trim
		 * 	@param delimiter to trim, default value is a string containing all whitespace characters
		 */
		std::string& TrimLeftInplace(std::string& s, const std::string& delimiters = " \f\n\r\t\v");

		/**
		 *	trims the given string based on the delimiter string
		 *	@param string to trim
		 * 	@param delimiter to trim, default value is a string containing all whitespace characters
		 */
		std::string& TrimInplace(std::string& s, const std::string& delimiters = " \f\n\r\t\v");

		void OpenFileHandle(const std::string& fileName);
		void CloseTopFileHandle();

		Stack<std::ifstream*> mFileHandles;
		std::uint32_t mFileHandleCounter;

		SharedData* mSharedData;
		Vector<IXmlParseHelper*> mHelpers;

		std::uint32_t mLastClonedHelper;
		bool mIsCloned;

		std::string mFileName;
		XML_Parser mXmlParser;
	};
}

