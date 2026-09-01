cmake_minimum_required(VERSION 3.8)
project(xbox_joy)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(teleop_twist_joy REQUIRED)
find_package(rclcpp_components REQUIRED)

add_executable(xbox_joy_node src/xbox.cpp)

# 原始写法（Lyrical 已移除 ament_target_dependencies 命令，保留注释备查）
# ament_target_dependencies(xbox_joy_node
#   rclcpp
#   sensor_msgs
#   std_msgs
#   geometry_msgs
#   teleop_twist_joy
#   rclcpp_components
# )

# 源码实际只用 rclcpp 和 sensor_msgs；其余包仅保留 find_package，无需链接。
# 注：rclcpp_components 没有导出 ::rclcpp_components 顶层 target（只有 ::component / ::component_manager），
# 且 xbox.cpp 并未使用它。
target_link_libraries(xbox_joy_node
  rclcpp::rclcpp
  sensor_msgs::sensor_msgs
)

install(TARGETS xbox_joy_node
  DESTINATION lib/${PROJECT_NAME})

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

ament_package()
