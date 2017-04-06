file(GLOB sources *.cpp)
add_executable(learnopengl.hellotriangle ${sources})
add_dependencies(learnopengl.hellotriangle glew glfw)
target_link_libraries(learnopengl.hellotriangle glew32 glfw3 opengl32)
