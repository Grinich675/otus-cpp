#pragma once
#include "../Document.hpp"
#include "BaseCommand.hpp"

#include <list>
#include <map>

/** \defgroup CommandHistory Command history object
 * \brief This group contains object, which used for storing given user commands
 *  @{
 */

/**
 \brief Object for storing user commands
 \details
 Class used for storing,tracking,applying and reverting user commands to bounded document.
 */
class CommandHistory
{
	using history_iter= std::list<Command*>::iterator;

	/**
	 \brief Internal storage for storing sequence of applied commands
	 */
	std::list<Command*> history{};

	/**
	 \brief Iterator for tracking last applied command
	 */
	history_iter last_applied;

	Document* doc;

	/**
	 \brief Internal object for storing associations
	 \details
	 Associates external object to ones, stored inside document internal storage.
	 */
	std::map<VObject*,VObject*> obj_cache{};


	void HandleNewCommand(Command* cmd)
	{
		if(history.size() == 0)
		{
			history.push_back(cmd);
			last_applied=history.begin();
			return;
		}

		if( last_applied!= --history.end() )
		{

			for(auto _old_cmd=std::next(last_applied); _old_cmd!=history.end(); ++_old_cmd)
			{
				delete (*_old_cmd) ;
				//history.erase(_old_cmd);
			}

			history.erase(std::next(last_applied),history.end());
		}

		history.push_back(cmd);

		++last_applied;
	};

public:

	CommandHistory()=delete;

	/**
	 \brief Constructs history of commands for given Document.

	\details
	 Constructs object for storing commands bounded with given doc.

	\param doc_ given document
	*/
	CommandHistory(Document* doc_):last_applied(nullptr), doc(doc_)
	{};

	/**
	 \brief Destroys history,reverting all applied transformation to bounded document.

	\details
	 Function destroys document history, restoring state as it was before applying all applied transformation.
	*/
	~CommandHistory()
	{
		for(auto it = history.begin();it!=history.end();++it)
		{
			delete *it;
		}
	}

	/**
	 \brief Function for reverting last applied transformation to given document.

	\details
	 Function restores document state as it was before applying last applied transformation.
	*/
	void RevokeLast()
	{
		auto cmd = *last_applied;
		cmd->revert(*doc);
		--last_applied;
	};

	/**
	 \brief Function for adding object to bounded document.
	 \details
	 Function adds given object to bounded document on top layer. Command added to tracking list.
	\param element given object
	*/
	void AddElement(VObject* element)
	{
		auto cmd=new AddCommand(element);
		HandleNewCommand(cmd);
		cmd->apply(*doc);
	};

	/**
	 \brief Function for removing object from bounded document by its coordinates.
	 \details
	 Function removes object from bounded document using coordinates. Command added to tracking list.
	\param point coordinates for search
	*/
	void RemoveElement(Coordinates point)
	{
		auto elemnt = doc->select(point);
		auto cmd = new RemoveCommand(elemnt);
		HandleNewCommand(cmd);
		cmd->apply(*doc);
	};

	/**
	 \brief Function for removing object from bounded document.
	 \details
	 Function removes given object from bounded document, if it have one. Command added to tracking list.
	\param element given object to remove
	*/
	void RemoveElement(VObject* element)
	{
		auto cmd = new RemoveCommand(element);
		HandleNewCommand(cmd);
		cmd->apply(*doc);
	};

	/**
	 \brief Function for for searching object in bounded document.
	 \details
	 Function searches for an object containing given coordinates in document, considering objects level.
	 Command added to tracking list.
	\param point given object for search
	\return cloned internal object. association added to internal cache
	*/
	VObject* SelectElement(Coordinates point)
	{
		auto cmd = new SelectCommand(point);
		VObject* internal_obj = cmd->apply(*doc);
		VObject* new_external = internal_obj->clone();
		obj_cache.insert(std::make_pair(new_external,internal_obj));

		delete cmd;

		return new_external;
	};

	/**
	 \brief Function for editing given object in bounded document.

	\details
	 Function searches association for an external object if there is one,
	 clones current state of object in bounded document and assigns new data from edited version to it.
	 Command added to tracking list.

	\param external_old 	 given external object to replace
	\param external_edited   external object which contains replaced data
	*/
	VObject* ChangeAttributes(VObject* external_old,VObject* external_edited)
	{
		auto old_internal = obj_cache.find(external_old);

		if (old_internal != obj_cache.end())
		{
			auto cmd = new ChangeAttribCommand(old_internal->second,external_edited);
			HandleNewCommand(cmd);
			cmd->apply(*doc);
		}

		return external_old;
	};


};
/** @} */
