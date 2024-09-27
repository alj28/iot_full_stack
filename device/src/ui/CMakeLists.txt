
target_sources(
    app
    PRIVATE
    buttons.c
    leds.c
)

target_include_directories(
    app
    PUBLIC
    ./
)