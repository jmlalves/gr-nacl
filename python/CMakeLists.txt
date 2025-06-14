# Copyright 2011 Free Software Foundation, Inc.
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
# Include python install macros
########################################################################
include(GrPython)
if(NOT PYTHONINTERP_FOUND)
    return()
endif()

########################################################################
# Find pybind11 and build our extension
########################################################################
find_package(pybind11 REQUIRED)

# point at our new binding source
pybind11_add_module(_nacl
    nacl_pybind.cpp
)

# link against the core C++ library
target_link_libraries(_nacl PRIVATE
    gnuradio-nacl  # your OOT library target
)

########################################################################
# Install python bitstreams
########################################################################
GR_PYTHON_INSTALL(
    FILES
      __init__.py
    DESTINATION ${GR_PYTHON_DIR}/nacl
)

# install the compiled extension
install(TARGETS _nacl
    DESTINATION ${GR_PYTHON_DIR}/nacl
)

########################################################################
# Handle the unit tests: adjust imports to use _nacl
########################################################################
include(GrTest)
set(GR_TEST_TARGET_DEPS gnuradio-nacl)
GR_ADD_TEST(qa_encrypt_public ${PYTHON_EXECUTABLE}
    -c "import nacl; nacl.encrypt_public.make()"
)
# …repeat for other tests, or point them at your existing qa_*.py
