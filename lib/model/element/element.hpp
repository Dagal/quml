/*******************************************************************
* Copyright (c) 2009 Thomas Fannes (thomasfannes@gmail.com)
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following
* conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*******************************************************************/

#ifndef DEFINES_HPP
#define DEFINES_HPP

#include <boost/shared_ptr.hpp>

/*!
  \namespace element
  \brief This namespace contains all the basic elements for the UML model.
*/
namespace element
{
	class UMLDiagram;
	class ClassObject;
	class PackageObject;
	class DatatypeObject;
	class ElementObject;
	class OperationObject;
	class Parameter;
	class PropertyObject;

	//! This enum specifies the type of the element
	/*!
	  The numeric value of each of the enumerated values is of important for casting the elements.
	  An element A is castable to type B if the following is valid:  A::elementtype & B == B.
	  It can be seen that a ClassObject and a PrimitiveObject are castable to a DatatypeObject,
	  a OperationObject is castable to a MethodObject and a PropertyObject to a ParameterObject.
	*/
	enum ElementType
	{
		Element = 0 /*!< Basic element with binary value 00000000 */,
		Element_Package = 1 /*!< Package element with binary value 00000001 */,
		Element_Datatype = 8 /*!< Datatype element with binary value 00001000 */,
		Element_Primitive = 10 /*!< Primitive element with binary value 00001010 */,
		Element_Class = 12 /*!< Class element with binary value 00001100 */,

		Element_Method = 16 /*!< Method element with binary value 00100000 */,
		Element_Operation = 32 /*!< Operation element with binary value 00110000 */,

		Element_Property = 64 /*!< Property element with binary value 01000000 */,
	};

	typedef boost::shared_ptr<ElementObject> ElementObjectPtr;

	void initialise();
}

#endif // DEFINES_HPP
