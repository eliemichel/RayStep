# Automatically generate moc files for Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5Widgets)
if(Qt5Widgets_FOUND)
	message("Using Qt ${Qt5Widgets_VERSION} from ${Qt5Widgets_INCLUDE_DIRS}")
endif()

add_executable(
	RayStep
	resources.qrc
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
	ViewportWindow.h
	ViewportWindow.ui
	ViewportWindow.cpp
	TextWindow.h
	TextWindow.ui
	TextWindow.cpp
	RenderWidget.h
	RenderWidget.cpp
	SceneTreeModel.h
	SceneTreeModel.cpp
	SceneGraph.h
	SceneGraph.cpp
	NodePropertiesModel.h
	NodePropertiesModel.cpp
	SceneTreeView.h
	SceneTreeView.cpp
	SceneGraphIntrospector.h
	SceneGraphIntrospector.cpp
	UniformsModel.h
	UniformsModel.cpp

	augen/ResourceManager.h
	augen/ResourceManager.cpp
	augen/Shader.h
	augen/Shader.cpp
	augen/ShaderPreprocessor.h
	augen/ShaderPreprocessor.cpp
	augen/Actorio.h
	augen/Actorio.cpp
	augen/Camera.h
	augen/Camera.cpp

	main.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(RayStep glad)
target_link_libraries(RayStep glm)
target_link_libraries(RayStep upng)
target_link_libraries(RayStep json)

target_link_libraries_and_dll(RayStep Qt5::Core)
target_link_libraries_and_dll(RayStep Qt5::Gui)
target_link_libraries_and_dll(RayStep Qt5::Widgets)
