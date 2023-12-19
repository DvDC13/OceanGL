target_sources(imgui_lib PRIVATE
            sources/imgui.cpp
            sources/imgui_demo.cpp
            sources/imgui_draw.cpp
            sources/imgui_widgets.cpp
            sources/imgui_tables.cpp
            backend/imgui_impl_glfw.cpp
            backend/imgui_impl_opengl3.cpp
            )