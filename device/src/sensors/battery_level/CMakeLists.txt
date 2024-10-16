
target_sources(
    app
    PRIVATE
    ./battery_level.c
    ./battery_level_shell.c
)

target_include_directories(
    app
    PUBLIC
    ./
)
