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

class UMLDiagram;
class ClassObject;
class DatatypeObject;
class ElementObject;
class OperationObject;
class ParameterObject;
class PropertyObject;

enum ElementType
{
	Element = 0,

	Element_Package = 1,

	Element_Datatype = 4 << 1,
	Element_Primitive = 5 << 1,
	Element_Class = 6 << 1,

	Element_Method = 2 << 4,
	Element_Operation = 3 << 4,

	Element_Parameter = 2 << 6,
	Element_Property = 3 << 6,



};

typedef boost::shared_ptr<ElementObject> ElementObjectPtr;

#endif // DEFINES_HPP
