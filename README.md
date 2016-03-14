# NihavGameEngine
A data driven game engine, written in C++,using XML as its scripting language, being developed for the programming track curriculum, FIEA (Spring 2016).

## Modules

- Templated Containers
 - Singly Linked List
 - Vector
 - Stack
 - HashMap
- Datum - a run-time dynamic data-type storage
- Scope - forms a dynamic heirarchical database for name-value pairs
- Attributed - base class for "glue" code which links Scopes to native class/object and provides utility facilities such as prescribing the contents of a Scope table at compile time
- XmlParseMaster - XML parser which follows the Chain Of Responsibility pattern to read the xml script and pass it on to relevant helpers for parsing
- IXmlParseHelper - abstract class which forms the interface for helpers of XmlParseMaster

###Upcoming Modules -
- Specialized XML parse helper to create a Scope from an XML script
- Object Factory
- Events and Actions
