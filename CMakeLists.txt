cmake_minimum_required(VERSION 3.18)
project(Clip VERSION 4.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(Qt5 COMPONENTS Core Concurrent Gui OpenGL PrintSupport Svg Widgets Xml REQUIRED)

# QT CMake things
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Add dependencies
find_package(Eigen3)
if (${Eigen3_FOUND})
    message(STATUS "Eigen3 found on system")
else ()
    message(STATUS "Eigen3 not found on system, fetching...")
    include(${PROJECT_SOURCE_DIR}/cmake/Eigen3.cmake)
endif ()

find_package(TIFF)
if (${TIFF_FOUND})
    message(STATUS "libTIFF found on system")
    set(TIFF_COMPONENT "TIFF")
else ()
    message(STATUS "libtiff not found on system, fetching...")
    include(${PROJECT_SOURCE_DIR}/cmake/TIFF.cmake)
    set(TIFF_COMPONENT "tiff")
endif ()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

add_executable(Clip WIN32
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
        #        tools/webkittextobject.cpp tools/webkittextobject.h
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
        #        ui/printdialog.cpp ui/printdialog.h ui/printdialog.ui
        ui/projectiongraphicsview.cpp ui/projectiongraphicsview.h
        ui/projectionplane.cpp ui/projectionplane.h ui/projectionplane.ui
        ui/reorient.cpp ui/reorient.h ui/reorient.ui
        ui/resolutioncalculator.cpp ui/resolutioncalculator.h ui/resolutioncalculator.ui
        ui/rotatecrystal.cpp ui/rotatecrystal.h ui/rotatecrystal.ui
        ui/sadeasteregg.cpp ui/sadeasteregg.h ui/sadeasteregg.ui
        ui/stereocfg.cpp ui/stereocfg.h ui/stereocfg.ui
        resources/resources.qrc
        resources/clip.rc
)

target_link_libraries(Clip PUBLIC
        Qt5::Concurrent
        Qt5::Core
        Qt5::Gui
        Qt5::OpenGL
        Qt5::PrintSupport
        Qt5::Svg
        Qt5::Widgets
        Qt5::Xml
        Eigen3::Eigen
        TIFF::${TIFF_COMPONENT}
)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_compile_definitions(Clip PUBLIC
            CLIP_DEBUG
            #            CLIP_DEBUG_SOURCEDIR=${CMAKE_CURRENT_SOURCE_DIR}
    )
endif ()

if (NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "Clip/bin")
endif ()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}")

install(TARGETS Clip
        RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
        BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
        LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
