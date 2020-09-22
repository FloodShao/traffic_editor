cmake_minimum_required(VERSION 3.5)
project(building_crowdsim_pr_test)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)

install(DIRECTORY
  maps/
  DESTINATION share/${PROJECT_NAME}
)

ament_package()

file(GLOB_RECURSE traffic_editor_paths "maps/*.building.yaml")

## for gazebo
foreach(path ${traffic_editor_paths})

  # Get the output world name
  string(REPLACE "." ";" list1 ${path})
  list(GET list1 0 name)
  string(REPLACE "/" ";" list2 ${name})
  list(GET list2 -1 world_name)

  set(map_path ${path})
  set(output_world_name ${world_name})
  set(output_dir ${CMAKE_CURRENT_BINARY_DIR}/maps/${output_world_name})
  set(output_world_path ${output_dir}/${output_world_name}.world)
  set(output_model_dir ${output_dir}/models)

  # Step 1: generate the map.world and navgraph without crowd sim plugin
  # first, generate the world
  add_custom_command(
    OUTPUT ${output_world_path}
    COMMAND ros2 run building_map_tools building_map_generator gazebo ${map_path} ${output_world_path} ${output_model_dir}
    DEPENDS ${map_path}
  )

  add_custom_target(generate_${output_world_name} ALL
    DEPENDS ${output_world_path}
  )

  # Step 2: generate the navmesh and required files
  set(crowd_sim_config_resource ${output_dir}/config_resource/)

  add_custom_command(
    OUTPUT ${world_name}_crowdsim
    COMMAND ros2 run building_map_tools building_crowdsim ${map_path} ${crowd_sim_config_resource} ${output_world_path}
    DEPENDS ${output_world_path}
  )
  add_custom_target(generate_${world_name}_crowdsim ALL
    DEPENDS ${world_name}_crowdsim
  )

  # now, generate the nav graphs
  set(output_nav_graphs_dir ${output_dir}/nav_graphs/)
  set(output_nav_graphs_phony ${output_nav_graphs_dir}/phony)
  add_custom_command(
    OUTPUT ${output_nav_graphs_phony}
    COMMAND ros2 run building_map_tools building_map_generator nav ${map_path} ${output_nav_graphs_dir}
    DEPENDS ${map_path}
  )

  add_custom_target(generate_${output_world_name}_nav_graphs ALL
    DEPENDS ${output_nav_graphs_phony}
  )

  install(
    DIRECTORY ${output_dir}
    DESTINATION share/${PROJECT_NAME}/maps
  )

endforeach()

## for ign
foreach(path ${traffic_editor_paths})

  # Get the output world name
  string(REPLACE "." ";" list1 ${path})
  list(GET list1 0 name)
  string(REPLACE "/" ";" list2 ${name})
  list(GET list2 -1 world_name)

  set(map_path ${path})
  set(output_world_name ${world_name}_ign)
  set(output_dir ${CMAKE_CURRENT_BINARY_DIR}/maps/${output_world_name})
  set(output_world_path ${output_dir}/${output_world_name}.world)
  set(output_model_dir ${output_dir}/models)

  # Step 1: generate the map.world and navgraph without crowd sim plugin
  # first, generate the world
  add_custom_command(
    OUTPUT ${output_world_path}_ign
    COMMAND ros2 run building_map_tools building_map_generator ignition ${map_path} ${output_world_path} ${output_model_dir}
    DEPENDS ${map_path}
  )

  add_custom_target(generate_${output_world_name}_ign ALL
    DEPENDS ${output_world_path}_ign
  )

  # Step 2: generate the navmesh and required files
  set(crowd_sim_config_resource ${output_dir}/config_resource/)

  add_custom_command(
    OUTPUT ${world_name}_crowdsim_ign
    COMMAND ros2 run building_map_tools building_crowdsim ${map_path} ${crowd_sim_config_resource} ${output_world_path}
    DEPENDS ${output_world_path}_ign
  )
  add_custom_target(generate_${world_name}_crowdsim_ign ALL
    DEPENDS ${world_name}_crowdsim_ign
  )

  install(
    DIRECTORY ${output_dir}
    DESTINATION share/${PROJECT_NAME}/maps
  )

endforeach()
