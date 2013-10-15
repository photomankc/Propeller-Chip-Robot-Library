/*
  PString.h - String class for Propeller

  Modified from Arduino library 8/15/2012 for use with Parallax Propeller
  2013 - Kyle Crane

  Copyright (c) 2009-10 Hernando Barragan.  All rights reserved.
*/


#ifndef _PSTRING_H_
#define _PSTRING_H_

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

class String
{
protected:
    char*           m_buffer;          // the string data array
    unsigned int    m_capacity;        // the array length minus the null
    unsigned int    m_length;          // the String length minus the null

public:
    String( const char *cstr = "" );
    String( const String &str );
    String( const char );
    String( const unsigned char );
    String( const int, const int base=10);
    String( const unsigned int, const int base=10 );
    String( const long, const int base=10 );
    String( const unsigned long, const int base=10 );
    ~String() { free(m_buffer); m_length = m_capacity = 0;}


    ////////////////////////////////////////////////////////////////////
    // Overloaded operators
    //
    const String & operator = ( const String &rs );
    const String & operator +=( const String &rs );
    int    operator ==( const String &rs ) const;
    int    operator !=( const String &rs ) const;
    int    operator < ( const String &rs ) const;
    int    operator > ( const String &rs ) const;
    int    operator <=( const String &rs ) const;
    int    operator >=( const String &rs ) const;
    char   operator []( unsigned int index ) const;
    char&  operator []( unsigned int index );
    friend String     operator + ( String ls, const String &rs );


    /////////////////////////////////////////////////////////////////////
    // Basic methods
    //
    char            charAt( unsigned int index ) const;
    int             compareTo( const String &anotherString ) const;
    unsigned char   endsWith( const String &suffix ) const;
    unsigned char   equals( const String &anObject ) const;
    int             indexOf( char ch ) const;
    int             indexOf( char ch, unsigned int fromIndex ) const;
    int             indexOf( const String &str ) const;
    int             indexOf( const String &str, unsigned int fromIndex ) const;
    int             lastIndexOf( char ch ) const;
    int             lastIndexOf( char ch, unsigned int fromIndex ) const;
    int             lastIndexOf( const String &str ) const;
    int             lastIndexOf( const String &str, unsigned int fromIndex ) const;
    const unsigned int length( ) const { return m_length; }
    void            setChar(unsigned int index, const char ch);
    unsigned char   startsWith( const String &prefix ) const;
    unsigned char   startsWith( const String &prefix, unsigned int toffset ) const;
    String          substring( unsigned int beginIndex ) const;
    String          substring( unsigned int beginIndex, unsigned int endIndex ) const;
    String          trim( ) const;
    void            getBytes(unsigned char *buf, unsigned int bufsize) const;
    void            toCharArray(char *buf, unsigned int bufsize) const;
    int             toInt() const;
    char*           c_str() {return m_buffer;};
    const String&   concat( const String &str );
    String          replace( char oldChar, char newChar );
    String          replace( const String& orig, const String& replace );

protected:
    void            allocateBuffer(unsigned int maxStrLen);
};

/** @brief Allocate enough memory to contain a string of a given length
    
    @param unsigned int maxStrLen: The number of bytes to allocate for string storage
 */
inline void String::allocateBuffer(unsigned int maxStrLen)
{
  m_capacity = maxStrLen;
  m_buffer = (char *) malloc(m_capacity + 1);
  if (m_buffer == NULL) m_length = m_capacity = 0;
}


/** @brief Append one string to another.
 
    @param String ls: Left side of the operation.
    @param String rs: Right side of the operation.
    @return String: Appended final string.
 */
inline String operator+( String ls, const String &rs )
{
  return ls += rs;
}


/** @brief compare this string to another and return true if they are equal.
 
 @param String rs: The string to compare against.
 @return int: Value representing the result of the string comparison, true if left == right.
 */
inline int String::operator==( const String &rs ) const
{
    return ( m_length == rs.m_length && strcmp( m_buffer, rs.m_buffer ) == 0 );
}


/** @brief compare this string to another and return true if they are not equal.
 
 @param String rs: The string to compare against.
 @return int: Value representing the result of the string comparison, true if left != right.
 */
inline int String::operator!=( const String &rs ) const
{
    return ( m_length != rs.length() || strcmp( m_buffer, rs.m_buffer ) != 0 );
}


/** @brief compare this string to another and return the result.
 
 @param String rs: The string to compare against.
 @return int: Value representing the result of the string comparison, true if left < right.
 */
inline int String::operator<( const String &rs ) const
{
    return strcmp( m_buffer, rs.m_buffer ) < 0;
}


/** @brief compare this string to another and return the result.
 
    @param String rs: The string to compare against.
    @return int: Value representing the result of the string comparison, true if left > right.
 */
inline int String::operator>( const String &rs ) const
{
    return strcmp( m_buffer, rs.m_buffer ) > 0;
}


/** @brief compare this string to another and return the result.
 
    @param String rs: The string to compare against.
    @return int: Value representing the result of the string comparison, true if left <= right.
 */
inline int String::operator<=( const String &rs ) const
{
    return strcmp( m_buffer, rs.m_buffer ) <= 0;
}


/** @brief compare this string to another and return the result.
 
    @param String rs: The string to compare against.
    @return int: Value representing the result of the string comparison, true if left >= right.
 */
inline int String::operator>=( const String & rs ) const
{
    return strcmp( m_buffer, rs.m_buffer ) >= 0;
}


/** @brief Append the given string to this string returning a new string
 
 @param const String& s2: String object to append to this string.
 @return const String&: Refernce to this th string.
 */
inline const String& String::concat( const String &s2 )
{
    return (*this) += s2;
}

#endif

/*
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
