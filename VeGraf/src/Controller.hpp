#include "ObjectAttributes.hpp"
#include "serializers/Serializers.hpp"
#include "VecPrimitives.h"
#include "Document.hpp"
#include "Commands/BaseCommand.hpp"
#include "Commands/CommandHistory.hpp"

/** \defgroup Controller Document Controller
 *  \brief This group contains controller used for interacting with Document,CommandHistory,Commands objects
 *  @{
 */



/**
 \brief Class defines Controller user interaction
 \details
  Class contains all needed methods to interact with Document,CommandHistory,Commands objects.
  Ties document its command history and serializer for autosaving
 */
class VeGrafController
{
	Document* doc;

	CommandHistory* history;

	Serializer* autosaver;


public:

	/**
	\brief Constructs controller.

	\details
	 Constructs controller which bounded to empty(newly created) doc, its history and JSON serializer for autosaving.
	 Redraws working plane.
	*/
	VeGrafController()
	{
		doc = new Document();
		history= new CommandHistory(doc);
		autosaver= new JsonSerializer();

		doc->Draw();
	};

	/**
	 \brief Constructs controller.

	\details
	 Constructs controller which bounded to opened(given) doc, its empty history and serializer for autosaving.
	 Redraws working plane.
	\param saved_doc serialized document
	\param autosaver_ serialization object for autosaving
	*/
	VeGrafController(Serializer* saved_doc,Serializer* autosaver_)
	{
		doc= new Document();

		doc->open(saved_doc);

		history= new CommandHistory(doc);

		autosaver= autosaver_;

		doc->Draw();
	};

	~VeGrafController()
	{
		delete history;
		delete doc;
		delete autosaver;
	};

	/**
	 \brief Bounds Controller with another serialized document.

	\details
     Clears current bounded document,its history and recreates it from serialized document.
	 Redraws working plane.
	\param saved_doc serialized document
	*/
	void Open(Serializer* new_doc)
	{

		delete history;
		doc->clear();
		doc->open(new_doc);
		history= new CommandHistory(doc);
		doc->Draw();
	};

	/**
	 \brief Closes current bounded document.
	*/
	void Close()
	{

	};

	/**
	\brief Saves current bounded document object, using given serializer.
	\param dst serializer object
	*/
	void Save(Serializer* dst)
	{
		doc->save(dst);
	};

	/**
	 \brief Saves current bounded document object, using bounded serializer.
	*/
	void AutoSave()
	{
		doc->save(autosaver);
	};

	/**
	\brief Function for clearing bounded document and its history.
	\details
	 Function clears bounded document,history and redraws working plane.
	*/
	void Clear()
	{
		delete history;
		doc->clear();
		history = new CommandHistory(doc);
		doc->Draw();
	};

	/**
	\brief Function for redrawing working plane.
	\details
	 Function clears redraws working plane.
	*/
	void Draw()
	{
		doc->Draw();
	};

	/**
	 \brief Function for adding object to bounded document.
	 \details
	 Function adds given object to bounded document on top layer. Command added to tracking list.
	 Redraws working plane.
	\param element given object
	*/
	void Add(VObject* el)
	{
		history->AddElement(el);
		doc->Draw();
	};

	/**
	 \brief Function for removing object from bounded document by its coordinates.
	 \details
	 Function removes object from bounded document using coordinates. Command added to tracking list.
	 Redraws working plane.
	\param point coordinates for search
	*/
	void Delete(Coordinates point)
	{
		history->RemoveElement(point);
		doc->Draw();
	};

	/**
	 \brief Function for for searching object in bounded document.
	 \details
	 Function searches for an object containing given coordinates in document, considering objects level.
	 Command added to tracking list.
	\param point given object for search
	\return cloned internal object. association added to internal cache
	*/
	VObject* Select(Coordinates point_)
	{
		return history->SelectElement(point_);
	};

	/**
	 \brief Function for editing given object in bounded document.

	\details
	 Function searches association for an external object if there is one,
	 clones current state of object in bounded document and assigns new data from edited version to it.
	 Command added to tracking list.
	 Redraws working plane.
	\param old 	 given external object to replace
	\param _new   external object which contains replaced data
	*/
	void Change(VObject* old, VObject* _new)
	{
		history->ChangeAttributes(old,_new);
		doc->Draw();
	};

	/**
	 \brief Function for reverting last applied transformation to bounded document.
	\details
	 Function restores document state as it was before applying last applied transformation.
	 Redraws working plane.
	*/
	void Undo()
	{
		history->RevokeLast();
		doc->Draw();
	};
};

/** @} */
