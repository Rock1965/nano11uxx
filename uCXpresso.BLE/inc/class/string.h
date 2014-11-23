/*
 ===============================================================================
 Name        : string.h
 Author      : uCXpresso
 Version     : v1.1.00
 Date		 : 2014/11/22
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : Provide String operators
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+--------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+--------------
 2012/2/1	v1.0.0	First Edition									Jason
 2012/8/15	v1.0.1	Add two operators								Jason
 	 	 	 	 		+= (TCHAR)
 	 	 	 	 		+= (LPCTSTR)
 2012/8/30	v1.0.2	Add lock flag in getBuffer						Jason
 	 	 	 	 	 	 to keep string memory, don't auto destroy.
 2012/9/19	v1.0.3	Add 3 operators									Jason
  	 	 	 	 	 	+= (int)
  	 	 	 	 	 	+= (double)
  	 	 	 	 	 	+= (uint32_t)
 2013/4/5	v1.0.4	Rename member function params to field			Jason
 	 	 	 	 	Rename member function paramsInt to fieldInt
 	 	 	 	 	Rename member function paramsFloat to fieldFloat
  	 	 	 	 	Add copy string with length
 2013/5/21	v1.0.5  Fixed bugs:										Jason
 	 	 	 	 	 makeUpper()
 	 	 	 	 	 makeLower()
 	 	 	 	 	 copy()
 2014/11/22	v1.1.0	Add parseInt(), parseHex() and parseFloat()		Jason
 	 	 	 	 	member functions.
  ===============================================================================
 */

#ifndef STRING_HPP_
#define STRING_HPP_

#include "class/smallprintf.h"

#define DEF_STRBUF_SIZE	32

/**
 * @brief a string class inherit from CSmallPrintf.
 * @class CString string.h "class/string.h"
 * @ingroup Miscellaneous
 */
class CString: public CSmallPrintf {
public:
	/**
	 * @brief Constructs CString objects with buffer size.
	 * @param size Set the string buffer size. (default size=32 bytes)
	 */
	CString(int size=DEF_STRBUF_SIZE);

	/**
	 * @brief Constructs CString object and copy from other string object.
	 */
	CString(const CString &str);

	/**
	 * @brief Constructs CString object and copy from an exist string.
	 * @param str	 Pointer to a string.
	 * @param length Indicate the string length. auto string length if length is zero.
	 */
	CString(LPCTSTR str, int length=0);

	/**
	 * @brief Constructs CString object and copy from an exist string.
	 * @param buf Pointer to a string.
	 */
	CString(LPTSTR str);

	/**
	 * @brief Retrieve the buffer size of string.
	 */
	virtual int		getBufSize();

	/**
	 * @brief Retrieve the string buffer pointer.
	 */
	virtual LPTSTR 	getBuffer(bool lock=false);

	/**
	 * @brief Returns the number of characters in the CString object.
	 */
	virtual int 	length();

	/**
	 * @brief Clears the CString object
	 */
	virtual void 	clear();

	/**
	 * @brief Return the character at a given position.
	 * @param index	Zero-base index of the character in the CString object.
	 */
	virtual TCHAR 	getAt(int index);

	/**
	 * @brief Sets a character at a given position.
	 * @param index	Zero-base index of the character in the CString object
	 * @param ch	The character to insert.
	 */
	virtual void	setAt(int index, TCHAR ch);

	/**
	 * @brief Copys an exist CString object to the object.
	 */
	virtual int		copy(CString &str);

	/**
	 * @brief Copys an exist string to the object.
	 */
	virtual int		copy(LPCTSTR str, int length);

	/**
	 * @brief Clones a new CString object from the object.
	 */
	virtual CString clone();

	/**
	 * @brief Converts all the characters in this string to uppercase characters.
	 */
	virtual LPCTSTR makeUpper();

	/**
	 * @brief Converts all the characters in this string to lowercase characters.
	 */
	virtual LPCTSTR makeLower();

	/**
	 * @brief Reverses the characters in this string.
	 */
	virtual LPCTSTR makeReverse();

	/**
	 * @brief Extracts the middle part of a string.
	 * @param first	Zero-base index of the first character in this CString object that is to be included in the extracted substring.
	 * @param count	The number of characters to extract from this CString object.
	 */
	virtual CString mid(int first, int count);

