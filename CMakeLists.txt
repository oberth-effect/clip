cmake_minimum_required(VERSION 3.18)
project(Clip VERSION 4.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(Qt5 COMPONENTS Core Concurrent Gui OpenGL PrintSupport Svg WebKitWidgets Widgets Xml REQUIRED)

# QT CMake things
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(TIFF)
find_package(Eigen3 REQUIRED)


#file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/*.cpp)
#file(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}/*.h)
#file(GLOB_RECURSE UI ${PROJECT_SOURCE_DIR}/*.ui)
#file(GLOB_RECURSE RESOURCES ${PROJECT_SOURCE_DIR}/*.qrc)
#
#add_executable(Clip ${SOURCES} ${HEADERS} ${UI} ${RESOURCES})


add_executable(Clip
        config.h
        config/colorbutton.cpp config/colorbutton.h
        config/colorconfigitem.cpp config/colorconfigitem.h
        config/configstore.cpp config/configstore.h
        core/crystal.cpp core/crystal.h
        core/diffractingstereoprojector.cpp core/diffractingstereoprojector.h
        core/laueplaneprojector.cpp core/laueplaneprojector.h
        core/projector.cpp core/projector.h
        core/projectorfactory.cpp core/projectorfactory.h
        core/reflection.cpp core/reflection.h
        core/spacegroup.cpp core/spacegroup.h
        core/spacegroupdata.cpp
        core/stereoprojector.cpp core/stereoprojector.h
        defs.cpp defs.h
        image/basdataprovider.cpp image/basdataprovider.h
        image/beziercurve.cpp image/beziercurve.h
        image/brukerprovider.cpp image/brukerprovider.h
        image/dataprovider.cpp image/dataprovider.h
        image/dataproviderfactory.cpp image/dataproviderfactory.h
        image/datascaler.cpp image/datascaler.h
        image/datascalerfactory.cpp image/datascalerfactory.h
        image/imagedatastore.cpp image/imagedatastore.h
        image/laueimage.cpp image/laueimage.h
        image/mwdataprovider.cpp image/mwdataprovider.h
        image/qimagedataprovider.cpp image/qimagedataprovider.h
        image/simplemonochromscaler.cpp image/simplemonochromscaler.h
        image/simplergbscaler.cpp image/simplergbscaler.h
        image/xyzdataprovider.cpp image/xyzdataprovider.h
        indexing/candidategenerator.cpp indexing/candidategenerator.h
        indexing/indexer.cpp indexing/indexer.h
        indexing/livemarkermodel.cpp indexing/livemarkermodel.h
        indexing/marker.cpp indexing/marker.h
        indexing/solution.cpp indexing/solution.h
        indexing/solutionmodel.cpp indexing/solutionmodel.h
        main.cpp
        refinement/fitobject.cpp refinement/fitobject.h
        refinement/fitparameter.cpp refinement/fitparameter.h
        refinement/fitparametergroup.cpp refinement/fitparametergroup.h
        refinement/fitparametertreeitem.cpp refinement/fitparametertreeitem.h
        refinement/neldermead.cpp refinement/neldermead.h
        refinement/neldermead_worker.cpp refinement/neldermead_worker.h
        tools/abstractmarkeritem.cpp tools/abstractmarkeritem.h
        tools/circleitem.cpp tools/circleitem.h
        tools/colortextitem.cpp tools/colortextitem.h
        tools/combolineedit.cpp tools/combolineedit.h
        tools/cropmarker.cpp tools/cropmarker.h
        tools/debug.h
        tools/diagramgv.cpp tools/diagramgv.h
        tools/histogramitem.cpp tools/histogramitem.h
        tools/indexparser.cpp tools/indexparser.h
        tools/init3D.h
        tools/itemstore.cpp tools/itemstore.h
        tools/mat3D.cpp tools/mat3D.h
        tools/mousepositioninfo.h
        tools/numberedit.cpp tools/numberedit.h
        tools/objectstore.cpp tools/objectstore.h
        tools/optimalrotation.cpp tools/optimalrotation.h
        tools/propagatinggraphicsobject.cpp tools/propagatinggraphicsobject.h
        tools/resizeingtablewidget.cpp tools/resizeingtablewidget.h
        tools/ruleritem.cpp tools/ruleritem.h
        tools/rulermodel.cpp tools/rulermodel.h
        tools/spotindicatorgraphicsitem.cpp tools/spotindicatorgraphicsitem.h
        tools/spotitem.cpp tools/spotitem.h
        tools/threadrunner.cpp tools/threadrunner.h
        tools/tools.cpp tools/tools.h
        tools/vec3D.cpp tools/vec3D.h
        tools/webkittextobject.cpp tools/webkittextobject.h
        tools/xmllistiterators.cpp tools/xmllistiterators.h
        tools/xmltools.cpp tools/xmltools.h
        tools/zipiterator.cpp tools/zipiterator.h
        tools/zoneitem.cpp tools/zoneitem.h
        ui/clip.cpp ui/clip.h ui/clip.ui
        ui/clipconfig.cpp ui/clipconfig.h ui/clipconfig.ui
        ui/contrastcurves.cpp ui/contrastcurves.h ui/contrastcurves.ui
        ui/crystaldisplay.cpp ui/crystaldisplay.h ui/crystaldisplay.ui
        ui/fitdisplay.cpp ui/fitdisplay.h ui/fitdisplay.ui
        ui/hkltool.cpp ui/hkltool.h ui/hkltool.ui
        ui/imagetoolbox.cpp ui/imagetoolbox.h ui/imagetoolbox.ui
        ui/indexdisplay.cpp ui/indexdisplay.h ui/indexdisplay.ui
        ui/laueplanecfg.cpp ui/laueplanecfg.h ui/laueplanecfg.ui
        ui/monoscalercfg.cpp ui/monoscalercfg.h ui/monoscalercfg.ui
        ui/mouseinfodisplay.cpp ui/mouseinfodisplay.h ui/mouseinfodisplay.ui
        ui/printdialog.cpp ui/printdialog.h ui/printdialog.ui
        ui/projectiongraphicsview.cpp ui/projectiongraphicsview.h
        ui/projectionplane.cpp ui/projectionplane.h ui/projectionplane.ui
        ui/reorient.cpp ui/reorient.h ui/reorient.ui
        ui/resolutioncalculator.cpp ui/resolutioncalculator.h ui/resolutioncalculator.ui
        ui/rotatecrystal.cpp ui/rotatecrystal.h ui/rotatecrystal.ui
        ui/sadeasteregg.cpp ui/sadeasteregg.h ui/sadeasteregg.ui
        ui/stereocfg.cpp ui/stereocfg.h ui/stereocfg.ui
        resources/resources.qrc
)
#target_include_directories(Clip PUBLIC
#    ../eigen
#    /usr/include/eigen3
#)

target_link_libraries(Clip PUBLIC
        Qt::Concurrent
        Qt::Core
        Qt::Gui
        Qt::OpenGL
        Qt::PrintSupport
        Qt::Svg
        Qt5::WebKitWidgets
        Qt::Widgets
        Qt::Xml
        TIFF::TIFF
        Eigen3::Eigen
)

## Resources:
#set(resources_resource_files
#        "resources/AboutMessage.txt"
#        "resources/DefaultCrystal.xml"
#        "resources/DefaultWorkspace.cws"
#        "resources/Default_LauePlaneProjector.xml"
#        "resources/Default_StereoProjector.xml"
#        "resources/curves/00-greyscale.curve"
#        "resources/curves/01-red-yellow.curve"
#        "resources/curves/02-red-magenta.curve"
#        "resources/curves/03-green-cyan.curve"
#        "resources/curves/04-green-yellow.curve"
#        "resources/curves/05-blue-cyan.curve"
#        "resources/curves/06-blue-magenta.curve"
#        "resources/curves/07-rainbow.curve"
#        "resources/icons/arrow-down.png"
#        "resources/icons/arrow-up.png"
#        "resources/icons/clip.ico"
#        "resources/icons/clip.png"
#        "resources/icons/color_curve.png"
#        "resources/icons/configure.png"
#        "resources/icons/crystal.png"
#        "resources/icons/crystaltable.png"
#        "resources/icons/cursor_hor.png"
#        "resources/icons/cursor_rot.png"
#        "resources/icons/cut.png"
#        "resources/icons/editcopy.png"
#        "resources/icons/editcut.png"
#        "resources/icons/editpaste.png"
#        "resources/icons/editredo.png"
#        "resources/icons/editundo.png"
#        "resources/icons/fileclose.png"
#        "resources/icons/fileopen.png"
#        "resources/icons/fileprint.png"
#        "resources/icons/fileprintpdf.png"
#        "resources/icons/fileprintpng.png"
#        "resources/icons/fileprintps.png"
#        "resources/icons/fileprintsvg.png"
#        "resources/icons/filequickprint.png"
#        "resources/icons/filesave.png"
#        "resources/icons/fit-page.png"
#        "resources/icons/fit-width.png"
#        "resources/icons/flag.png"
#        "resources/icons/flip_horizontal.png"
#        "resources/icons/flip_vertical.png"
#        "resources/icons/imagetable.png"
#        "resources/icons/index.png"
#        "resources/icons/info.png"
#        "resources/icons/layout-landscape.png"
#        "resources/icons/layout-portrait.png"
#        "resources/icons/lock.png"
#        "resources/icons/page-setup.png"
#        "resources/icons/pan.png"
#        "resources/icons/projector.png"
#        "resources/icons/projectortable.png"
#        "resources/icons/rotate_ccw.png"
#        "resources/icons/rotate_cw.png"
#        "resources/icons/ruler.png"
#        "resources/icons/textbold.png"
#        "resources/icons/textcenter.png"
#        "resources/icons/textitalic.png"
#        "resources/icons/textjustify.png"
#        "resources/icons/textleft.png"
#        "resources/icons/textright.png"
#        "resources/icons/textsubscript.png"
#        "resources/icons/textsuperscript.png"
#        "resources/icons/textunder.png"
#        "resources/icons/unlock.png"
#        "resources/icons/viewmag+.png"
#        "resources/icons/viewmag-.png"
#        "resources/icons/zone.png"
#        "resources/icons/zoom.png"
#        "resources/papa.jpeg"
#        "resources/report.css"
#        "resources/report_LauePlaneProjector.html"
#        "resources/report_crystal.html"
#        "resources/report_image.html"
#)
#


#set(resources1_resource_files
#    "resources/AboutMessage.txt"
#    "resources/DefaultCrystal.xml"
#    "resources/DefaultWorkspace.cws"
#    "resources/Default_LauePlaneProjector.xml"
#    "resources/Default_StereoProjector.xml"
#    "resources/curves/00-greyscale.curve"
#    "resources/curves/01-red-yellow.curve"
#    "resources/curves/02-red-magenta.curve"
#    "resources/curves/03-green-cyan.curve"
#    "resources/curves/04-green-yellow.curve"
#    "resources/curves/05-blue-cyan.curve"
#    "resources/curves/06-blue-magenta.curve"
#    "resources/curves/07-rainbow.curve"
#    "resources/icons/arrow-down.png"
#    "resources/icons/arrow-up.png"
#    "resources/icons/clip.ico"
#    "resources/icons/clip.png"
#    "resources/icons/color_curve.png"
#    "resources/icons/configure.png"
#    "resources/icons/crystal.png"
#    "resources/icons/crystaltable.png"
#    "resources/icons/cursor_hor.png"
#    "resources/icons/cursor_rot.png"
#    "resources/icons/cut.png"
#    "resources/icons/editcopy.png"
#    "resources/icons/editcut.png"
#    "resources/icons/editpaste.png"
#    "resources/icons/editredo.png"
#    "resources/icons/editundo.png"
#    "resources/icons/fileclose.png"
#    "resources/icons/fileopen.png"
#    "resources/icons/fileprint.png"
#    "resources/icons/fileprintpdf.png"
#    "resources/icons/fileprintpng.png"
#    "resources/icons/fileprintps.png"
#    "resources/icons/fileprintsvg.png"
#    "resources/icons/filequickprint.png"
#    "resources/icons/filesave.png"
#    "resources/icons/fit-page.png"
#    "resources/icons/fit-width.png"
#    "resources/icons/flag.png"
#    "resources/icons/flip_horizontal.png"
#    "resources/icons/flip_vertical.png"
#    "resources/icons/imagetable.png"
#    "resources/icons/index.png"
#    "resources/icons/info.png"
#    "resources/icons/layout-landscape.png"
#    "resources/icons/layout-portrait.png"
#    "resources/icons/lock.png"
#    "resources/icons/page-setup.png"
#    "resources/icons/pan.png"
#    "resources/icons/projector.png"
#    "resources/icons/projectortable.png"
#    "resources/icons/rotate_ccw.png"
#    "resources/icons/rotate_cw.png"
#    "resources/icons/ruler.png"
#    "resources/icons/textbold.png"
#    "resources/icons/textcenter.png"
#    "resources/icons/textitalic.png"
#    "resources/icons/textjustify.png"
#    "resources/icons/textleft.png"
#    "resources/icons/textright.png"
#    "resources/icons/textsubscript.png"
#    "resources/icons/textsuperscript.png"
#    "resources/icons/textunder.png"
#    "resources/icons/unlock.png"
#    "resources/icons/viewmag+.png"
#    "resources/icons/viewmag-.png"
#    "resources/icons/zone.png"
#    "resources/icons/zoom.png"
#    "resources/papa.jpeg"
#    "resources/report.css"
#    "resources/report_LauePlaneProjector.html"
#    "resources/report_crystal.html"
#    "resources/report_image.html"
#)
#
#qt6_add_resources(Clip "resources1"
#    PREFIX
#        "/icons"
#    BASE
#        "resources"
#    FILES
#        ${resources1_resource_files}
#)
#set(resources2_resource_files
#    "resources/AboutMessage.txt"
#    "resources/DefaultCrystal.xml"
#    "resources/DefaultWorkspace.cws"
#    "resources/Default_LauePlaneProjector.xml"
#    "resources/Default_StereoProjector.xml"
#    "resources/curves/00-greyscale.curve"
#    "resources/curves/01-red-yellow.curve"
#    "resources/curves/02-red-magenta.curve"
#    "resources/curves/03-green-cyan.curve"
#    "resources/curves/04-green-yellow.curve"
#    "resources/curves/05-blue-cyan.curve"
#    "resources/curves/06-blue-magenta.curve"
#    "resources/curves/07-rainbow.curve"
#    "resources/icons/arrow-down.png"
#    "resources/icons/arrow-up.png"
#    "resources/icons/clip.ico"
#    "resources/icons/clip.png"
#    "resources/icons/color_curve.png"
#    "resources/icons/configure.png"
#    "resources/icons/crystal.png"
#    "resources/icons/crystaltable.png"
#    "resources/icons/cursor_hor.png"
#    "resources/icons/cursor_rot.png"
#    "resources/icons/cut.png"
#    "resources/icons/editcopy.png"
#    "resources/icons/editcut.png"
#    "resources/icons/editpaste.png"
#    "resources/icons/editredo.png"
#    "resources/icons/editundo.png"
#    "resources/icons/fileclose.png"
#    "resources/icons/fileopen.png"
#    "resources/icons/fileprint.png"
#    "resources/icons/fileprintpdf.png"
#    "resources/icons/fileprintpng.png"
#    "resources/icons/fileprintps.png"
#    "resources/icons/fileprintsvg.png"
#    "resources/icons/filequickprint.png"
#    "resources/icons/filesave.png"
#    "resources/icons/fit-page.png"
#    "resources/icons/fit-width.png"
#    "resources/icons/flag.png"
#    "resources/icons/flip_horizontal.png"
#    "resources/icons/flip_vertical.png"
#    "resources/icons/imagetable.png"
#    "resources/icons/index.png"
#    "resources/icons/info.png"
#    "resources/icons/layout-landscape.png"
#    "resources/icons/layout-portrait.png"
#    "resources/icons/lock.png"
#    "resources/icons/page-setup.png"
#    "resources/icons/pan.png"
#    "resources/icons/projector.png"
#    "resources/icons/projectortable.png"
#    "resources/icons/rotate_ccw.png"
#    "resources/icons/rotate_cw.png"
#    "resources/icons/ruler.png"
#    "resources/icons/textbold.png"
#    "resources/icons/textcenter.png"
#    "resources/icons/textitalic.png"
#    "resources/icons/textjustify.png"
#    "resources/icons/textleft.png"
#    "resources/icons/textright.png"
#    "resources/icons/textsubscript.png"
#    "resources/icons/textsuperscript.png"
#    "resources/icons/textunder.png"
#    "resources/icons/unlock.png"
#    "resources/icons/viewmag+.png"
#    "resources/icons/viewmag-.png"
#    "resources/icons/zone.png"
#    "resources/icons/zoom.png"
#    "resources/papa.jpeg"
#    "resources/report.css"
#    "resources/report_LauePlaneProjector.html"
#    "resources/report_crystal.html"
#    "resources/report_image.html"
#)
#
#qt6_add_resources(Clip "resources2"
#    PREFIX
#        "/"
#    BASE
#        "resources"
#    FILES
#        ${resources2_resource_files}
#)

#if(WIN32 AND CONFIG(debug,debug OR release))
#    set_target_properties(Clip PROPERTIES
#        WIN32_EXECUTABLE FALSE
#    )
#endif()
#
#if(static)
#    target_compile_definitions(Clip PUBLIC
#        CLIP_STATIC
#    )
#endif()

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_compile_definitions(Clip PUBLIC
            CLIP_DEBUG
            CLIP_DEBUG_SOURCEDIR="\"\\\"${CMAKE_CURRENT_SOURCE_DIR}\\\"\""
    )
endif ()

if (NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif ()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}")

install(TARGETS Clip
        RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
        BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
        LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
