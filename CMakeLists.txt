cmake_minimum_required(VERSION 3.16)
project(progettopf VERSION 0.1.0)

# abilita il supporto per i test, tra cui l'opzione BUILD_TESTING usata sotto
include(CTest)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# richiedi l'uso di C++17, senza estensioni non-standard offerte dal compilatore usato 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# abilita warning
string(APPEND CMAKE_CXX_FLAGS
      " -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion"
      " -Wshadow -Wimplicit-fallthrough -Wextra-semi -Wold-style-cast -Wreorder")

# abilita asserzioni di debug (in gcc), l'address sanitizer e l'undefined-behaviour sanitizer in debug mode
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -D_GLIBCXX_ASSERTIONS -fsanitize=address,undefined -fno-omit-frame-pointer")
string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " -fsanitize=address,undefined -fno-omit-frame-pointer")

# cerca libreria sfml
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# se il testing e' abilitato...
#   per disabilitare il testing, passare -DBUILD_TESTING=OFF a cmake durante la fase di configurazione
if (BUILD_TESTING)

# aggiungi l'eseguibile all.t
add_executable(test.t test.cpp flock.cpp vector2d.cpp bird.cpp)
target_link_libraries(test.t sfml-graphics sfml-window sfml-system sfml-widgets)

# aggiungi l'eseguibile all.t alla lista dei test
add_test(NAME test.t COMMAND test.t)

endif()

add_executable(flock_simulation main.cpp bird.cpp flock.cpp vector2d.cpp text_table.cpp)

add_subdirectory(third_party/sfml-widgets SYSTEM)

target_link_libraries(flock_simulation sfml-graphics sfml-window sfml-system sfml-widgets)


# disabilita i warning della libreria sfml-widgets
target_compile_options(sfml-widgets PRIVATE -w)






