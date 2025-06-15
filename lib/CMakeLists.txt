# Copyright 2011,2012 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX
find_package(spdlog REQUIRED)

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

list(APPEND nacl_sources
    encrypt_public_impl.cc
    generate_keypair_impl.cc
    decrypt_public_impl.cc
    generate_key_impl.cc
    encrypt_secret_impl.cc
    decrypt_secret_impl.cc
    crypt_tagged_stream_impl.cc
)

set(nacl_sources "${nacl_sources}" PARENT_SCOPE)
if(NOT nacl_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT nacl_sources)

add_library(gnuradio-nacl SHARED ${nacl_sources})
find_package(Gnuradio REQUIRED COMPONENTS runtime python)
target_link_libraries(gnuradio-nacl 
    PUBLIC
        ${Boost_LIBRARIES}
        ${GNURADIO_ALL_LIBRARIES}
        ${SODIUM_LIBRARIES}
        ${PYTHON_LIBRARIES}
        ${GNURADIO_RUNTIME_LIBRARIES}
)
target_compile_features(gnuradio-nacl PUBLIC cxx_std_17)
#set_target_properties(gnuradio-nacl PROPERTIES 
#    VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${VERSION_PATCH}
#    SOVERSION ${PROJECT_VERSION_MAJOR}
#)

if(APPLE)
    set_target_properties(gnuradio-nacl PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
install(TARGETS gnuradio-nacl
    LIBRARY DESTINATION lib${LIB_SUFFIX} # .so/.dylib file
    ARCHIVE DESTINATION lib${LIB_SUFFIX} # .lib file
    RUNTIME DESTINATION bin              # .dll file
)
