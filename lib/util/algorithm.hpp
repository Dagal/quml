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

#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <algorithm>

namespace stf
{
	template<typename _ForwardIterator, typename _Compare> _ForwardIterator binary_find_if(_ForwardIterator __first, _ForwardIterator __last, _Compare __fan_comp)
	{
		typedef typename std::iterator_traits<_ForwardIterator>::value_type _ValueType;
		typedef typename std::iterator_traits<_ForwardIterator>::difference_type _DistanceType;

		// concept requirements
		__glibcxx_function_requires(_ForwardIteratorConcept<_ForwardIterator>)

		_DistanceType __len = std::distance(__first, __last);
		_DistanceType __half;
		_ForwardIterator __middle;

		while (__len > 0)
		{
			__half = __len >> 1;
			__middle = __first;
			std::advance(__middle, __half);

			int __val = __fan_comp(*__middle);
			if (__val < 0)
			{
				__first = __middle;
				++__first;
				__len = __len - __half - 1;
			}
			else if(__val > 0)
			{
				__len = __half;
			}
			else return __middle;
		}

		return __last;
	}

//	template<typename _ForwardIterator, typename _OutputIterator typename _Transformer, typename _Predicate> _ForwardIterator copy_transformed_if(_ForwardIterator __first, _ForwardIterator __last, _OutputIterator __startOutput, _Transformer __transformer, _Predicate __predicate)
//	{
//		__glibcxx_function_requires(_ForwardIteratorConcept<_ForwardIterator>)
//
//		_ForwardIterator __current = __first;
//
//		while(__current != __last)
//		{
//
//		}
//
//	}
}

#endif // ALGORITHM_HPP
