##################################################
# epoc values #
##################################################

add_executable (values values.c)
ADD_DEPENDENCIES(values emokit)
target_link_libraries (values emokit ${LIBS})

SET_TARGET_PROPERTIES(values PROPERTIES
INSTALL_RPATH_USE_LINK_PATH TRUE
INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib
BUILD_WITH_INSTALL_RPATH TRUE )

INSTALL (TARGETS values
RUNTIME DESTINATION bin
)
