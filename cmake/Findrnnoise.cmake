# - Try to find the rnnoise libraries
#
# This will define
#
# RNNOISE_FOUND            system has libopenmha
# RNNOISE_INCLUDE_DIRS     the rnnoise include directory
# RNNOISE_LIBRARIES        rnnoise library


if (RNNOISE_INCLUDE_DIR)
  # Already in cache, be silent
  set (RNNOISE_FIND_QUIETLY TRUE)
endif (RNNOISE_INCLUDE_DIR)

set(_POSSIBLE_RNNOISE_LIBRARY rnnoise)

find_path(RNNOISE_INCLUDE_DIR rnnoise.h
  HINTS 
  ${RNNOISE_SOURCE_DIR}
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /opt
  /usr
  /usr/local
  ENV RNNOISE_DIR
  PATH_SUFFIXES include)

find_library(RNNOISE_LIBRARY
  rnnoise
  HINTS 
  ${RNNOISE_SOURCE_DIR}
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /opt
  /opt/local
  /usr
  /usr/local
  ENV RNNOISE_DIR
  PATH_SUFFIXES lib64 lib .libs)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(rnnoise DEFAULT_MSG RNNOISE_INCLUDE_DIR RNNOISE_LIBRARY)

if(RNNOISE_FOUND)
  set(RNNOISE_INCLUDE_DIRS ${RNNOISE_INCLUDE_DIR})
  set(RNNOISE_LIBRARIES ${RNNOISE_LIBRARY})
endif(RNNOISE_FOUND)

mark_as_advanced(RNNOISE_INCLUDE_DIR RNNOISE_LIBRARY)
