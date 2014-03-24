/*
 ===============================================================================
 Name        : gc.h
 Author      : uCXpresso
 Version     : v1.0.0
 Date		 : 2014/3/24
 Copyright   : Copyright (C) www.embeda.com.tw
 License	 : MIT
 Description : Simple Garbage Collector Template Class
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/3/24	v1.0.0	First Edition									Jason
 ===============================================================================
 */
#ifdef GC_TEMPLATE
#ifndef gcHandle_H_
#define gcHandle_H_

#include <class/object.h>

#define NOTHING	((xHandle) NULL)

/**GC Base Class
 */
class _gc_ : public CObject {
public:
	virtual void reference() = PURE_VIRTUAL_FUNC;	// increase reference count
	virtual void release() = PURE_VIRTUAL_FUNC;		// decrease reference count

protected:
	typedef struct {
		int		refcount;
		xHandle elem;
	}GC_T;
	GC_T	*m_gc;
};

/**gcHandleT is a template class to provide a Simple Garbage Collector
 * \class gcHandleT gc.h "class/gc.h"
 * \ingroup Miscellaneous
 */
template <class CType>
class gcHandleT: public _gc_ {
public:

	//
	// Constructors
	//

	/**Construct both initialized and uninitialized the object.
	 */
	gcHandleT(CType *elem=NULL);

	/**Constructor with reference to other gcHandleT<CType>
	 * \param gc is gcHandleT object
	 */
	gcHandleT(const gcHandleT<CType> &gc);

	//
	// Assigned Operators
	//

	/**Operator '=' to refer to a gcHandleT object and increase the reference count.
	 * \param gc is gcHandleT object
	 */
	virtual gcHandleT<CType>& operator = (gcHandleT<CType> &gc);

	/**Operator '=' to refer to new element
	 */
	virtual gcHandleT<CType>& operator = (CType* elem);

	/**Operator '=' to refer to GC_T point
	 */
	virtual void operator = (xHandle gc);

	//
	// Retrieved Operators
	//

	/**Operator '->', to return the element point
	 * \return element point
	 */
	virtual CType* operator -> ();

	/**Operator 'CType*', to return the element point
	 * \return element point
	 */
	virtual operator CType* ();

	/**Operator '*' to return the element contents.
	 *  \return data contents
	 */
	virtual CType& operator * ();

	/**Operator '&' to return the GC_T point.
	 */
	virtual xHandle operator & ();

	/**Operator xHandle to return the GC_T point.
	 */
	virtual operator xHandle();

	//
	// Boolean Operators
	//

	bool operator == (gcHandleT<CType> &gc);
	bool operator != (gcHandleT<CType> &gc);
	bool operator == (CType *elem);
	bool operator != (CType *elem);

	//
	// Miscellaneous
	//

	/**Reference to element
	 */
	virtual void reference();

	/**Releases element
	 */
	virtual void release();

	/**Get the reference count.
	 * \return reference count.
	 */
	virtual int count();

	/**Refers to a gcHandleT object without reference count
	 * \param gc is gcHandleT object
	 */
	virtual void weakTo(gcHandleT<CType> &gc);

	/**Clone a new gcHandleT
	 * \code
	 *
	 * // clone p1 to p2
	 * p2 = p1.clone();
	 *
	 * \endcode
	 * \return a new GC_T
	 */
	virtual xHandle clone();

	/**Check data valid
	 */
	inline bool isValid() {
		return (count() ? true : false);
	}

	/**Operator for data valid checked.
	 */
	inline operator bool () {
		return isValid();
	}

	//
	// Internal used
	//

	/// \cond
	virtual ~gcHandleT();

	/// \endcond
};
#endif /* gcHandle_H_ */

#else
#include "template/gc.cpp"
#endif // ELSE GC_TEMPLATE

