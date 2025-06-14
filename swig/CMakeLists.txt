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
# Check if there is C++ code at all
########################################################################
if(NOT nacl_sources)
	MESSAGE(STATUS "No C++ sources... skipping swig/")
	return()
endif(NOT nacl_sources)

########################################################################
# Include SWIG and Python generation macros (GNU Radio 3.10+)
########################################################################
find_package(SWIG   REQUIRED)
find_package(Python COMPONENTS Interpreter Development REQUIRED)
include(GR_SWIG)
include(GRPython)

########################################################################
# Setup swig generation
########################################################################
# Add GNUradio's SWIG include dirs
foreach(incdir ${GNURADIO_RUNTIME_INCLUDE_DIRS})
    list(APPEND GR_SWIG_INCLUDE_DIRS ${incdir}/gnuradio/swig)
endforeach()

# Documentation interface
set(GR_SWIG_DOC_FILE ${CMAKE_CURRENT_BINARY_DIR}/nacl_swig_doc.i)
set(GR_SWIG_DOC_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../include)

# Generate the SWIG Python module
GR_SWIG_MODULE(
    MODULE          nacl_swig
    LANGUAGE        python
    SOURCES         ${CMAKE_CURRENT_SOURCE_DIR}/nacl_swig.i
    LIBRARIES       gnuradio-nacl
    INCLUDE_DIRS    ${GR_SWIG_INCLUDE_DIRS}
    DOC_SOURCE      ${GR_SWIG_DOC_FILE}
    DOC_DIRS        ${GR_SWIG_DOC_DIRS}
)

########################################################################
# Install the build swig module
########################################################################
GR_SWIG_INSTALL(
    TARGETS nacl_swig
    DESTINATION ${GR_PYTHON_DIR}/nacl
)

########################################################################
# Install swig .i files for development
########################################################################
install(
    FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/nacl_swig.i
        ${CMAKE_CURRENT_BINARY_DIR}/nacl_swig.i
    DESTINATION
        ${GR_INCLUDE}/nacl/swig
)
