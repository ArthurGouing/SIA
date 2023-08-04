QT += core gui widgets openglwidgets

TARGET = run
TEMPLATE = app

SOURCES += main.cpp

SOURCES += \
    mainwidget.cpp \
    geometryengine.cpp \
    skin.cpp \
    joint.cpp \
    bvh_writer.cpp

HEADERS += \
    mainwidget.h \
    geometryengine.h \
    skin.h  \
    joint.h  \
    bvh_wirter.h 

RESOURCES += \
    shaders.qrc \
    textures.qrc

# Add Trimesh Library
HEADERS += \
trimesh/include/Box.h \
trimesh/include/Color.h \
trimesh/include/GLCamera.h \
trimesh/include/ICP.h \
trimesh/include/KDtree.h \
trimesh/include/TriMesh.h \
trimesh/include/TriMesh_algo.h \
trimesh/include/Vec.h \
trimesh/include/XForm.h \
trimesh/include/bsphere.h \
trimesh/include/lineqn.h \
#trimesh/include/mempool.h \
trimesh/include/noise3d.h \
trimesh/include/strutil.h \
trimesh/include/timestamp.h \

SOURCES += \
trimesh/libsrc/TriMesh_bounding.cc \
trimesh/libsrc/TriMesh_connectivity.cc \
trimesh/libsrc/TriMesh_curvature.cc \
trimesh/libsrc/TriMesh_grid.cc \
trimesh/libsrc/TriMesh_io.cc \
trimesh/libsrc/TriMesh_normals.cc \
trimesh/libsrc/TriMesh_pointareas.cc \
trimesh/libsrc/TriMesh_stats.cc \
trimesh/libsrc/TriMesh_tstrips.cc \
trimesh/libsrc/conn_comps.cc \
trimesh/libsrc/diffuse.cc \
trimesh/libsrc/edgeflip.cc \
trimesh/libsrc/faceflip.cc \
trimesh/libsrc/filter.cc \
#trimesh/libsrc/lmsmooth.cc \
trimesh/libsrc/overlap.cc \
trimesh/libsrc/remove.cc \
trimesh/libsrc/reorder_verts.cc \
#trimesh/libsrc/shared.cc \
#trimesh/libsrc/GLCamera.cc \
trimesh/libsrc/ICP.cc \
trimesh/libsrc/KDtree.cc \
trimesh/libsrc/subdiv.cc


# install
target.path = $$[QT_INSTALL_EXAMPLES]/opengl/cube
INSTALLS += target
