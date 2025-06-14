// python/nacl_pybind.cpp
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>   // if you need STL conversions

// include your GR block headers
#include <nacl/encrypt_public_impl.h>
#include <nacl/decrypt_public_impl.h>
#include <nacl/generate_keypair_impl.h>
#include <nacl/generate_key_impl.h>

namespace py = pybind11;

PYBIND11_MODULE(_nacl, m) {
    m.doc() = "gr-nacl pybind11 bindings";

    // bind encrypt_public block
    py::class_<gr::nacl::encrypt_public,
               gr::block, gr::basic_block, std::shared_ptr<gr::nacl::encrypt_public>>(m, "encrypt_public")
        .def(py::init(&gr::nacl::encrypt_public::make))
        ;

    // bind decrypt_public
    py::class_<gr::nacl::decrypt_public,
               gr::block, gr::basic_block, std::shared_ptr<gr::nacl::decrypt_public>>(m, "decrypt_public")
        .def(py::init(&gr::nacl::decrypt_public::make))
        ;

    // bind generate_keypair
    py::class_<gr::nacl::generate_keypair,
               gr::block, gr::basic_block, std::shared_ptr<gr::nacl::generate_keypair>>(m, "generate_keypair")
        .def(py::init(&gr::nacl::generate_keypair::make))
        ;

    // bind generate_key
    py::class_<gr::nacl::generate_key,
               gr::block, gr::basic_block, std::shared_ptr<gr::nacl::generate_key>>(m, "generate_key")
        .def(py::init(&gr::nacl::generate_key::make))
        ;
}
