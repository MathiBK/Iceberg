cmake_minimum_required(VERSION 3.14)
project(iceberg)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")


add_executable(iceberg main.cpp BaseState.h state_machine.cpp state_machine.h
        map/maptest.cpp map/maptest.h objects/layer.cpp

        objects/layer.h objects/object.h objects/sprite.cpp objects/sprite.h
        objects/entities/player.cpp objects/entities/player.h
        states/play_state.cpp states/play_state.h objects/object_physics.cpp
        objects/object_physics.h objects/entities/entity.cpp objects/entities/entity.h objects/entities/slime.cpp

        utility/polygon.h abilities/ability.h abilities/dash.cpp abilities/dash.h
        abilities/grapplingHook.cpp abilities/grapplingHook.h objects/particles/grappler.cpp objects/particles/grappler.h objects/particles/particle.h

        objects/entities/skeleton.cpp objects/entities/skeleton.h
        animation/animation.cpp animation/animation.h save_manager.cpp save_manager.h utility/sound_manager.cpp utility/sound_manager.h
        states/death_state.cpp states/death_state.h utility/texture_manager.cpp utility/texture_manager.h
        objects/on_screen/on_screen_object.cpp objects/on_screen/on_screen_object.h objects/checkpoint.cpp objects/checkpoint.h states/config_state.cpp
        states/config_state.h objects/entities/redSlime.cpp objects/entities/redSlime.h objects/entities/greenSlime.cpp objects/entities/greenSlime.h
        objects/entities/bigSlime.cpp objects/entities/bigSlime.h


        abilities/bow.cpp abilities/bow.h
        objects/particles/arrow.cpp objects/particles/arrow.h
        states/pause_state.cpp states/pause_state.h
        states/upgrade_state.cpp states/upgrade_state.h
        states/choose_ability_state.cpp states/choose_ability_state.h
        abilities/basic_attack.cpp
        abilities/basic_attack.h abilities/none_ability.cpp abilities/none_ability.h states/splash_state.cpp states/splash_state.h
        states/main_menu_state.cpp states/main_menu_state.h)

target_link_libraries(iceberg jsoncpp sfml-graphics sfml-audio sfml-window sfml-system)
target_include_directories(iceberg PUBLIC .)
target_include_directories(iceberg PUBLIC /usr/include/jsonpp)

