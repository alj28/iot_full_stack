
#add_subdirectory(./zigbee)

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