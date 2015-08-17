# TEST: sub module

add_test_exe    (tString tString.cpp)
test_link_libs  (tString mtarcommon ${thread_lib})
create_test     (tString)
if (NOT USING_CLANG AND NOT WIN32)
  add_link_flag   (tString -Wl,--no-as-needed)
endif()

add_test_exe    (tPath tPath.cpp)
test_link_libs  (tPath mtarcommon)
create_test     (tPath)
if (NOT WIN32)# GCC
  add_comp_flag(tPath "-Wno-narrowing")
endif()
