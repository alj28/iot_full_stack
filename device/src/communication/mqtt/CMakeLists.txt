
target_sources(
    app
    PRIVATE
    ./mqtt.c
)

target_include_directories(
    app
    PUBLIC
    ./
)