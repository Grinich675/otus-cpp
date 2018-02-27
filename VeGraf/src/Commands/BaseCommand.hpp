#pragma once
#include "../VecPrimitives.h"
#include "../Document.hpp"

/** \defgroup Commands Document transformation types
 * \brief This group contains of all possible document transformation types
 *  @{
 */

/**
 \brief Describes all available command types
 */
enum class CommandType
{
	Select,				/**< Searches object in document     */
	Add,				/**< Add object to document    		 */
	Remove,				/**< Removes object from document    */
	Resize,				/**< Changes object size    		 */
	Move,				/**< Changes object center    		 */
	ChangeOrder,		/**< Changes object layouts    		 */
	ChangeAttributes	/**< Changes object attributes       */
};

/**
 \brief Base class of transformations
 \details
 Class used as base for all transformations.
 Every transformation inherits from it.
 */
class Command
{
public:

	Command()=default;

	virtual ~Command()=default;

	/**
	 \brief Function for applying transformation to given document.

	\details
	 Function applies transformation to given document.

	\param Document given document

	\return VObject* object that was added or edited in document
	*/
	virtual VObject* apply(Document& )=0;

	/**
	 \brief Function for reverting transformation in given document.

	\details
	 Function restores document state as it was before applying transformation .

	\param Document given document
	*/
	virtual void revert(Document& )=0;
};

/**
 \brief Class of command for selecting object in document
 \details
 Class used for searching object in document by coordinates.
 */
class SelectCommand: public Command
{

	VObject* selected; /**< Object,that was found by given coordinates    */
	Coordinates cord; /**< Coordinates used for search    				  */
public:

	SelectCommand()=delete;

	/**
	 \brief Constructs command for searching object by coordinates.

	\details
	 Constructs command, which searches for an object containing given coordinates, depending on objects level in document.

	\param _cord given coordinates
	*/
	SelectCommand(Coordinates& _cord):selected(nullptr),cord(_cord)
	{};

	~SelectCommand()=default;


	VObject* apply(Document& doc) override
	{
		selected = doc.select(cord);
		return selected;
	};


	void revert(Document& ) override {};
};

/**
 \brief Class of command for adding object to given document
 \details
 Class used for adding given object to given document. Object will be placed on top level in document.
 */
class AddCommand: public Command
{
	VObject* added;
public:

	AddCommand()=delete;

	/**
	 \brief Constructs command for adding given object.

	\details
	 Constructs command, which clones given object and adds to given document.

	\param obj  object to add
	*/
	AddCommand(VObject* obj):added(obj->clone()){};

	~AddCommand()
	{
		delete added;
	};

	/**
	 \brief Function for applying transformation to given document.

	\details
	 Function adds bounded cloned object to given document on top layer.

	\param doc given document
	*/
	VObject* apply(Document& doc) override
	{
		doc.add(added);
		return added;
	};

	/**
	 \brief Function for reverting transformation in given document.

	\details
	 Function removes added object from given document .

	\param doc given document
	*/
	void revert(Document& doc) override
	{
		doc.remove(added);
	};
};

/**
 \brief Class of command for removing object in given document
 \details
 Class used for removing given object in given document, if it has one.
 */
class RemoveCommand: public Command
{
	VObject* removed;
	document_iterator position;
public:

	RemoveCommand()=delete;

	/**
	 \brief Constructs command for removing given object.

	\details
	 Constructs command, which removes given object, if given document have such.

	\param _remove  object to remove
	*/
	RemoveCommand(VObject* _remove):removed(_remove)
	{};

	~RemoveCommand()=default;

	/**
	 \brief Function for applying transformation in given document.

	\details
	 Function removes object from given document, if it has one .

	\param doc given document
	*/
	VObject* apply(Document& doc) override
	{
		position= doc.remove(removed);
		return removed;
	};

	/**
	 \brief Function for reverting transformation in given document.

	\details
	 Function adds removed element considering position to given document.

	\param doc given document
	*/
	void revert(Document& doc) override
	{
		if(removed!= nullptr)
			doc.add(position,removed);
	};
};

/**
 \brief Class of command for editing object in given document
 \details
 Class used for editing given object in given document, if it has one.
 */
class ChangeAttribCommand: public Command
{
	VObject* deep_copy_old;
	VObject* old;
	VObject* edited;

public:


	ChangeAttribCommand()=delete;

	/**
	 \brief Constructs command for editing given object.

	\details
	 Constructs command, which replaces given current object in document to edited one.

	\param current_version  object to replace
	\param edited_version   object which will replace
	*/
	ChangeAttribCommand(VObject* current_version, VObject* edited_version):deep_copy_old(nullptr),old(current_version), edited(edited_version)
	{
	};

	~ChangeAttribCommand()
	{
		if(deep_copy_old!=nullptr)
			delete deep_copy_old;
	};

	/**
	 \brief Function for applying transformation in given document.

	\details
	 Function clones current state of object and assigns new data from edited version to it.

	\param doc given document
	*/
	VObject* apply(Document& doc) override
	{

		deep_copy_old=old->clone();

		old->Assign(edited);

		return old;
	};

	/**
	 \brief Function for reverting transformation in given document.

	\details
	 Function restores object state as it was before applying transformation .

	\param Document given document
	*/
	 void revert(Document& ) override
	{

		 if(deep_copy_old==nullptr)
			 return;

		 edited=old->clone();
		 old->Assign(deep_copy_old);
		 delete deep_copy_old;
		 deep_copy_old = edited;
	};
};


/** @} */
