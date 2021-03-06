/*
 ===============================================================================
 Name        : gc.h
 Author      : uCXpresso
 Version     : v1.0.2
 Date		 : 2014/3/26
 Copyright   : Copyright (C) www.embeda.com.tw
 License	 : MIT
 Description : Simple Garbage Collector Template Class
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/3/24	v1.0.0	First Edition									Jason
 2014/3/25	v1.0.1	Add Mutex Lock for Multi-Tasking.				Jason
 2014/3/26	v1.0.2	Add [] operator for Array.						Jason
 ===============================================================================
 */
#ifdef GC_TEMPLATE
#ifndef gcHandle_H_
#define gcHandle_H_

#include <class/object.h>
#include <class/mutex.h>

#define GC_FOR_MULTITASKING		1

/**GC Base Class
 * \class _gc_ gc.h "class/gc.h"
 * \ingroup RTOS
 */
class _gc_ : public CObject {
public:
	virtual void reference() = PURE_VIRTUAL_FUNC;	// increase reference count
	virtual void release() = PURE_VIRTUAL_FUNC;		// decrease reference count

#if GC_FOR_MULTITASKING
	/**Lock resource.
	 */
	virtual inline void lock() 		{ if ( m_gc ) m_gc->mutex.lock(); }

	/**Unlock resource.
	 */
	virtual inline void unlock()	{ if ( m_gc ) m_gc->mutex.unlock(); }
#endif

protected:
	typedef struct {
		#if GC_FOR_MULTITASKING
			CMutex	mutex;		// mutex lock for multi-tasking
		#endif
		int			refcount;	// reference count
		xHandle 	elem;		// user's data point
	}GC_T;
	GC_T	*m_gc;
};

/**gcHandleT is a template class to provide a Simple Garbage Collector
 * \class gcHandleT gc.h "class/gc.h"
 * \ingroup RTOS
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
	 *  \return element
	 */
	virtual CType& operator * ();

	/**Operator [] array to return the element contents.
	 *  \return element
	 */
	virtual CType& operator [] (int index);

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

/** \example /misc/garbage_collector/src/main.cpp
 * This is an example of how to use the gcHandleT class.
 * More details about this example.
 */
#endif /* gcHandle_H_ */

#else
#include "template/gc.cpp"
#endif // ELSE GC_TEMPLATE

