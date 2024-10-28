
target_sources(
    app
    PRIVATE
    ./mqtt.c
    ./mqtt_shell.c
    ./mqtt_settings.c
)

target_include_directories(
    app
    PUBLIC
    ./
)