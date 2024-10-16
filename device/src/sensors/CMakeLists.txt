
target_sources(
    app
    PRIVATE
    ./sensors_hub.c
)

target_include_directories(
    app
    PUBLIC
    ./
)

add_subdirectory(battery_level)