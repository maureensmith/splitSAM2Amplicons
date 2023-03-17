add_subdirectory(errors)
add_subdirectory(files)
add_subdirectory(lines)

target_link_libraries(${CMAKE_PROJECT_NAME}
    errors
    files
    lines
    stdc++fs
)

