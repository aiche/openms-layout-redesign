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

#------------------------------------------------------------------------------
# This cmake file handles all the project specific compiler flags

if (CMAKE_COMPILER_IS_GNUCXX)
	# Determine gcc version
	execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GNUCXX_VERSION)
	string(REGEX MATCHALL "[0-9]+" GCC_VERSION_COMPONENTS ${GNUCXX_VERSION})
	list(GET GCC_VERSION_COMPONENTS 0 GNUCXX_MAJOR_VERSION)
	list(GET GCC_VERSION_COMPONENTS 1 GNUCXX_MINOR_VERSION)

  add_definitions(-Wall -Wextra -Wno-non-virtual-dtor -Wno-long-long -Wno-variadic-macros)
  if (NOT MT_ENABLE_CUDA)  # necessary since CUDA contains non-pedantic code
		add_definitions(--pedantic)
	endif()

	# Recommended setting for eclipse, see http://www.cmake.org/Wiki/CMake:Eclipse
	if (CMAKE_GENERATOR STREQUAL "Eclipse CDT4 - Unix Makefiles")
		add_definitions(-fmessage-length=0)
	endif()

	if (NOT OPENMS_64BIT_ARCHITECTURE AND ${GNUCXX_MAJOR_VERSION} MATCHES "4" AND ${GNUCXX_MINOR_VERSION} MATCHES "3")
		add_definitions(-march=i486)
	endif()
elseif (MSVC)
	# do not use add_definitions
	# add definitions also lands in stuff like RC_DEFINITION which tend to fail if you use
	# Eclipse CDT 4 - NMAKE generator
	# use set(CF_OPENMS_ADDCXX_FLAGS "${CF_OPENMS_ADDCXX_FLAGS} ...") instead

	## disable dll-interface warning
	set(CF_OPENMS_ADDCXX_FLAGS "${CF_OPENMS_ADDCXX_FLAGS} /wd4251 /wd4275")

	## disable deprecated functions warning (e.g. for POSIX functions)
	set(CF_OPENMS_ADDCXX_FLAGS "${CF_OPENMS_ADDCXX_FLAGS} /wd4996")

	## disable explicit template instantiation request for partially defined classes
	set(CF_OPENMS_ADDCXX_FLAGS "${CF_OPENMS_ADDCXX_FLAGS} /wd4661")

	## disable warning: "decorated name length exceeded, name was truncated"
	set(CF_OPENMS_ADDCXX_FLAGS "${CF_OPENMS_ADDCXX_FLAGS} /wd4503")

	## don't warn about unchecked std::copy()
	add_definitions(/D_SCL_SECURE_NO_WARNINGS /D_CRT_SECURE_NO_WARNINGS /D_CRT_SECURE_NO_DEPRECATE)

	## xerces bug workaround
	add_definitions(/DOPENMS_XERCESDLL)

	## FeatureFinder.obj is huge and won't compile in VS2008 debug otherwise:
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")

	## use multiple Cores (if available)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")

  if (NOT OPENMS_64BIT_ARCHITECTURE)
    ## enable SSE1 on 32bit, on 64bit the compiler flag does not exist
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE")
  endif()
elseif ("${CMAKE_C_COMPILER_ID}" MATCHES "Clang")
  set(CMAKE_COMPILER_IS_CLANG true CACHE INTERNAL "Is CLang compiler (clang++)")
else()
	set(CMAKE_COMPILER_IS_INTELCXX true CACHE INTERNAL "Is Intel C++ compiler (icpc)")
endif()

## platform dependent compiler flags:
include(CheckCXXCompilerFlag)
if (NOT WIN32) # we only want fPIC on non-windows systems (fPIC is implicitly true there)
	CHECK_CXX_COMPILER_FLAG("-fPIC" WITH_FPIC)
	if (WITH_FPIC)
		add_definitions(-fPIC)
	endif()
endif()

## -Wconversion flag for GCC
set(CXX_WARN_CONVERSION OFF CACHE BOOL "Enables warnings for type conversion problems (GCC only)")
if (CXX_WARN_CONVERSION)
	if (CMAKE_COMPILER_IS_GNUCXX)
		add_definitions(-Wconversion)
	endif()
endif()
message(STATUS "Compiler checks for conversion: ${CXX_WARN_CONVERSION}")