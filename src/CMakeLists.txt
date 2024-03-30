file(GLOB SOURCES main.cpp)

add_subdirectory(lang)

include_directories(./)
include_directories(../lua/)

add_executable(revdb ${SOURCES})
# pull in common dependencies
target_link_libraries(revdb
    lang
)

