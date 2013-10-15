 /*
  PString.cpp - String library for Wiring & Arduino
  Copyright (c) 2009-10 Hernando Barragan.  All rights reserved.
*/

#include "string_support.h"
#include "pstring.h"

/** @brief Contruct a new string from an existing C string
    
    @param const char* cstr: Pointer to existing C string to copy
 */
String::String(const char* cstr)
{
  if (cstr == NULL)
    cstr = "";
  allocateBuffer(m_length = strlen(cstr));
  if (m_buffer != NULL)
    strcpy(m_buffer, cstr);
}


/** @brief Contruct a new string from an existing String object
 
    @param const String& str: Pointer to existing C string to copy
 */
String::String(const String& str)
{
  allocateBuffer(m_length = str.m_length);
  if (m_buffer != NULL)
    strcpy(m_buffer, str.m_buffer);
}


/** @brief Contruct a new string from an existing character
 
 @param const char value: Character value to use for string data
 */
String::String(const char value)
{
  m_length = 1;
  allocateBuffer(1);
  if (m_buffer != NULL) {
    m_buffer[0] = value;
    m_buffer[1] = 0;
  }
}


/** @brief Contruct a new string from an existing character
 
 @param const unsigned char value: Character value to use for string data
 */
String::String(const unsigned char value)
{
  m_length = 1;
  allocateBuffer(1);
  if (m_buffer != NULL) {
    m_buffer[0] = value;
    m_buffer[1] = 0;
  }
}


/** @brief Contruct a new string from an existing numeric value
 
 @param const int value: The numeric value to create the string from
 @param const int base: The base of the number system used to convert the value (2|8|10|16)
 */
String::String(const int value, const int base)
{
  char buf[33];
  sup_itoa((signed long)value, buf, base);
  allocateBuffer(m_length = strlen(buf));
  if (m_buffer != NULL)
    strcpy(m_buffer, buf);
}


/** @brief Contruct a new string from an existing numeric value
 
 @param const unsigned int value: The numeric value to create the string from
 @param const int base: The base of the number system used to convert the value (2|8|10|16)
 */
String::String(const unsigned int value, const int base)
{
  char buf[33];
  sup_uitoa(value, buf, base);
  allocateBuffer(m_length = strlen(buf));
  if (m_buffer != NULL)
    strcpy(m_buffer, buf);
}


/** @brief Contruct a new string from an existing numeric value
 
 @param const long value: The numeric value to create the string from
 @param const int base: The base of the number system used to convert the value (2|8|10|16)
 */
String::String(const long value, const int base)
{
  char buf[33];
  sup_itoa(value, buf, base);
  allocateBuffer(m_length = strlen(buf));
  if (m_buffer != NULL)
    strcpy(m_buffer, buf);
}


/** @brief Contruct a new string from an existing numeric value
 
 @param const unsigned long value: The numeric value to create the string from
 @param const int base: The base of the number system used to convert the value (2|8|10|16)
 */
String::String(const unsigned long value, const int base)
{
  char buf[33];
  sup_uitoa(value, buf, 10);
  allocateBuffer(m_length = strlen(buf));
  if (m_buffer != NULL)
    strcpy(m_buffer, buf);
}


/** @brief Return the character in the string at the given index
 
 @param unsigned int loc: The positive index of the character
 @return char: The character at the requested location
 */
char String::charAt(unsigned int loc) const
{
  return operator[](loc);
}


/** @brief Set the character in the string at the given index
 
 @param unsigned int loc: The positive index of the character
 @param const char aChar: The character to place at loc
 */
void String::setChar(unsigned int loc, const char aChar)
{
  if(m_buffer == NULL) return;
  if(m_length > loc) {
    m_buffer[loc] = aChar;
  }
}


/** @brief Compares this string to another string object.

 @param const String& s2: String object to compare against this object.
 @return int: zero if strings are equal, positive if this string is greater, negative if this string is less than parameter.
 */
