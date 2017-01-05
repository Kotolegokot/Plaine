find_path(ASIO_INCLUDE_DIR asio.hpp /usr/include /usr/local/include)

#find_library(ASIO_LIBRARY NAMES asio PATH /usr/lib /usr/local/lib) 

if (ASIO_INCLUDE_DIR)
   set(ASIO_FOUND true)
endif (ASIO_INCLUDE_DIR)


if (ASIO_FOUND)
   message(STATUS "Found ASIO: ${ASIO_INCLUDE_DIR}")
else (ASIO_FOUND)
   if (ASIO_FIND_REQUIRED)
       message(FATAL_ERROR "Could not find ASIO")
   endif (ASIO_FIND_REQUIRED)
endif (ASIO_FOUND)
