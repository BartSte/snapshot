function(find_static_library LIB_NAME OUT)

  if(WIN32 OR MSVC)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
  elseif(UNIX)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
  endif()

  find_library(FOUND_${LIB_NAME}_STATIC ${LIB_NAME})

  if(FOUND_${LIB_NAME}_STATIC)
    get_filename_component(ABS_FILE ${FOUND_${LIB_NAME}_STATIC} ABSOLUTE)
  else()
    message(SEND_ERROR "Unable to find library ${LIB_NAME}")
  endif()

  set(${OUT}
      ${ABS_FILE}
      PARENT_SCOPE)

endfunction()
