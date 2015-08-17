# TEST: sub module

add_test_exe    (tString tString.cpp)
test_link_libs  (tString mtarcommon)
create_test     (tString)

add_test_exe    (tPath tPath.cpp)
test_link_libs  (tPath mtarcommon)
create_test     (tPath)
if (APPLE)
  add_comp_flag(tPath "-Wno-c++11-narrowing")
endif()
