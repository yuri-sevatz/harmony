cmake_minimum_required(VERSION 2.8.8)

# project details
# ---------------
project(harmony)

set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_PATCH "0")
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

#find_package(Qt5 COMPONENTS Core Network WebKit WebKitWidgets Xml REQUIRED)

# configuration

OPTION(INSTALL_HEADERS "Build Development Headers" OFF)

CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/libharmony/include/config.h.cmake ${CMAKE_SOURCE_DIR}/libharmony/include/config.h)

# add your headers and source files here
# ---------------

set(${PROJECT_NAME}_pub_headers
    action.hpp
    csocket.hpp
    device.hpp
    function.hpp
    harmony.hpp
)

set(${PROJECT_NAME}_pub_sources

)

set(${PROJECT_NAME}_pri_headers

)

set(${PROJECT_NAME}_pri_sources
    csocket.cpp
    harmony.cpp
)

set(${PROJECT_NAME}_headers
    ${${PROJECT_NAME}_pub_headers}
    ${${PROJECT_NAME}_pri_headers}
)

set(${PROJECT_NAME}_sources
    ${${PROJECT_NAME}_pub_sources}
    ${${PROJECT_NAME}_pri_sources}
)

#QT5_WRAP_CPP(${PROJECT_NAME}_headers_moc ${${PROJECT_NAME}_headers})

# Create the library
# ---------------
add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_sources})

#qt5_use_modules(${PROJECT_NAME} Core Network WebKit WebKitWidgets Xml)

# Install
# ---------------
install(TARGETS ${PROJECT_NAME}
                RUNTIME DESTINATION bin
                LIBRARY DESTINATION lib
                ARCHIVE DESTINATION lib/static)

IF(DEFINED INSTALL_HEADERS)
install(DIRECTORY . DESTINATION include/lib${PROJECT_NAME} FILES_MATCHING PATTERN "*.hpp")
ENDIF(DEFINED INSTALL_HEADERS)
