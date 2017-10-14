# Automatically generate moc files for Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5Widgets)
if(Qt5Widgets_FOUND)
	message("Using Qt ${Qt5Widgets_VERSION} from ${Qt5Widgets_INCLUDE_DIRS}")
endif()

add_executable(
	piced
	ResourceManager.h
	Logger.h
	Logger.cpp
	utils/strutils.h
	utils/strutils.cpp
	utils/fileutils.h
	utils/fileutils.cpp
	Future.h
	MainWindow.h
	MainWindow.ui
	MainWindow.cpp
	RenderWidget.h
	RenderWidget.cpp

	augen/GenericApp.h
	augen/ResourceManager.h
	augen/ResourceManager.cpp
	augen/Shader.h
	augen/Shader.cpp
	augen/ShaderPreprocessor.h
	augen/ShaderPreprocessor.cpp
	augen/Actorio.h
	augen/Actorio.cpp

	main.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(piced glad)
target_link_libraries(piced glm)
target_link_libraries(piced upng)
target_link_libraries(piced json)

target_link_libraries_and_dll(piced Qt5::Core)
target_link_libraries_and_dll(piced Qt5::Gui)
target_link_libraries_and_dll(piced Qt5::Widgets)
