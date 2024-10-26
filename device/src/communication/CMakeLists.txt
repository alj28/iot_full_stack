
#add_subdirectory(./zigbee)

if(${BOARD} STREQUAL "native_posix")
    add_subdirectory(./mqtt)
endif()

target_sources(
    app
    PRIVATE
    ./communication_hub.c
)

target_include_directories(
    app
    PRIVATE
    ./
)