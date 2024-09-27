
target_sources(
    app
    PRIVATE
    ./zigbee.c
    ./zigbee_commissioning.c
)

target_include_directories(
    app
    PUBLIC
    ./
)