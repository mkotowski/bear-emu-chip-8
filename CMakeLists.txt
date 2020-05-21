cmake_minimum_required(VERSION 3.17.0 FATAL_ERROR)

project(BearEmuChip8 VERSION 0.1.0 LANGUAGES CXX C)

# Let's ensure -std=c++xx instead of -std=g++xx
set(CMAKE_CXX_EXTENSIONS OFF)

# INSTALL DIRECTORY ##################################################

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	set(CMAKE_INSTALL_PREFIX 
		"${PROJECT_SOURCE_DIR}/compiled" 
		CACHE PATH 
		"Set the destination of installed packages." 
		FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

# UPX COMPRESSION ####################################################

find_package(SelfPackers REQUIRED)
message(STATUS "${SELF_PACKER_FOR_EXECUTABLE}")

# APP FILES ##########################################################

file(GLOB 
	SOURCE_FILES 
	CONFIGURE_DEPENDS 
	"${PROJECT_SOURCE_DIR}/src/*.cpp")

file(GLOB 
	HEADER_FILES 
	CONFIGURE_DEPENDS 
	"${PROJECT_SOURCE_DIR}/include/*.hpp")

# WINTOAST LIB #######################################################

file(GLOB 
	WINTOAST_SOURCE_FILES 
	CONFIGURE_DEPENDS 
	"${PROJECT_SOURCE_DIR}/libs/wintoast/*.cpp")

file(GLOB 
	WINTOAST_HEADER_FILES 
	CONFIGURE_DEPENDS 
	"${PROJECT_SOURCE_DIR}/libs/wintoast/*.h")

# EXECUTABLE #########################################################

add_executable(BearEmuChip8  
	"${SOURCE_FILES}" "${HEADER_FILES}"
	"${WINTOAST_SOURCE_FILES}" "${WINTOAST_HEADER_FILES}"
	)

target_include_directories(BearEmuChip8 PUBLIC
	"${PROJECT_SOURCE_DIR}/include"
	"${PROJECT_SOURCE_DIR}/libs/wintoast"
	)

# DOCUMENTATION ######################################################

find_package(Doxygen 
	REQUIRED dot 
	OPTIONAL_COMPONENTS mscgen dia)

set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "README.md")

set(DOXYGEN_GENERATE_HTML  YES)
set(DOXYGEN_GENERATE_LATEX YES)

set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/docs")

doxygen_add_docs(
	documentation
	"${PROJECT_SOURCE_DIR}/README.md"
	"${PROJECT_SOURCE_DIR}/src"
	"${PROJECT_SOURCE_DIR}/include"
	COMMENT "Generating API documentation with Doxygen"
)

# INSTALLING #########################################################

include(InstallRequiredSystemLibraries)
install(TARGETS BearEmuChip8 
	RUNTIME DESTINATION "bin" COMPONENT "BearEmuChip8")

# CPACK ##############################################################

set(CPACK_GENERATOR "NSIS")
set(CPACK_SOURCE_GENERATOR "ZIP")

set(CPACK_OUTPUT_CONFIG_FILE "CPackConfig.cmake")
set(CPACK_OUTPUT_FILE_PREFIX 
	"${CMAKE_SOURCE_DIR}/installers")

set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_VERSION "0.1.0")

# set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.html")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.txt")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
# set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_SOURCE_DIR}/README.html")

set(CPACK_SOURCE_IGNORE_FILES 
	"${CMAKE_SOURCE_DIR}/build/;${CMAKE_SOURCE_DIR}/.git/")

set(CPACK_PACKAGE_EXECUTABLES 
	"BearEmuChip8" "Bear Emulator CHIP-8")

set(CPACK_CREATE_DESKTOP_LINKS 
	"BearEmuChip8")

set(CPACK_PACKAGE_CHECKSUM "MD5")

include(CPack)
