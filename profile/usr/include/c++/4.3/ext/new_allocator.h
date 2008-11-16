               :// Allocator that wraps operator new -*- C++ -*-
               :
               :// Copyright (C) 2001, 2002, 2003, 2004, 2005 Free Software Foundation, Inc.
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
               :/** @file ext/new_allocator.h
               : *  This file is a GNU extension to the Standard C++ Library.
               : */
               :
               :#ifndef _NEW_ALLOCATOR_H
               :#define _NEW_ALLOCATOR_H 1
               :
               :#include <new>
               :#include <bits/functexcept.h>
               :#include <bits/stl_move.h>
               :
               :_GLIBCXX_BEGIN_NAMESPACE(__gnu_cxx)
               :
               :  using std::size_t;
               :  using std::ptrdiff_t;
               :
               :  /**
               :   *  @brief  An allocator that uses global new, as per [20.4].
               :   *
               :   *  This is precisely the allocator defined in the C++ Standard. 
               :   *    - all allocation calls operator new
               :   *    - all deallocation calls operator delete
               :   */
               :  template<typename _Tp>
               :    class new_allocator
               :    {
               :    public:
               :      typedef size_t     size_type;
               :      typedef ptrdiff_t  difference_type;
               :      typedef _Tp*       pointer;
               :      typedef const _Tp* const_pointer;
               :      typedef _Tp&       reference;
               :      typedef const _Tp& const_reference;
               :      typedef _Tp        value_type;
               :
               :      template<typename _Tp1>
               :        struct rebind
               :        { typedef new_allocator<_Tp1> other; };
               :
    54  0.0093 :      new_allocator() throw() { } /* __gnu_cxx::new_allocator<Command*>::new_allocator()      1 1.7e-04, __gnu_cxx::new_allocator<Obstacle*>::new_allocator()     53  0.0091, total:     54  0.0093 */
               :
               :      new_allocator(const new_allocator&) throw() { }
               :
               :      template<typename _Tp1>
               :        new_allocator(const new_allocator<_Tp1>&) throw() { }
               :
               :      ~new_allocator() throw() { }
               :
               :      pointer
               :      address(reference __x) const { return &__x; }
               :
               :      const_pointer
               :      address(const_reference __x) const { return &__x; }
               :
               :      // NB: __n is permitted to be 0.  The C++ standard says nothing
               :      // about what the return value is when __n == 0.
               :      pointer
    37  0.0064 :      allocate(size_type __n, const void* = 0) /* __gnu_cxx::new_allocator<std::_List_node<Obstacle*> >::allocate(unsigned int, void const*) total:    210  0.0362 */
               :      { 
   124  0.0214 :	if (__builtin_expect(__n > this->max_size(), false))
               :	  std::__throw_bad_alloc();
               :
    22  0.0038 :	return static_cast<_Tp*>(::operator new(__n * sizeof(_Tp)));
    27  0.0047 :      }
               :
               :      // __p is not permitted to be a null pointer.
               :      void
    20  0.0034 :      deallocate(pointer __p, size_type) /* __gnu_cxx::new_allocator<std::_List_node<Obstacle*> >::deallocate(std::_List_node<Obstacle*>*, unsigned int) total:     72  0.0124 */
    52  0.0090 :      { ::operator delete(__p); }
               :
               :      size_type
    25  0.0043 :      max_size() const throw()  /* __gnu_cxx::new_allocator<char>::max_size() const      1 1.7e-04, __gnu_cxx::new_allocator<std::_List_node<Obstacle*> >::max_size() const     34  0.0059, total:     35  0.0060 */
    10  0.0017 :      { return size_t(-1) / sizeof(_Tp); }
               :
               :      // _GLIBCXX_RESOLVE_LIB_DEFECTS
               :      // 402. wrong new expression in [some_] allocator::construct
               :      void 
    15  0.0026 :      construct(pointer __p, const _Tp& __val)  /* __gnu_cxx::new_allocator<Gen*>::construct(Gen**, Gen* const&)      1 1.7e-04, __gnu_cxx::new_allocator<Obstacle*>::construct(Obstacle**, Obstacle* const&)    135  0.0233, total:    136  0.0234 */
   121  0.0208 :      { ::new((void *)__p) _Tp(__val); }
               :
               :#ifdef __GXX_EXPERIMENTAL_CXX0X__
               :      template<typename... _Args>
               :        void
               :        construct(pointer __p, _Args&&... __args)
               :	{ ::new((void *)__p) _Tp(std::forward<_Args>(__args)...); }
               :#endif
               :
               :      void 
    34  0.0059 :      destroy(pointer __p) { __p->~_Tp(); } /* __gnu_cxx::new_allocator<Obstacle*>::destroy(Obstacle**) total:     34  0.0059 */
               :    };
               :
               :  template<typename _Tp>
               :    inline bool
               :    operator==(const new_allocator<_Tp>&, const new_allocator<_Tp>&)
               :    { return true; }
               :  
               :  template<typename _Tp>
               :    inline bool
               :    operator!=(const new_allocator<_Tp>&, const new_allocator<_Tp>&)
               :    { return false; }
               :
               :_GLIBCXX_END_NAMESPACE
               :
               :#endif
/* 
 * Total samples for file : "/usr/include/c++/4.3/ext/new_allocator.h"
 * 
 *    541  0.0932
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
