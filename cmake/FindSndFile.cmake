# - Try to find the libsndfile
#
# This will define
#
# SNDFILE_FOUND - system has libsndfile
# SNDFILE_INCLUDE_DIRS - the libsndfile include directory
# SNDFILE_LIBRATIES - link these to use libsndfile

if(SNDFILE_INCLUDE_DIR)
  # Already in cache, be silent
  set(SNDFILE_FIND_QUIETLY TRUE)
endif(SNDFILE_INCLUDE_DIR)
  
find_package(PkgConfig QUIET)
pkg_check_modules(PC_SNDFILE QUIET sndfile)

find_path(SNDFILE_INCLUDE_DIR
  NAMES
  sndfile.h
  HINTS
  ${PC_SNDFILE_INCLUDEDIR}
  ${PC_SNDFILE_INCLUDE_DIRS}
  ${SNDFILE_SOURCE_DIR}
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr
  /usr/local
  /opt
  /opt/local
  ENV SNDFILE_DIR
  PATH_SUFFIXES include include/sndfile)

find_library(SNDFILE_LIBRARY
  NAMES
  sndfile
  HINTS
  ${PC_SNDFILE_LIBDIR}
  ${PC_SNDFILE_LIBRARY_DIRS}
  ${SNDFILE_SOURCE_DIR}
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr
  /usr/local
  /opt
  /opt/local
  ENV SNDFILE_DIR
  PATH_SUFFIXES lib64 lib)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(sndfile DEFAULT_MSG SNDFILE_INCLUDE_DIR SNDFILE_LIBRARY)

if(SNDFILE_FOUND)
  set(SNDFILE_INCLUDE_DIRS ${SNDFILE_INCLUDE_DIR})
  set(SNDFILE_LIBRARIES ${SNDFILE_LIBRARY})
endif(SNDFILE_FOUND)

mark_as_advanced(SNDFILE_INCLUDE_DIR SNDFILE_LIBRARY)
