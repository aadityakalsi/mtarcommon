# project

cmake_minimum_required (VERSION 2.8)

# -- Include hunter
include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.12.5.tar.gz"
    SHA1 "a54fad3f61e35ba43ccede0c57c1a12e37e95e5f"
)

# -- Set project name
set(PROJ_NAME mtarcommon)

project (${PROJ_NAME})

# -- Set install dirs
if (DEFINED ENV{INSTALL_BASE_DIR})
  set(PROJ_INSTALL_DIR $ENV{INSTALL_BASE_DIR})
else()
  set(PROJ_INSTALL_DIR ${CMAKE_SOURCE_DIR}/install)
endif()

set(CPACK_PACKAGE_VERSION "0.1.1")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "1")
set(CPACK_PACKAGE_CONTACT )

set(PROJ_LICENSE_FILE COPYING)
set(PROJ_README_FILE  README.txt)

# -- Include the tests in the bin folder?
set(INCLUDE_TEST_IN_BIN FALSE)
set(USE_CPP TRUE)
set(USE_CPP11 TRUE)

# -- Include the magic ProjTools
include("cmake/ProjTools.cmake")

# -- Add the subdirectories
set(PROJ_SUBDIRS  unittest; doc; tools; test)

# add all subdirs
foreach(subdir ${PROJ_SUBDIRS})
  add_subdirectory(${subdir})
endforeach(subdir)

#################################################################
# Proj deps                                                     #
#################################################################

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

hunter_add_package(ZLIB)
hunter_add_package(BZip2)
#hunter_add_package(Boost COMPONENTS filesystem iostreams)

# -- find ZLIB
find_package(ZLIB REQUIRED)
get_target_property(zlib_libs ZLIB::zlib IMPORTED_LOCATION_RELEASE)
# -- find BZip2
find_package(BZip2 REQUIRED CONFIG)
get_target_property(bzip2_libs BZip2::bz2 IMPORTED_LOCATION_RELEASE)
# -- find Boost
#set(Boost_USE_STATIC_LIBS ON)
#if(MSVC)
#  add_definitions(-DBOOST_ALL_NO_LIB=1)
#endif()

#find_package(Boost REQUIRED filesystem iostreams system)

# All libs have the same include dir
set(zlib_incl  ${ZLIB_INCLUDE_DIR})
set(bzip2_incl ${ZLIB_INCLUDE_DIR})

#set(boost_incl ${Boost_INCLUDE_DIR})
#set(boost_libs ${Boost_LIBRARIES})

set(boost_incl )
set(boost_libs )

#################################################################
# Proj sources                                                  #
#################################################################

message("-- [mtarcommon] Found ZLIB  includes: ${zlib_incl}")
message("-- [mtarcommon] Found BZip2 includes: ${bzip2_incl}")
#message("-- [mtarcommon] Found Boost includes: ${boost_incl}")

if(NOT WIN32)
  set(thread_lib pthread)
endif()

set(PUB_HDRS
    defs.hpp;
    allocator.hpp;
    string.hpp;
    path.hpp)

set(INT_HDRS
    )

set(LIB_SRCS
    allocator.cpp;
    path.cpp)

add_lib(mtarcommon SHARED ${PUB_HDRS} ${INT_HDRS} ${LIB_SRCS})
add_inc_dir(mtarcommon ${zlib_incl})
add_lib_build_def(mtarcommon MTAR_COMMON_BUILD)
link_libs(mtarcommon ${zlib_libs} ${bzip2_libs} ${thread_lib})
set_tgt_ver(mtarcommon 0.1.1 0.1)
install_tgt(mtarcommon)

install_hdr(${PUB_HDRS})

