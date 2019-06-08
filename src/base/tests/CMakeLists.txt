include_directories(../)

add_executable(NonCopyable_test NonCopyable_test.cpp)
target_link_libraries(NonCopyable_test spero_base)

add_executable(TypeCast_test TypeCast_test.cpp)
target_link_libraries(TypeCast_test spero_base)

add_executable(Mutex_test Mutex_test.cpp)
target_link_libraries(Mutex_test spero_base)
