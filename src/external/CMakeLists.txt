# Glad
add_subdirectory(glad)
set_property(TARGET glad PROPERTY FOLDER "External")

# glm (0.9.8.5)
add_subdirectory(glm)
#set_property(TARGET glm_dummy PROPERTY FOLDER "External")

# uPNG
add_subdirectory(upng)
set_property(TARGET upng PROPERTY FOLDER "External")

# json
add_subdirectory(json)
#set_property(TARGET json PROPERTY FOLDER "External")
