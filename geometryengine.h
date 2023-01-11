// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef GEOMETRYENGINE_H
#define GEOMETRYENGINE_H

#include "joint.h"

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

extern bool verbose;

class GeometryEngine : protected QOpenGLFunctions
{
public:
    GeometryEngine(bool* looping, bool* animating);
    virtual ~GeometryEngine();

    float get_delta_time(){return root->frame_time;}
    void drawSkeletonGeometry(QOpenGLShaderProgram *program);

private:
    void UpdateSkeletonGeometry(int iframe);

    Joint* root;
    int frame;
    bool* is_loop;
    bool* is_animated;

    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
};

#endif // GEOMETRYENGINE_H
