execute_process(COMMAND "/home/ubuntu/catkin_ws/build/turtlebot3_autorace/turtlebot3_autorace_camera/catkin_generated/python_distutils_install.sh" RESULT_VARIABLE res)

if(NOT res EQUAL 0)
  message(FATAL_ERROR "execute_process(/home/ubuntu/catkin_ws/build/turtlebot3_autorace/turtlebot3_autorace_camera/catkin_generated/python_distutils_install.sh) returned error code ")
endif()
