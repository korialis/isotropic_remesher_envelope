cmake_minimum_required(VERSION 3.10)

#set vcpkg conf
set(VCPKG_TARGET_TRIPLET "x64-windows")

#set standard & other conf
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#set windows specific flags
if(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
	add_definitions(-DWIN32 -D_WIN32 -D_WIN64 -DNOMINMAX)
endif()

project(isotropicremeshing VERSION 0.1 LANGUAGES CXX)

#fetch external dependencies, either from github or from local conf
include(FetchContent)

#vcgdir
if (VCGDIR) # VCGDIR exists - using custom user vcglib path
	if(EXISTS ${VCGDIR})
		add_subdirectory(${VCGDIR} ${CMAKE_CURRENT_BINARY_DIR}/vcglib)
		message(STATUS "- VCGLib - using custom VCGDIR path library")
	else()
		set(VCGDIR NOTFOUND)
	endif()
else()
	#fetch vcglib from github, pull the devel branch
	FetchContent_Declare(
		vcglib_devel
		GIT_REPOSITORY https://github.com/cnr-isti-vclab/vcglib.git
		GIT_TAG        devel #devel branch
	)
	FetchContent_MakeAvailable(vcglib_devel)
	message(STATUS " - vcglib - downloaded vcglib#devel from github")
endif()
set(VCGDIR "${vcglib_devel_SOURCE_DIR}")
message(STATUS " - vcglib - VCGDIR=${VCGDIR}")

if (FASTENVELOPE) # VCGDIR exists - using custom user vcglib path
	if(EXISTS ${FASTENVELOPE})
		add_subdirectory(${FASTENVELOPE} ${CMAKE_CURRENT_BINARY_DIR}/vcglib)
		message(STATUS "- fast-envelope - using custom fast-envelope path library")
	else()
		set(FASTENVELOPE NOTFOUND)
	endif()
else()
	#fetch vcglib from github, pull the devel branch
	FetchContent_Declare(
		fast-envelope
		GIT_REPOSITORY https://github.com/wangbolun300/fast-envelope
	)
	FetchContent_MakeAvailable(fast-envelope)
	message(STATUS " - vcglib - downloaded vcglib#devel from github")
endif()
set(FASTENVELOPE "${fast-envelope_SOURCE_DIR}")
message(FASTENVELOPE " - fast-envelope - FASTENVELOPE=${FASTENVELOPE}")

file(GLOB SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_sources(${PROJECT_NAME} PRIVATE ${VCGDIR}/wrap/ply/plylib.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}")

target_link_libraries(${PROJECT_NAME} PRIVATE
	vcglib
	FastEnvelope
	spdlog::spdlog
)