include_directories(.)

file(GLOB_RECURSE ALL_SOURCE "*.cpp" "*.c")
file(GLOB_RECURSE ALL_HEAD "*.hpp" "*.h")

add_executable(
    simlan ${ALL_HEAD} ${ALL_SOURCE} 
)
