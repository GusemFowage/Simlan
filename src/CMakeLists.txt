include_directories(.)

add_executable(
    simlan "main.cpp"
    "Simlan/Standard.hpp" "Simlan/Standard.cpp"
    "Lexer/Source.hpp" "Lexer/Source.cpp"
    "Lexer/Lexer.hpp" "Lexer/Lexer.cpp")