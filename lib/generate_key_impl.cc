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

// lib/generate_key_impl.cc
#include "generate_key_impl.h"
#include <gnuradio/io_signature.h>
#include <cstring>
#include <algorithm>
#include <functional>

namespace gr {
namespace nacl {

generate_key::sptr
generate_key::make(const std::string &filename_key)
{
    return generate_key::sptr(
        new generate_key_impl(filename_key));
}

generate_key_impl::generate_key_impl(const std::string &filename_key)
    : generate_key("generate_key",
                   gr::io_signature::make(0,0,0),
                   gr::io_signature::make(0,0,0)),
      d_key_file(filename_key)
{
    d_port_id_in = pmt::mp("in");
    message_port_register_in(d_port_id_in);
    set_msg_handler(d_port_id_in,
        std::bind(&generate_key_impl::handle_msg, this, std::placeholders::_1));
    d_port_id_out = pmt::mp("out");
    message_port_register_out(d_port_id_out);
}

generate_key_impl::~generate_key_impl() = default;

void
generate_key_impl::handle_msg(pmt::pmt_t)
{
    // stub: issue a 32-byte zero key
    std::vector<unsigned char> key(32, 0);
    pmt::pmt_t out = pmt::make_blob(key.data(), key.size());
    message_port_pub(d_port_id_out, out);
}

int
generate_key_impl::work(int, gr_vector_const_void_star&, gr_vector_void_star&)
{
    return 0;
}

} // namespace nacl
} // namespace gr
