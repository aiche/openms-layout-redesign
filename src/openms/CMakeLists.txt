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

project("OpenMS")
cmake_minimum_required(VERSION 2.8.3 FATAL_ERROR)

## naming conventions:
##
## prefix a variable with 'CF_' if it is used to configure a file!
## e.g., CF_LibOpenMSExport
set(CF_OPENMS_PACKAGE_VERSION "${OPENMS_PACKAGE_VERSION_MAJOR}.${OPENMS_PACKAGE_VERSION_MINOR}.${OPENMS_PACKAGE_VERSION_PATCH}" CACHE INTERNAL "OpenMS VERSION" FORCE)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	set(CF_OPENMS_ASSERTIONS 1)
else()
	set(CF_OPENMS_ASSERTIONS 0)
endif()
set(CF_OPENMS_ASSERTIONS ${CF_OPENMS_ASSERTIONS} CACHE INTERNAL "Enables debug messages (precondition and postconditions are enabled, a bit slower) - this is NOT changing any compiler flags!" FORCE)

# cmake args: -DCrawdad_DIR=/path/to/Crawdad/ -DWITH_CRAWDAD=TRUE
if (WITH_CRAWDAD)
  find_package(Crawdad)
  include_directories(${CRAWDAD_INCLUDE_DIRS})
endif()

########################################################
###    external libs (contrib or system)						 ###
########################################################

include(${OPENMS_HOST_DIRECTORY}/cmake/OpenMSBuildSystem_externalLibs.cmake)

# TODO: XERCESC and  CONTRIB are included only with the others
# Expose OpenMS's public includes (including transitive dependencies)
set(OPENMS_INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/include
                               ${PROJECT_BINARY_DIR}/include
                               ${QT_INCLUDES}
                               ${GSL_INCLUDE_DIRS}
                               ${Boost_INCLUDE_DIRS}
                               ${SEQAN_INCLUDE_DIRS}
                               ${LIBSVM_INCLUDE_DIRS}
                               ${GLPK_INCLUDE_DIRS}
                               ${ZLIB_INCLUDE_DIRS}
                               ${BZIP2_INCLUDE_DIR}
                               ${OPENSWATHALGO_INCLUDE_DIRECTORIES}
                               ${CONTRIB_INCLUDE_DIR}
    CACHE INTERNAL "OpenMS include directories" FORCE)

# add OpenMS include directories
include_directories(${OPENMS_INCLUDE_DIRECTORIES})

########################################################
###    useful programms															 ###
########################################################

find_program(SVNVERSION_EXECUTABLE
             svnversion
             PATHS "c:/programme/subversion/bin" "c:/program files/subversion/bin"   ### additional search paths (along with $PATH)
             DOC "svnversion executable which helps in determining the svn revision when building TOPP tools")

if ("${SVNVERSION_EXECUTABLE}" STREQUAL "SVNVERSION_EXECUTABLE-NOTFOUND")
	message(STATUS "Info: The programm svnversion could not be found. SVN-revision information will not be available! Add the location of svnversion(.exe) to your PATH environment variable if you require SVN-revision.")
	set(OPENMS_HAS_SVNVERSION OFF CACHE INTERNAL "SVNVersion(.exe) present?")
else()
	set(OPENMS_HAS_SVNVERSION ON CACHE INTERNAL "SVNVersion(.exe) present?")
endif()

set(SVN_REVISION_FILE "${PROJECT_BINARY_DIR}/include/OpenMS/openms_svn_revision.h")

## update the openms_svn_revision.h file before compiling OpenMS
set(ENABLE_SVN ON CACHE STRING "Use svnversion (if available) to implant a svn revision number into the library!")
if (ENABLE_SVN)
	message(STATUS "SVN revision information enabled. Use 'ENABLE_SVN=OFF' to disable!")
	add_custom_target(svn_revision_update ALL
										COMMAND ${CMAKE_COMMAND} -DSOURCE_DIR=${OPENMS_HOST_DIRECTORY} -DSVN_REVISION_FILE=${SVN_REVISION_FILE}
																						 -DOPENMS_HAS_SVNVERSION=${OPENMS_HAS_SVNVERSION} -DSVNVERSION_EXECUTABLE=${SVNVERSION_EXECUTABLE}
																						 -DENABLE_SVN=ON
																						 -P ${OPENMS_HOST_DIRECTORY}/cmake/svnversion.cmake)
	set_source_files_properties(${SVN_REVISION_FILE} PROPERTIES GENERATED TRUE HEADER_FILE_ONLY TRUE)
else()
  message(STATUS "SVN revision information disabled. Use 'ENABLE_SVN=ON' to enable!")
	include(${OPENMS_HOST_DIRECTORY}/cmake/svnversion.cmake) # generate the header once (for VersionInfo.C include)
endif()


########################################################
###    configure config.h														 ###
########################################################

## this need to be set before config.h is configured!
set(CMAKE_INSTALL_PREFIX "" CACHE INTERNAL "This must be equal to INSTALL_PREFIX!" FORCE)
set(INSTALL_PREFIX "." CACHE PATH "Installation prefix for OpenMS and TOPP")

include(${OPENMS_HOST_DIRECTORY}/cmake/OpenMSBuildSystem_configh.cmake)