	/**
	 * @brief Finds a substring inside a larger string.
	 * @param str	A substring to search for.
	 * @param start	The index of the character in the string to begin the search with, or 0 to start from the beginning.
	 * @returnThe zero-based index of the first character in this CString object that matches the requested substring or characters;
	 * 	-1 if the substring or character is not found.
	 */
	virtual int find(LPCTSTR str, int start=0);

	/**
	 * @brief Finds a character inside a larger string.
	 * @param ch	A single character to search for.
	 * @param start	The index of the character in the string to begin the search with, or 0 to start from the beginning.
	 * @returnThe zero-based index of the first character in this CString object that matches the requested substring or characters;
	 * 	-1 if the substring or character is not found.
	 */
	virtual int find(TCHAR ch, int start=0);

	/**
	 * @brief Finds a substring array inside a larger string.
	 * @param strs	A substring array to search for.
	 * @param start	The index of the character in the string to begin the search with, or 0 to start from the beginning.
	 * @returnThe zero-based index of the first substring in this CString object that matches the requested substring;
	 * 	-1 if the substring is not found.
	 */
	virtual int find(LPCTSTR const strs[], int count, int start=0);

	/**
	 * @brief Finds a substring inside a larger string without case sensitive.
	 * @param str	A substring to search for.
	 * @param start	The index of the character in the string to begin the search with, or 0 to start from the beginning.
	 * @returnThe zero-based index of the first character in this CString object that matches the requested substring or characters;
	 * 	-1 if the substring or character is not found.
	 */
	virtual int findIC(LPCTSTR str, int start=0);

	/**
	 * @brief Converts the string to integer value.
	 * @param start The index of the character in the string to begin the parse with, or 0 to start from the beginning.
	 * @return long type value.
	 */
	virtual long 	parseInt(int start=0);

	/**
	 * @brief Converts the hex string to integer value.
	 * @param start The index of the character in the string to begin the parse with, or 0 to start from the beginning.
	 * @return long type value
	 */
	virtual long 	parseHex(int start=0);

	/**
	 * @brief Converts the hex string to integer value.
	 * @param start The index of the character in the string to begin the parse with, or 0 to start from the beginning.
	 * @return float type value
	 */
	virtual float	parseFloat(int start=0);

	/**
	 * @brief Uses a new string to replace an old string in th CString object.
	 * @param oldstr The old string to replace for.
	 * @param newstr The new string.
	 * @return Number of substring be replaced.
	 */
	virtual int replace(LPCTSTR oldstr, LPCTSTR newstr);

	/**
	 * @brief Decodes the URI (web) string.
	 */
	virtual int uriDecode(LPCTSTR uri);

	//
	// shorthand operators
	//
	virtual TCHAR 	operator [](int index);
	virtual void 	operator = (CString &str);
	virtual void	operator = (LPCTSTR str);

	virtual CString& operator + (CString &str);
	virtual CString& operator + (TCHAR ch);
	virtual CString& operator + (LPCTSTR str);

	virtual void operator += (CString &str);
	virtual void operator += (TCHAR ch);
	virtual void operator += (LPCTSTR str);

	virtual inline void operator += (int val) {
		putv(val);
	}
	virtual inline void operator += (double val) {
		putv(val);
	}
	virtual inline void operator +=  (uint32_t val) {
		putv((unsigned)val);
	}

	virtual bool	 operator == (CString &str);
	virtual bool	 operator == (LPCTSTR str);

	virtual inline LPCTSTR operator *() {
			return getBuffer();
	}
	virtual inline operator LPCTSTR () {
		return getBuffer();
	}

	//
	///@cond PRIVATE
	//
	virtual ~CString();
	virtual int		chkLength();
	virtual void 	freeExtra();

protected:
	LPTSTR	m_buffer;
	int		m_length;
	int		m_size;

	// virtual functions for class smallprintf
	virtual void alloc(int bufsize=DEF_STRBUF_SIZE);
	virtual int realloc(int chkStrLen);
	virtual void free();
	virtual int putc(int ch);
	virtual int getc(){return 0;}
	inline virtual LPCTSTR gets() {
		return (LPCTSTR) getBuffer();
	}
	void moveL(int first, int count);
	void moveR(int first, int count);
	///@endcond
};
#endif /* STRING_H_ */
