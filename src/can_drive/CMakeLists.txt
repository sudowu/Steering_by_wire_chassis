cmake_minimum_required(VERSION 3.8)
project(can_drive)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(can_msgs REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

# 构建可执行文件
add_executable(can_drive_node src/can_drive.cpp)
target_include_directories(can_drive_node PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include/can_drive
)
# 原始写法（Lyrical 已移除 ament_target_dependencies 命令，保留注释备查）
# ament_target_dependencies(can_drive_node
#   rclcpp
#   std_msgs
#   can_msgs
# )
target_link_directories(can_drive_node PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/lib
)
target_link_libraries(can_drive_node
  rclcpp::rclcpp
  std_msgs::std_msgs
  can_msgs::can_msgs
  controlcan
)

# 设置 RPATH 使可执行文件能在安装目录下找到 libcontrolcan.so
set_target_properties(can_drive_node PROPERTIES
  BUILD_RPATH "${CMAKE_CURRENT_SOURCE_DIR}/lib"
  INSTALL_RPATH "$ORIGIN"
  LINK_FLAGS "-Wl,--disable-new-dtags"
)

# 安装
install(TARGETS can_drive_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY lib/
  DESTINATION lib/${PROJECT_NAME}
  FILES_MATCHING PATTERN "*.so"
)

ament_package()