## add configured files (used in 'cmake/includes.cmake')
set (OPENMS_CONFIGURED_FILES ${SVN_REVISION_FILE} ${CONFIGURED_CONFIG_H} ${CONFIGURED_OPENMS_PACKAGE_VERSION_H})

## Configures what is tested (DB-test, MS2-search engines)
include(${OPENMS_HOST_DIRECTORY}/cmake/OpenMSBuildSystem_testConfig.cmake)

########################################################
###    BUILD the lib                                 ###
########################################################

#package type
set(VALID_PACKAGE_TYPES "none" "rpm" "deb" "dmg")
set(PACKAGE_TYPE "none" CACHE STRING "Package type (internal): ${VALID_PACKAGE_TYPES}")
# check build type
list(FIND VALID_PACKAGE_TYPES ${PACKAGE_TYPE} list_pos)
if( ${list_pos} EQUAL -1 )
	message(STATUS "The PACKAGE_TYPE ${PACKAGE_TYPE} is invalid")
	message(STATUS "Valid PACKAGE_TYPEs are:")
	foreach( BT ${VALID_PACKAGE_TYPES} )
		message(STATUS " * ${BT}")
	endforeach()
	message(FATAL_ERROR "Aborting ...")
endif()

## big include file for headers and C files, which fills the OpenMS_sources variable
include (${OPENMS_HOST_DIRECTORY}/cmake/includes.cmake)

## add library target
## warning: set BUILD_SHARED_LIBS to decide if library is shared or static (see above)! We need the BUILD_SHARED_LIBS flag to set declspec flags for MSVC!
if (FOUND_CUDART AND MT_ENABLE_CUDA) # we need both conditions due to possible present cached entries
	CUDA_ADD_LIBRARY(OpenMS ${OpenMS_sources} ${Cuda_sources})
else()
	ADD_LIBRARY(OpenMS ${OpenMS_sources})
endif()

## update the openms_svn_revision.h file before compiling OpenMS
if (ENABLE_SVN)
	add_dependencies(OpenMS svn_revision_update)
endif()

## all the dependency libraries are linked into libOpenMS.so, except Qt and CUDA which are still dynamic
set(OPENMS_DEP_LIBRARIES  ${CONTRIB_CBC} 
                          ${GSL_LIBRARIES} 
                          ${GSL_CBLAS_LIBRARIES} 
                          ${DEP_LIBSVM_LIBRARY} 
                          ${CONTRIB_XERCESC} 
                          ${Boost_LIBRARIES} 
                          ${BZIP2_LIBRARIES} 
                          ${ZLIB_LIBRARIES} 
                          ${GLPK_LIBRARIES})
                          
if (TBB_FOUND)
	list(APPEND OPENMS_DEP_LIBRARIES ${TBB_LIBRARIES})
endif()
if (MSVC)
	list(APPEND OPENMS_DEP_LIBRARIES opengl32.lib)
endif()
if (WITH_CRAWDAD)
  list(APPEND OPENMS_DEP_LIBRARIES Crawdad)
endif()

target_link_libraries(OpenMS OpenSwathAlgo ${APPLE_EXTRA_LIBS} ${QT_LIBRARIES} ${OPENMS_DEP_LIBRARIES})

## OPENMS_LIBRARIES defines the libraries used by OpenMS; this should be used to link against executables
## somehow the link dependencies of cmake are broken such that when using POSTFIX names for libs the dependencies are not forwarded...
## we fix this by adding the dependencies of OpenMS directly to the executables as well.
set(OPENMS_LIBRARIES optimized OpenMS${CMAKE_RELEASE_POSTFIX} debug OpenMS${CMAKE_DEBUG_POSTFIX} ${QT_LIBRARIES}
    CACHE INTERNAL "OpenMS libraries" FORCE)

if (MSVC) # Windows
	list(APPEND OPENMS_LIBRARIES ${OPENMS_DEP_LIBRARIES})
	list(APPEND OPENMS_LIBRARIES OpenSwathAlgo)
endif()

## directory for OpenMS(d).lib (required for linking executables)
if (MSVC)
	link_directories(${PROJECT_BINARY_DIR})
endif()

##### CTDs ####
# We will use this path for the knime packages and the osx installer
set(SEARCH_ENGINES_DIRECTORY "" CACHE PATH "Directory containing the search engine executables that should be shipped with OpenMS. Note: We expect the layout from the SVN.")

set(ENABLE_PREPARE_KNIME_PACKAGE OFF CACHE BOOL "If enabled, targets to prepare KNIME packages will be generated. Main target will be 'prepare_knime_package'.")
if (ENABLE_PREPARE_KNIME_PACKAGE)
	include(cmake/OpenMSBuildSystem_ctdSupport.cmake)
endif()

########################################################
###              setup python wrapper                ###
########################################################
OPTION(PYOPENMS "setup build system for pyOpenMS" OFF)
IF (PYOPENMS)
  MESSAGE(STATUS "run cmake files for pyopenms")
	include(pyOpenMS/pyopenms.cmake)
ENDIF()

########################################################
###    verbose Post-build messages and help targets  ###
########################################################
include(${OPENMS_HOST_DIRECTORY}/cmake/OpenMSBuildSystem_messages.cmake)
