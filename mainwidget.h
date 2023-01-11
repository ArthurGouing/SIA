// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "geometryengine.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QBasicTimer>

extern bool verbose;

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
    QOpenGLShaderProgram program;
    GeometryEngine *geometries = nullptr;
    QBasicTimer timer;
    float delta_time;

    QOpenGLTexture *texture = nullptr;

    float sensitivity;
    QMatrix4x4 projection;
    qreal angularSpeed = 0;
    QVector3D rotationAxis;
    QQuaternion rotation;
    QVector3D translation;
    
    Qt::MouseButton mouseButton;
    bool shift_modifier;

    bool _animating;
    bool _looping;
    QVector2D lastMousePosition;
    QVector2D mousePosition;
    QPoint zoom;
};

#endif // MAINWIDGET_H
