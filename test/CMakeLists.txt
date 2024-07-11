include(AddCatch2)

add_executable(
        _tests
        test.cpp
        other_test.cpp

)
target_link_libraries(
        _tests PRIVATE procUtils LEAF Catch2::Catch2WithMain
)
#target_link_libraries(tests unit_tests)
#target_link_libraries(tests Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(_tests)
