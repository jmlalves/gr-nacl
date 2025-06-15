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

// lib/crypt_tagged_stream_impl.cc
#include "crypt_tagged_stream_impl.h"
#include <gnuradio/io_signature.h>
#include <cstring>
#include <functional>

namespace gr {
namespace nacl {

crypt_tagged_stream::sptr
crypt_tagged_stream::make(const std::string &key,
                          const std::string &nonce,
                          bool rotate_nonce,
                          const std::string &len_key)
{
    return crypt_tagged_stream::sptr(
        new crypt_tagged_stream_impl(key, nonce, rotate_nonce, len_key));
}

crypt_tagged_stream_impl::crypt_tagged_stream_impl(const std::string &key,
                                                const std::string &nonce,
                                                bool rotate_nonce,
                                                const std::string &len_key)
    : d_key(key), d_nonce(nonce), d_rotate_nonce(rotate_nonce)
{
    d_port_id_in = pmt::mp("in");
    message_port_register_in(d_port_id_in);
    set_msg_handler(d_port_id_in,
        std::bind(&crypt_tagged_stream_impl::handle_msg, this, std::placeholders::_1));
    d_port_id_out = pmt::mp("out");
    message_port_register_out(d_port_id_out);
}

crypt_tagged_stream_impl::~crypt_tagged_stream_impl() = default;

int
crypt_tagged_stream_impl::calculate_output_stream_length(const gr_vector_int &ninput_items)
{
    // stub: 1-for-1
    return ninput_items[0];
}

void
crypt_tagged_stream_impl::handle_msg(pmt::pmt_t msg)
{
    if (pmt::is_blob(msg)) {
        size_t len = pmt::blob_length(msg);
        const unsigned char *data = static_cast<const unsigned char*>(pmt::blob_data(msg));
        // stub: echo input
        pmt::pmt_t out = pmt::init_u8vector(len, data);
        message_port_pub(d_port_id_out, out);
    }
}

int
crypt_tagged_stream_impl::work(int                        noutput_items,
                              gr_vector_int            &ninput_items,
                              gr_vector_const_void_star &input_items,
                              gr_vector_void_star       &output_items)
{
    return noutput_items;
}

} // namespace nacl
} // namespace gr
