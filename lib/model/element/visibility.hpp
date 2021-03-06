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

#ifndef VISIBILITY_HPP
#define VISIBILITY_HPP

#include <QString>

enum VisibilityType
{
	Visibility_Private,
	Visibility_Public,
	Visibility_Package,
	Visibility_Protected,
};

class Visibility
{
public:
	Visibility(VisibilityType type = Visibility_Private);

	void setType(VisibilityType type);
	VisibilityType type() const;
	QString umlNotation() const;

private:
	VisibilityType _type;
};

inline Visibility::Visibility(VisibilityType type)
	: _type(type)
{
}

inline void Visibility::setType(VisibilityType type)
{
	_type = type;
}

inline VisibilityType Visibility::type() const
{
	return _type;
}

inline QString Visibility::umlNotation() const
{
	switch(_type)
	{
	case Visibility_Public:		return "+";
	case Visibility_Protected:	return "#";
	case Visibility_Private:	return "-";
	case Visibility_Package:	return "~";
	default:					return "";
	}
}

#endif // VISIBILITY_HPP
