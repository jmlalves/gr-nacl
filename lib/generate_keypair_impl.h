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

// lib/generate_keypair_impl.h
#ifndef INCLUDED_NACL_GENERATE_KEYPAIR_IMPL_H
#define INCLUDED_NACL_GENERATE_KEYPAIR_IMPL_H

#include <nacl/generate_keypair.h>
#include <pmt/pmt.h>
#include <string>

namespace gr {
  namespace nacl {

    class generate_keypair_impl : public generate_keypair
    {
     private:
      pmt::pmt_t d_port_id_in;
      pmt::pmt_t d_port_id_sk;     // CHANGE: secret-key port
      pmt::pmt_t d_port_id_pk;     // CHANGE: public-key port
      std::string d_sk_file;
      std::string d_pk_file;

     public:
      generate_keypair_impl(const std::string &filename_sk,
                            const std::string &filename_pk);
      ~generate_keypair_impl() override;

      void handle_msg(pmt::pmt_t msg) override;
      int work(int noutput_items,
               gr_vector_const_void_star &input_items,
               gr_vector_void_star &output_items) override;
    };

  } // namespace nacl
} // namespace gr

#endif /* INCLUDED_NACL_GENERATE_KEYPAIR_IMPL_H */