int String::compareTo(const String& s2) const
{
  return strcmp(m_buffer, s2.m_buffer);
}


/** @brief Set the data in this string object equal to that of another string object.
 
 @param String& rs: The object who's data will be copied to this object.
 @return String&: Reference to this object.
 */
const String& String::operator=(const String& rs)
{
  if (this == &rs)
    return *this;

  if (rs.m_length > m_length)
  {
    free(m_buffer);
    allocateBuffer(rs.m_length);
  }

  if (m_buffer != NULL) {
    m_length = rs.m_length;
    strcpy(m_buffer, rs.m_buffer);
  }
  return *this;
}


/** @brief Append another string object's data to this string.
 
 @param String& other: The string object who's data will be appended to this string.
 @return String&: Reference to this string.
 */
const String& String::operator+=(const String& other)
{
  m_length += other.m_length;
  if (m_length > m_capacity)
  {
    char *temp = (char *)realloc(m_buffer, m_length + 1);
    if (temp != NULL) {
      m_buffer = temp;
      m_capacity = m_length;
    } else {
      m_length -= other.m_length;
      return *this;
    }
  }
  strcat(m_buffer, other.m_buffer);
  return *this;
}


/** @brief Access a character at the given index within the string data.
 
 @param unsigned int index: Index of the character to return.
 @return char&: Refernce to the charater in the data array.
 */
char& String::operator[](unsigned int index)
{
  static char dummy_writable_char;
  if (index >= m_length || !m_buffer)
  {
    dummy_writable_char = 0;
    return dummy_writable_char;
  }
  return m_buffer[index];
}


/** @brief Access a character at the given index within the string data.
 
 @param unsigned int index: Index of the character to return.
 @return char: Charater in the data array.
 */
char String::operator[](unsigned int index) const
{
  return m_buffer[index];
}


/** @brief Locate a substring within this string object starting from the end of the string.
 
    @param String& s2: The substring to locate in this string.
    @return True if the substring is found at the end, false (0) otherwise.
 */
unsigned char String::endsWith(const String& s2) const
{
  if (m_length < s2.m_length)
    return 0;

  return strcmp(&m_buffer[m_length - s2.m_length], s2.m_buffer) == 0;
}


/** @brief Tests if another string is equal to this string.
 
    @param String& s2: The other string to test for equality.
    @return unsigned char: True if the strings are identical, false (0) otherwise.
 */
unsigned char String::equals(const String& s2) const
{
  return (m_length == s2.m_length && strcmp(m_buffer,s2.m_buffer) == 0);
}


/** @brief Replace all occurances of a particular character in the string. Does not alter the original string data.
 
    @param char findChar: Character to locate.
    @param char replaceChar: Character to replace it with.
    @return String: Object that reperesents the replacement activity.
 */
String String::replace(char findChar, char replaceChar)
{
  if (m_buffer == NULL)
      return *this;
    
  String result = m_buffer;
  char* temp = result.m_buffer;
    
  while((temp = strchr(temp, findChar)) != 0)
    *temp = replaceChar;

  return result;
}


/** @brief Replace all occurances of a particular string in this string with another. Does not alter the original string data.
 
 @param String& orig: Sub-string to locate.
 @param String& replace: Character to replace it with.
 @return String: Object that reperesents the replacement activity.
 */
String String::replace(const String& orig, const String& replace)
{
  if (m_buffer == NULL) return *this;
  String temp = m_buffer, newString;

  int loc;
  while ((loc = temp.indexOf(orig)) != -1)
  {
    newString += temp.substring(0, loc);
    newString += replace;
    temp = temp.substring(loc + orig.m_length);
  }
  newString += temp;
  return newString;
}


/** @brief Return the first position of a particular character in the string.
 
    @param char ch: Character to search for.
    @return int: Zero-based index of the character position in the string.
 */
int String::indexOf(char ch) const
{
  return indexOf(ch, 0);
}


