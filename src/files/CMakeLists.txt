set(FILES_NAME files)

add_library(${FILES_NAME} OBJECT
    files.cpp
    input.cpp
    output.cpp
)

target_include_directories(${FILES_NAME} PUBLIC .)

target_link_libraries(${FILES_NAME}
    pthread #generic thread lib?
)

