cmake_minimum_required(VERSION 3.30)

project(wav_player)

add_executable(${PROJECT_NAME})

target_compile_options(
    ${PROJECT_NAME} 
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic 
)

add_subdirectory(portaudio EXCLUDE_FROM_ALL)

add_subdirectory(src)

target_include_directories(
    ${PROJECT_NAME} 
    PRIVATE
        ${PortAudio_SOURCE_DIR}/include
        inc
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
        portaudio
)