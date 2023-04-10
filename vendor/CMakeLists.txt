include(FetchContent)
set(FETCHCONTENT_QUIET NO)

if (NOT sfml_FOUND)
    FetchContent_Declare(
            sfml
            GIT_REPOSITORY "https://github.com/SFML/SFML"
            GIT_TAG 2.5.x
            SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SFML
    )
    FetchContent_MakeAvailable(sfml)
endif ()

if (NOT imgui_FOUND)
    FetchContent_Declare(
            imgui
            GIT_REPOSITORY https://github.com/ocornut/imgui
            GIT_TAG e25e4526cd41cd6536194de098540d54244f54e9
            SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    )
    FetchContent_MakeAvailable(imgui)
endif ()

if (NOT imgui-sfml)
    FetchContent_Declare(
            imgui-sfml
            GIT_REPOSITORY https://github.com/eliasdaler/imgui-sfml
            GIT_TAG f0d8f72f073b367e9506b8794158cc91f015252e
            SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui-sfml
    )
    set(IMGUI_DIR ${imgui_SOURCE_DIR})
    set(IMGUI_SFML_FIND_SFML OFF)
    set(IMGUI_SFML_IMGUI_DEMO OFF)
    FetchContent_MakeAvailable(imgui-sfml)
endif ()