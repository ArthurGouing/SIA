// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "geometryengine.h"
#include "joint.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

class GeometryEngine;

class MainWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    //using QOpenGLWidget::QOpenGLWidget;
    MainWidget();
    ~MainWidget();

protected:
    void keyPressEvent(QKeyEvent *e)       override;
    void mousePressEvent(QMouseEvent *e)   override;
    void mouseMoveEvent(QMouseEvent *e)    override;
    void wheelEvent(QWheelEvent *ev)       override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void timerEvent(QTimerEvent *e)        override;

    void initializeGL()         override;
    void resizeGL(int w, int h) override;
    void paintGL()              override;

    void initShaders();
    void initTextures();

private:
    QBasicTimer timer;
    QOpenGLShaderProgram program;
    GeometryEngine *geometries = nullptr;


    QOpenGLTexture *texture = nullptr;

    QMatrix4x4 projection;
    
    float sensitivity;
    bool shift_modifier;
    bool _animating;
    bool _looping;
    Qt::MouseButton mouseButton;
    QVector2D lastMousePosition;
    QVector2D mousePosition;
    QPoint zoom;
    QVector3D rotationAxis;
    qreal angularSpeed = 0;
    QQuaternion rotation;
    QVector3D translation;
};

#endif // MAINWIDGET_H
