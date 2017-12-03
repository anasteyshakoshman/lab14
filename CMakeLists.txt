cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)
include(cmake/HunterGate.cmake)

HunterGate(
    URL "https://github.com/anasteyshakoshman/hunter/archive/v0.19.137.3.tar.gz"
    SHA1 1290f4b8d32ce4fef2b9101c9d951649224e1ca5
)
project(builder)

hunter_add_package(Boost COMPONENTS program_options system filesystem)
find_package(Boost CONFIG REQUIRED program_options system filesystem)

add_executable(builder ${CMAKE_CURRENT_SOURCE_DIR}/builder.cpp)
target_link_libraries(builder Boost::program_options Boost::system Boost::filesystem)

install(TARGETS builder RUNTIME DESTINATION bin)
