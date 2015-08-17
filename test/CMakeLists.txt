# TEST: sub module

add_test_exe    (tString tString.cpp)
test_link_libs  (tString mtarcommon)
create_test     (tString)

add_test_exe    (tPath tPath.cpp)
test_link_libs  (tPath mtarcommon)
create_test     (tPath)
if (NOT WIN32)# GCC
  add_comp_flag(tPath "-Wno-narrowing")
endif()
