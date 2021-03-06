PROJECT(FastCgiQt)
# Stop cmake 2.6 from whining
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
IF(COMMAND cmake_policy)
	CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})

# If building for release, don't include QDebug stuff
IF("x${CMAKE_BUILD_TYPE}" STREQUAL "x")
	SET(CMAKE_BUILD_TYPE "Release")
ENDIF()
IF("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	ADD_DEFINITIONS("-DQT_NO_DEBUG_OUTPUT")
ENDIF()

# Developer options
OPTION(WARNINGS_AS_ERRORS "Treat all compiler warnings as errors" OFF)

# Configuration options
OPTION(BUILD_EXAMPLES "Build several example applications" OFF)
OPTION(WITH_SERVICE_SUPPORT "Build helper classes for service-based applications" ON)
OPTION(WITH_SQL_SUPPORT "Build helper classes for interacting with databases" ON)
OPTION(WITH_MEMCACHED_SUPPORT "Build plugin for using memcached - needs libmemcached" OFF)
OPTION(WITH_FASTCGI_SUPPORT "Build support for the FastCGI interface" ON)
OPTION(WITH_SCGI_SUPPORT "Build support for the SCGI interface" ON)
OPTION(WITH_HTTP_SUPPORT "Build support for an embedded HTTPD (only for development)" ON)
OPTION(WITH_CGI_SUPPORT "Build support for plain old CGI" ON)

IF(WITH_SERVICE_SUPPORT)
	MESSAGE(STATUS "Service support is enabled.")
	OPTION(WITH_XSLT_SUPPORT "Build helper class for XSLT-based applications - needs Qt 4.5" ON)
ELSE()
	MESSAGE(STATUS "Service support is *NOT* enabled.")
ENDIF()

# Platform configuration
IF(CMAKE_COMPILER_IS_GNUCXX)
	MESSAGE(STATUS "Applying platform configuration for GCC")
	ADD_DEFINITIONS("-Wall")
	IF(WARNINGS_AS_ERRORS)
		ADD_DEFINITIONS("-Werror")
	ENDIF()
ELSEIF(CMAKE_CXX_COMPILER_ID STREQUAL "SunPro")
	MESSAGE(STATUS "Applying platform configuration for Sun Studio compiler")
	ADD_DEFINITIONS(
		"-erroff=nullref" # incompatible with qobject_cast<>
		"-errtags=yes"
		"-DWITHOUT_FLOCK"
	)
	IF(WARNINGS_AS_ERRORS)
		ADD_DEFINITIONS("-errwarn=%all")
	ENDIF()
ELSE()
	MESSAGE(WARNING "Don't have a specific configuration for this compiler: ${CMAKE_CXX_COMPILER_ID}")
ENDIF()


# Find Qt4
ADD_DEFINITIONS(-DQT_STATICPLUGIN)
FIND_PACKAGE( Qt4 REQUIRED )

SET(CMAKE_OSX_ARCHITECTURES i386)

SET(QT_DONT_USE_QTGUI TRUE)
SET(QT_USE_QTNETWORK TRUE)
IF(WITH_SQL_SUPPORT)
	SET(QT_USE_QTSQL TRUE)
	MESSAGE(STATUS "SQL support is enabled.")
ELSE()
	MESSAGE(STATUS "SQL support is *NOT* enabled.")
ENDIF()
IF(WITH_XSLT_SUPPORT)
	SET(QT_USE_QTXMLPATTERNS TRUE)
ENDIF()

# Include the cmake file needed to use qt4
INCLUDE( ${QT_USE_FILE} )

# Subdirs
ADD_SUBDIRECTORY(lib)
IF(BUILD_EXAMPLES)
	ADD_SUBDIRECTORY(examples)
ENDIF()
