# --------------------------------------------------------------------------
#                   OpenMS -- Open-Source Mass Spectrometry
# --------------------------------------------------------------------------
# Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
# ETH Zurich, and Freie Universitaet Berlin 2002-2012.
#
# This software is released under a three-clause BSD license:
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#  * Neither the name of any author or any participating institution
#    may be used to endorse or promote products derived from this software
#    without specific prior written permission.
# For a full list of authors, refer to the file AUTHORS.
# --------------------------------------------------------------------------
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL ANY OF THE AUTHORS OR THE CONTRIBUTING
# INSTITUTIONS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# --------------------------------------------------------------------------
# $Maintainer: Stephan Aiche, Chris Bielow $
# $Authors: Andreas Bertsch, Chris Bielow, Stephan Aiche $
# --------------------------------------------------------------------------

project("OpenMS_doc")
cmake_minimum_required(VERSION 2.8.3 FATAL_ERROR)

#------------------------------------------------------------------------------
# Doc programs
#------------------------------------------------------------------------------
set(OpenMS_doc_executables)
include(doxygen/parameters/executables.cmake)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "doc/doxygen/parameters")

# add OpenMS includes 
include_directories(${OPENMS_INCLUDE_DIRECTORIES})

# build doc executables
foreach(i ${OpenMS_doc_executables})
	add_executable(${i} EXCLUDE_FROM_ALL doxygen/parameters/${i}.C)
	target_link_libraries(${i} ${OPENMS_GUI_LIBRARIES})
endforeach(i)

# collection target
add_custom_target(doc_progs)
add_dependencies(doc_progs ${OpenMS_doc_executables} TOPP UTILS GUI)

include(${OPENMS_HOST_DIRECTORY}/cmake/OpenMSBuildSystem_doc.cmake)

#------------------------------------------------------------------------------
# Examples / Tutorials
#------------------------------------------------------------------------------
option(BUILD_EXAMPLES "Compile OpenMS code examples" ON)
if(BUILD_EXAMPLES AND "${PACKAGE_TYPE}" STREQUAL "none")
  add_subdirectory(code_examples)
endif(BUILD_EXAMPLES AND "${PACKAGE_TYPE}" STREQUAL "none")
