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

// lib/generate_keypair_impl.cc
#include "generate_keypair_impl.h"
#include <gnuradio/io_signature.h>
#include <cstring>
#include <algorithm>
#include <functional>

namespace gr {
namespace nacl {

generate_keypair::sptr
generate_keypair::make(const std::string &filename_sk,
                       const std::string &filename_pk)
{
    return generate_keypair::sptr(
        new generate_keypair_impl(filename_sk, filename_pk));
}

generate_keypair_impl::generate_keypair_impl(const std::string &filename_sk,
                                            const std::string &filename_pk)
    : d_sk_file(filename_sk),
      d_pk_file(filename_pk)
{
    d_port_id_in = pmt::mp("in");
    message_port_register_in(d_port_id_in);
    set_msg_handler(d_port_id_in,
        std::bind(&generate_keypair_impl::handle_msg, this, std::placeholders::_1));
    d_port_id_sk = pmt::mp("sk");
    message_port_register_out(d_port_id_sk);
    d_port_id_pk = pmt::mp("pk");
    message_port_register_out(d_port_id_pk);
}

generate_keypair_impl::~generate_keypair_impl() = default;

void
generate_keypair_impl::handle_msg(pmt::pmt_t)
{
    // stub: issue 32-byte zero keypair
    std::vector<unsigned char> sk(32,0), pk(32,0);
    pmt::pmt_t sk_blob = pmt::init_u8vector(sk.size(), sk.data());
    pmt::pmt_t pk_blob = pmt::init_u8vector(pk.size(), pk.data());
    message_port_pub(d_port_id_sk, sk_blob);
    message_port_pub(d_port_id_pk, pk_blob);
}

int
generate_keypair_impl::work(int                       noutput_items,
                         gr_vector_int            &ninput_items,
                         gr_vector_const_void_star &input_items,
                         gr_vector_void_star      &output_items)
{
    return 0;
}

} // namespace nacl
} // namespace gr