/** @brief Return the position of a particular character in the string from a specified starting point.
 
 @param char ch: Character to search for.
 @param unsigned int fromIndex: The position to begin the search from.
 @return int: Zero-based index of the character position in the string.
 */
int String::indexOf(char ch, unsigned int fromIndex) const
{
  if (fromIndex >= m_length)
    return -1;

  const char* temp = strchr(&m_buffer[fromIndex], ch);
  if (temp == NULL)
    return -1;

  return temp - m_buffer;
}


int String::indexOf(const String &s2) const
{
  return indexOf(s2, 0);
}


int String::indexOf(const String &s2, unsigned int fromIndex) const
{
  if (fromIndex >= m_length)
    return -1;

  const char *theFind = strstr(&m_buffer[fromIndex], s2.m_buffer);

  if (theFind == NULL)
    return -1;

  return theFind - m_buffer; // pointer subtraction
}


int String::lastIndexOf(char theChar) const
{
  return lastIndexOf(theChar, m_length - 1);
}


int String::lastIndexOf(char ch, unsigned int fromIndex) const
{
  if (fromIndex >= m_length)
    return -1;

  char tempchar = m_buffer[fromIndex + 1];
  m_buffer[fromIndex + 1] = '\0';
  char* temp = strrchr(m_buffer, ch);
  m_buffer[fromIndex + 1] = tempchar;

  if (temp == NULL)
    return -1;

  return temp - m_buffer;
}


int String::lastIndexOf(const String &s2) const
{
  return lastIndexOf(s2, m_length - s2.m_length);
}


int String::lastIndexOf(const String &s2, unsigned int fromIndex) const
{
  // check for empty strings
  if (s2.m_length == 0 || s2.m_length - 1 > fromIndex || fromIndex >= m_length)
    return -1;

  // matching first character
  char temp = s2[0];

  for (int i = fromIndex; i >= 0; i--)
  {
    if (m_buffer[i] == temp && (*this).substring(i, i + s2.m_length).equals(s2))
    return i;
  }
  return -1;
}


unsigned char String::startsWith(const String &s2) const
{
  if (m_length < s2.m_length)
    return 0;

  return startsWith(s2, 0);
}


unsigned char String::startsWith(const String &s2, unsigned int offset) const
{
  if (offset > m_length - s2.m_length)
    return 0;

  return strncmp(&m_buffer[offset], s2.m_buffer, s2.m_length) == 0;
}


String String::substring(unsigned int left) const
{
  return substring(left, m_length);
}


String String::substring(unsigned int left, unsigned int right) const
{
  if (left > right)
  {
    int temp = right;
    right = left;
    left = temp;
  }

  if (right > m_length)
    right = m_length;

  char temp = m_buffer[right];  // save the replaced character
  m_buffer[right] = '\0';
  String outPut = (m_buffer + left);  // pointer arithmetic
  m_buffer[right] = temp;  //restore character
  return outPut;
}


String String::trim() const
{
  if (m_buffer == NULL) return *this;
  String temp = m_buffer;
  unsigned int i,j;

  for (i = 0; i < m_length; i++)
  {
    if (!isspace(m_buffer[i]))
      break;
  }

  for (j = temp.m_length - 1; j > i; j--)
  {
    if (!isspace(m_buffer[j]))
      break;
  }

  return temp.substring(i, j + 1);
}


void String::getBytes(unsigned char *buf, unsigned int bufsize) const
{
  if (!bufsize || !buf) return;
  unsigned int len = bufsize - 1;
  if (len > m_length) len = m_length;
  strncpy((char *)buf, m_buffer, len);
  buf[len] = 0;
}


void String::toCharArray(char *buf, unsigned int bufsize) const
{
  if (!bufsize || !buf) return;
  unsigned int len = bufsize - 1;
  if (len > m_length) len = m_length;
  strncpy(buf, m_buffer, len);
  buf[len] = 0;
}


int String::toInt() const
{
  return atol(m_buffer);
}



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
