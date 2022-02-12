cmake_minimum_required(VERSION 3.10)

project(simulation VERSION 1.0 LANGUAGES CXX)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

add_executable( ${PROJECT_NAME}
    ${PROJECT_SOURCE_DIR}/src/Chain_ship.cpp
    ${PROJECT_SOURCE_DIR}/src/Controller.cpp
    ${PROJECT_SOURCE_DIR}/src/Cruise_ship.cpp
    ${PROJECT_SOURCE_DIR}/src/Cruiser.cpp
    ${PROJECT_SOURCE_DIR}/src/Geometry.cpp
    ${PROJECT_SOURCE_DIR}/src/Island.cpp
    ${PROJECT_SOURCE_DIR}/src/Local_view.cpp
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/Map_view.cpp
    ${PROJECT_SOURCE_DIR}/src/Model.cpp
    ${PROJECT_SOURCE_DIR}/src/Navigation.cpp
    ${PROJECT_SOURCE_DIR}/src/Sailing_view.cpp
    ${PROJECT_SOURCE_DIR}/src/Ship_component_factory.cpp
    ${PROJECT_SOURCE_DIR}/src/Ship_component.cpp
    ${PROJECT_SOURCE_DIR}/src/Ship_composite.cpp
    ${PROJECT_SOURCE_DIR}/src/Ship.cpp
    ${PROJECT_SOURCE_DIR}/src/Sim_object.cpp
    ${PROJECT_SOURCE_DIR}/src/Tanker.cpp
    ${PROJECT_SOURCE_DIR}/src/Torpedo_boat.cpp
    ${PROJECT_SOURCE_DIR}/src/Track_base.cpp
    ${PROJECT_SOURCE_DIR}/src/Twod_view.cpp
    ${PROJECT_SOURCE_DIR}/src/Utility.cpp
    ${PROJECT_SOURCE_DIR}/src/View.cpp
    ${PROJECT_SOURCE_DIR}/src/Warship.cpp
)