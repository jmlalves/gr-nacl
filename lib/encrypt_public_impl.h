/* -*- c++ -*- */
/* 
 * Copyright 2025 Joao Alves
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

// include/nacl/encrypt_public.h
#ifndef INCLUDED_NACL_ENCRYPT_PUBLIC_H
#define INCLUDED_NACL_ENCRYPT_PUBLIC_H

#include <nacl/api.h>
#include <gnuradio/block.h>
#include <string>
// CHANGE: for std::shared_ptr
#include <memory>

namespace gr {
  namespace nacl {

    /*!
     * \brief Public-key encryption block
     * \ingroup nacl
     */
    class NACL_API encrypt_public : virtual public gr::block
    {
     public:
      // CHANGE: use C++11 shared_ptr instead of boost
      typedef std::shared_ptr<encrypt_public> sptr;

      static sptr make(const std::string &filename_pk,
                       const std::string &filename_sk);
    };

  } // namespace nacl
} // namespace gr

#endif /* INCLUDED_NACL_ENCRYPT_PUBLIC_H */
