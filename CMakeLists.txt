# CMakeList.txt : CMake project for CMakeProject1, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.20)

project ("myapp")

# Add source to this project's executable.
find_package(wxWidgets REQUIRED COMPONENTS net core base)
include(${wxWidgets_USE_FILE})
add_executable (myapp main.cpp MyFrame.cpp MyFrame.hpp Game.cpp Game.hpp "Logika/Board.cpp" "Logika/Cell.cpp" "Logika/Board.hpp" "Logika/Cell.hpp")
target_link_libraries(myapp ${wxWidgets_LIBRARIES})

if (CMAKE_VERSION VERSION_GREATER 3.20)
	set_property(TARGET myapp PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.