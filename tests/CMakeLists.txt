find_package(Qt5 COMPONENTS Core REQUIRED)

add_executable(test_md_parser test_md_parser.cpp)

target_compile_options(test_md_parser PRIVATE -Wall)

target_include_directories(test_md_parser PRIVATE ${PROJECT_SOURCE_DIR}/3rdparty/catch2/include)

target_link_libraries(test_md_parser Qt5::Core)

add_test(NAME test_md_parser
         COMMAND test_md_parser
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/tests)
