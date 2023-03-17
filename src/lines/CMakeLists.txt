set(LINES_NAME lines)

add_library(${LINES_NAME} OBJECT
    line.cpp
    line_pair.cpp
)

target_include_directories(${LINES_NAME} PUBLIC .)

