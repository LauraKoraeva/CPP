cmake_minimum_required(VERSION 3.17)


set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
add_executable(focus_timer
	main.cpp 
	${MY_INCLUDE_DIR}/Config.h 
	Config.cpp
	${MY_INCLUDE_DIR}/Statistics.h 
	Statistics.cpp
	${MY_INCLUDE_DIR}/Session.h 
	Session.cpp
	${MY_INCLUDE_DIR}/Timer.h 
	Timer.cpp
	${MY_INCLUDE_DIR}/UI.h 
	UI.cpp 
	${MY_INCLUDE_DIR}/Utility.h 
	Utility.cpp 
)
target_include_directories(focus_timer PUBLIC ${MY_INCLUDE_DIR})