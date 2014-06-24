cmake_minimum_required(VERSION 2.8.0)
project(audio_player)

# Find all required modules for compilation
find_package(catkin REQUIRED COMPONENTS
    roscpp
    std_msgs
    SDL
    SDL_mixer)


# Nothing that can be imported by other modules
catkin_package()

include_directories(${catkin_INCLUDE_DIRS})
add_executable(audio_player src/sdl_play.cpp)
target_link_libraries(audio_player ${catkin_LIBRARIES})

install(TARGETS audio_player
    RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION} )

