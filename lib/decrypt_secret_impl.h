/* -*- c++ -*- */
/* Copyright 2025 Joao Alves
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

// lib/decrypt_secret_impl.h
#ifndef INCLUDED_NACL_DECRYPT_SECRET_IMPL_H
#define INCLUDED_NACL_DECRYPT_SECRET_IMPL_H

#include <nacl/decrypt_secret.h>
#include <pmt/pmt.h>
#include <string>

namespace gr {
  namespace nacl {

    class decrypt_secret_impl : public decrypt_secret
    {
     private:
      pmt::pmt_t d_port_id_in;
      pmt::pmt_t d_port_id_out;
      std::string d_key_file;

     public:
      decrypt_secret_impl(const std::string& filename_key);
      ~decrypt_secret_impl() override;

      void handle_msg(pmt::pmt_t msg);
      int work(int noutput_items,
               gr_vector_const_void_star& input_items,
               gr_vector_void_star& output_items);
    };

  } // namespace nacl
} // namespace gr

#endif /* INCLUDED_NACL_DECRYPT_SECRET_IMPL_H */
