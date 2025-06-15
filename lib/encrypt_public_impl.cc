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

// lib/encrypt_public_impl.cc
#include "encrypt_public_impl.h"
#include <gnuradio/io_signature.h>
#include <cstring>
#include <functional>

namespace gr {
namespace nacl {

encrypt_public::sptr
encrypt_public::make(const std::string &filename_pk,
                    const std::string &filename_sk)
{
    return encrypt_public::sptr(
        new encrypt_public_impl(filename_pk, filename_sk));
}

encrypt_public_impl::encrypt_public_impl(const std::string &filename_pk,
                                        const std::string &filename_sk)
    : gr::block("encrypt_public",
                gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0)),  // ← base-class init
      d_pk_file(filename_pk),
      d_sk_file(filename_sk)
{
    d_port_id_in = pmt::mp("in");
    message_port_register_in(d_port_id_in);
    set_msg_handler(d_port_id_in,
        std::bind(&encrypt_public_impl::handle_msg, this, std::placeholders::_1));
    d_port_id_out = pmt::mp("out");
    message_port_register_out(d_port_id_out);
}

encrypt_public_impl::~encrypt_public_impl() = default;

void
encrypt_public_impl::handle_msg(pmt::pmt_t msg)
{
    if (pmt::is_blob(msg)) {
        size_t len = pmt::blob_length(msg);
        const unsigned char *data = static_cast<const unsigned char*>(pmt::blob_data(msg));
        pmt::pmt_t out = pmt::init_u8vector(len, data);
        message_port_pub(d_port_id_out, out);
    }
}

int
encrypt_public_impl::work(int, gr_vector_const_void_star&, gr_vector_void_star&)
{
    return 0;
}

} // namespace nacl
} // namespace gr
