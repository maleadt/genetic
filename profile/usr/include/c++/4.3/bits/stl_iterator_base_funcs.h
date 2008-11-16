               :// Functions used by iterators -*- C++ -*-
               :
               :// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
               :// Free Software Foundation, Inc.
               ://
               :// This file is part of the GNU ISO C++ Library.  This library is free
               :// software; you can redistribute it and/or modify it under the
               :// terms of the GNU General Public License as published by the
               :// Free Software Foundation; either version 2, or (at your option)
               :// any later version.
               :
               :// This library is distributed in the hope that it will be useful,
               :// but WITHOUT ANY WARRANTY; without even the implied warranty of
               :// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
               :// GNU General Public License for more details.
               :
               :// You should have received a copy of the GNU General Public License along
               :// with this library; see the file COPYING.  If not, write to the Free
               :// Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
               :// USA.
               :
               :// As a special exception, you may use this file as part of a free software
               :// library without restriction.  Specifically, if other files instantiate
               :// templates or use macros or inline functions from this file, or you compile
               :// this file and link it with other files to produce an executable, this
               :// file does not by itself cause the resulting executable to be covered by
               :// the GNU General Public License.  This exception does not however
               :// invalidate any other reasons why the executable file might be covered by
               :// the GNU General Public License.
               :
               :/*
               : *
               : * Copyright (c) 1994
               : * Hewlett-Packard Company
               : *
               : * Permission to use, copy, modify, distribute and sell this software
               : * and its documentation for any purpose is hereby granted without fee,
               : * provided that the above copyright notice appear in all copies and
               : * that both that copyright notice and this permission notice appear
               : * in supporting documentation.  Hewlett-Packard Company makes no
               : * representations about the suitability of this software for any
               : * purpose.  It is provided "as is" without express or implied warranty.
               : *
               : *
               : * Copyright (c) 1996-1998
               : * Silicon Graphics Computer Systems, Inc.
               : *
               : * Permission to use, copy, modify, distribute and sell this software
               : * and its documentation for any purpose is hereby granted without fee,
               : * provided that the above copyright notice appear in all copies and
               : * that both that copyright notice and this permission notice appear
               : * in supporting documentation.  Silicon Graphics makes no
               : * representations about the suitability of this software for any
               : * purpose.  It is provided "as is" without express or implied warranty.
               : */
               :
               :/** @file stl_iterator_base_funcs.h
               : *  This is an internal header file, included by other library headers.
               : *  You should not attempt to use it directly.
               : *
               : *  This file contains all of the general iterator-related utility
               : *  functions, such as distance() and advance().
               : */
               :
               :#ifndef _STL_ITERATOR_BASE_FUNCS_H
               :#define _STL_ITERATOR_BASE_FUNCS_H 1
               :
               :#pragma GCC system_header
               :#include <bits/concept_check.h>
               :
               :_GLIBCXX_BEGIN_NAMESPACE(std)
               :
               :  template<typename _InputIterator>
               :    inline typename iterator_traits<_InputIterator>::difference_type
               :    __distance(_InputIterator __first, _InputIterator __last,
   118  0.0203 :               input_iterator_tag) /* std::iterator_traits<std::_List_const_iterator<Gen*> >::difference_type std::__distance<std::_List_const_iterator<Gen*> >(std::_List_const_iterator<Gen*>, std::_List_const_iterator<Gen*>, std::input_iterator_tag)      2 3.4e-04, std::iterator_traits<std::_List_const_iterator<Command*> >::difference_type std::__distance<std::_List_const_iterator<Command*> >(std::_List_const_iterator<Command*>, std::_List_const_iterator<Command*>, std::input_iterator_tag)      1 1.7e-04, std::iterator_traits<std::_List_const_iterator<Obstacle*> >::difference_type std::__distance<std::_List_const_iterator<Obstacle*> >(std::_List_const_iterator<Obstacle*>, std::_List_const_iterator<Obstacle*>, std::input_iterator_tag)   2690  0.4635, std::iterator_traits<std::_List_const_iterator<Creature*> >::difference_type std::__distance<std::_List_const_iterator<Creature*> >(std::_List_const_iterator<Creature*>, std::_List_const_iterator<Creature*>, std::input_iterator_tag)    620  0.1068, total:   3313  0.5708 */
               :    {
               :      // concept requirements
               :      __glibcxx_function_requires(_InputIteratorConcept<_InputIterator>)
               :
    60  0.0103 :      typename iterator_traits<_InputIterator>::difference_type __n = 0;
  1628  0.2805 :      while (__first != __last)
               :	{
   526  0.0906 :	  ++__first;
   532  0.0917 :	  ++__n;
               :	}
   126  0.0217 :      return __n;
   323  0.0557 :    }
               :
               :  template<typename _RandomAccessIterator>
               :    inline typename iterator_traits<_RandomAccessIterator>::difference_type
               :    __distance(_RandomAccessIterator __first, _RandomAccessIterator __last,
               :               random_access_iterator_tag)
               :    {
               :      // concept requirements
               :      __glibcxx_function_requires(_RandomAccessIteratorConcept<
               :				  _RandomAccessIterator>)
               :      return __last - __first;
               :    }
               :
               :  /**
               :   *  @brief A generalization of pointer arithmetic.
               :   *  @param  first  An input iterator.
               :   *  @param  last  An input iterator.
               :   *  @return  The distance between them.
               :   *
               :   *  Returns @c n such that first + n == last.  This requires that @p last
               :   *  must be reachable from @p first.  Note that @c n may be negative.
               :   *
               :   *  For random access iterators, this uses their @c + and @c - operations
               :   *  and are constant time.  For other %iterator classes they are linear time.
               :  */
               :  template<typename _InputIterator>
               :    inline typename iterator_traits<_InputIterator>::difference_type
   145  0.0250 :    distance(_InputIterator __first, _InputIterator __last) /* std::iterator_traits<std::_List_const_iterator<Command*> >::difference_type std::distance<std::_List_const_iterator<Command*> >(std::_List_const_iterator<Command*>, std::_List_const_iterator<Command*>)      1 1.7e-04, std::iterator_traits<std::_List_const_iterator<Obstacle*> >::difference_type std::distance<std::_List_const_iterator<Obstacle*> >(std::_List_const_iterator<Obstacle*>, std::_List_const_iterator<Obstacle*>)    211  0.0364, std::iterator_traits<std::_List_const_iterator<Creature*> >::difference_type std::distance<std::_List_const_iterator<Creature*> >(std::_List_const_iterator<Creature*>, std::_List_const_iterator<Creature*>)    490  0.0844, total:    702  0.1210 */
               :    {
               :      // concept requirements -- taken care of in __distance
               :      return std::__distance(__first, __last,
   344  0.0593 :			     std::__iterator_category(__first));
   213  0.0367 :    }
               :
               :  template<typename _InputIterator, typename _Distance>
               :    inline void
               :    __advance(_InputIterator& __i, _Distance __n, input_iterator_tag)
               :    {
               :      // concept requirements
               :      __glibcxx_function_requires(_InputIteratorConcept<_InputIterator>)
               :      while (__n--)
               :	++__i;
               :    }
               :
               :  template<typename _BidirectionalIterator, typename _Distance>
               :    inline void
               :    __advance(_BidirectionalIterator& __i, _Distance __n,
               :	      bidirectional_iterator_tag)
               :    {
               :      // concept requirements
               :      __glibcxx_function_requires(_BidirectionalIteratorConcept<
               :				  _BidirectionalIterator>)
               :      if (__n > 0)
               :        while (__n--)
               :	  ++__i;
               :      else
               :        while (__n++)
               :	  --__i;
               :    }
               :
               :  template<typename _RandomAccessIterator, typename _Distance>
               :    inline void
               :    __advance(_RandomAccessIterator& __i, _Distance __n,
               :              random_access_iterator_tag)
               :    {
               :      // concept requirements
               :      __glibcxx_function_requires(_RandomAccessIteratorConcept<
               :				  _RandomAccessIterator>)
               :      __i += __n;
               :    }
               :
               :  /**
               :   *  @brief A generalization of pointer arithmetic.
               :   *  @param  i  An input iterator.
               :   *  @param  n  The "delta" by which to change @p i.
               :   *  @return  Nothing.
               :   *
               :   *  This increments @p i by @p n.  For bidirectional and random access
               :   *  iterators, @p n may be negative, in which case @p i is decremented.
               :   *
               :   *  For random access iterators, this uses their @c + and @c - operations
               :   *  and are constant time.  For other %iterator classes they are linear time.
               :  */
               :  template<typename _InputIterator, typename _Distance>
               :    inline void
               :    advance(_InputIterator& __i, _Distance __n)
               :    {
               :      // concept requirements -- taken care of in __advance
               :      typename iterator_traits<_InputIterator>::difference_type __d = __n;
               :      std::__advance(__i, __d, std::__iterator_category(__i));
               :    }
               :
               :_GLIBCXX_END_NAMESPACE
               :
               :#endif /* _STL_ITERATOR_BASE_FUNCS_H */
/* 
 * Total samples for file : "/usr/include/c++/4.3/bits/stl_iterator_base_funcs.h"
 * 
 *   4015  0.6918
 */


/* 
 * Command line: opannotate --source --output-dir=. ../bin/Debug/evolve 
 * 
 * Interpretation of command line:
 * Output annotated source file with samples
 * Output all files
 * 
 * CPU: P4 / Xeon with 2 hyper-threads, speed 3391.7 MHz (estimated)
 * Counted GLOBAL_POWER_EVENTS events (time during which processor is not stopped) with a unit mask of 0x01 (mandatory) count 100000
 */
