// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef GEOMETRYENGINE_H
#define GEOMETRYENGINE_H

#include "joint.h"

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class GeometryEngine : protected QOpenGLFunctions
{
public:
    GeometryEngine();
    virtual ~GeometryEngine();

    void drawSkeletonGeometry(QOpenGLShaderProgram *program);

private:
    void UpdateSkeletonGeometry(int iframe);

    Joint* root;
    int frame;
    bool is_loop;

    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
};

#endif // GEOMETRYENGINE_H
