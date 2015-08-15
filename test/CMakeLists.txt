# TEST: sub module

add_test_exe    (tAllocator tAllocator.cpp)
test_link_libs  (tAllocator mtarcommon)
create_test     (tAllocator)
