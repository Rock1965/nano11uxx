/*
 ===============================================================================
 Name        : list.h
 Author      : uCXpresso
 Version     : v1.0.0
 Date		 : 2011/12/28
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : Data List class
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2012/12/28	v1.0.0	First Edition								Jason
 ===============================================================================
 */

#ifndef LIST_H_
#define LIST_H_

#include "class/object.h"
#include "class/mutex.h"

/**A position type.
 */
typedef void* LIST_POS;

/*A data element type
 */
typedef void* ELEM_PTR;

/**CList class provides the data list service.
 * \class CList list.h "class/list.h"
 * \note The CList class will use the CMutex to synchronize the list chain in multi-tasking.
 * \ingroup Miscellaneous
 */
class CList: public CObject {
public:
	/**Call the count() member to retrieve the number of elements in the list.
	 * \return an integer value to indicate the number of elements in the list.
	 */
	virtual int  count();

	/**Call the isEmpty() member to check whether list is empty or not.
	 * \return true if list is empty, otherwise not.
	 */
	virtual bool isEmpty();

	/**Add an element in the head of list.
	 * \param elem is a point to specify data.
	 * \return the position of element in the list. if return NULL, means failed.
	 */
	virtual LIST_POS addHead(ELEM_PTR elem);

	/**Add an element in the tail of list.
	 * \param elem is a point to specify data.
	 * \return the position of element in the list. if return NULL, means failed.
	 */
	virtual LIST_POS addTail(ELEM_PTR elem);

	/**Get an element from the head of list.
	 * \return the element point from head of list, if return NULL, means failed.
	 */
	virtual ELEM_PTR getHead();

	/**Get an element from the tail of list.
	 * \return the element point from tail of list, if return NULL, means failed.
	 */
	virtual ELEM_PTR getTail();

	/**Get an element from list with indicated position.
	 * \param pos is a list position.
	 * \return the element point from the list, if return NULL, means failed.
	 */
	virtual ELEM_PTR getAt(LIST_POS pos);

	/**Get an element from list with indicated index.
	 * \param index is an integer value to inidcate the position.
	 * \return the element point from the list, if return NULL, means failed.
	 */
	virtual ELEM_PTR getAt(int index);

	/**Remove element from head of list.
	 * \return the element point from the list, if return NULL, means failed.
	 */
	virtual ELEM_PTR removeHead();

	/**Remove element from tail of list.
	 * \return the element point from the list, if return NULL, means failed.
	 */
	virtual ELEM_PTR removeTail();

	/**Remove element from list with indicated position.
	 * \return the element point from the list, if return NULL, means failed.
	 */
	virtual ELEM_PTR removeAt(LIST_POS pos);

	/**Remove all element from the list.
	 * \param free_elem is boolean value to indicate whether free the elements memory.
	 * \param isObj is boolean value, if free_elem is true, and isObj is true, the elements will be freed with the CObject type.
	 */
	virtual void  removeAll(bool free_elem=false, bool isObj=false);

	/**Get head position.
	 * \return the LIST_POS
	 */
	virtual LIST_POS getHeadPos();

	/**Get tail position.
	 * \return the LIST_POS
	 */
	virtual LIST_POS getTailPos();

	/**Get next position from current position.
	 * \param pos is LIST_POS to indicate current position.
	 * \return LIST_POS to indicate the next position.
	 */
	virtual LIST_POS getNext(LIST_POS pos);

	/**Get previous position from current position.
	 * \param pos is LIST_POS to indicate current position.
	 * \return LIST_POS to indicate the previous position.
	 */
	virtual LIST_POS getPrev(LIST_POS pos);

	/**Find an element position in the list.
	 * \param elem is a point to element.
	 * \return position (LIST_POS), if return NULL, means not found.
	 */
	virtual LIST_POS find(ELEM_PTR elem);

	/**Insert an element before the indicated position.
	 * \param pos is LIST_POS point to indicate where want to insert the element.
	 * \param elem is an element.
	 * \return position of the new element in the list.
	 */
	virtual LIST_POS insertBefore(LIST_POS pos, ELEM_PTR elem);

	/**Insert an element after the indicated position.
	 * \param pos is LIST_POS point to indicate where want to insert the element.
	 * \param elem is an element.
	 * \return position of the new element in the list.
	 */
	virtual LIST_POS insertAfter(LIST_POS pos, ELEM_PTR elem);

	virtual inline ELEM_PTR operator [](int index) {
		return getAt(index);
	}

	/// @cond
	CList();
	virtual ~CList();
protected:
	CMutex m_mutex;
	LIST_POS m_ndHead;
	LIST_POS m_ndTail;
	int length;

	virtual LIST_POS alloc();
	virtual void free(LIST_POS pos);
	/// @endcond
};

/**Template CList class
 * \class CListT list.h "class/list.h"
 * \ingroup Miscellaneous
 */
template <class CType>
class CListT: public CList {
public:
	virtual inline LIST_POS addHeadT(CType* elem) {
		return CList::addHead(elem);
	}

	virtual inline LIST_POS addTailT(CType* elem) {
		return CList::addTail(elem);
	}

	virtual inline CType* getHeadT() {
		return (CType *)CList::getHead();
	}

	virtual inline CType* getTailT() {
		return (CType*)CList::getTail();
	}

	virtual inline CType* getAtT(LIST_POS pos) {
		return (CType*)CList::getAt(pos);
	}

	virtual inline CType* removeHeadT() {
		return (CType*)CList::removeHead();
	}

	virtual inline CType* removeTailT() {
		return (CType*)CList::removeTail();
	}

	virtual inline void removeAtT(CType* elem) {
		LIST_POS pos;
		pos = find(elem);
		if ( pos!=NULL ) {
			CList::removeAt(pos);
		}
	}

	virtual inline void removeAllT(bool free_elem=false) {
		CType* obj;
		while( (obj=removeHeadT())!=NULL ) {
			if ( free_elem ) {
				delete obj;
			}
		}
	}

	virtual inline LIST_POS insertBeforeT(LIST_POS pos, CType* elem) {
		return CList::insertBefore(pos, elem);
	}

	virtual inline LIST_POS insertAfterT(LIST_POS pos, CType* elem) {
		return CList::insertAfter(pos, elem);
	}
};

#endif /* LIST_H_ */
