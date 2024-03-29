// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "mainwidget.h"

#include <QMouseEvent>

#include <cmath>
#include <iostream>

MainWidget::MainWidget() : QOpenGLWidget(), sensitivity(1.0), translation(-0., -230., -1500.), _animating(true), _looping(true)//, timer(this)
{
}
MainWidget::~MainWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete texture;
    delete geometries;
    doneCurrent();
}

//! [0]
void MainWidget::keyPressEvent(QKeyEvent* e)
{
	int key = e->key();
	switch (key)
	{
	case Qt::Key_Space:
		_animating = !_animating;
		if ((_animating)&(verbose))
		std::cout << "Space key pressed : animation play" << std::endl;
		else if (verbose)
		std::cout << "Space key pressed : animation paused" << std::endl;
		break;
        case Qt::Key_L:
		_looping = !_looping;
		if ((_looping)&(verbose))
		std::cout << "L key pressed : loop mode activated" << std::endl;
		else if(verbose)
		std::cout << "L key pressed : loop mode disabled" << std::endl;
		break;
	default:
		break;
	}
}

void MainWidget::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    lastMousePosition = QVector2D(e->pos());
    mouseButton = e->button(); // pour faire un mode translation
}

void MainWidget::mouseMoveEvent(QMouseEvent *e)
{
    // Get mouse position
    mousePosition = QVector2D(e->pos());
    QVector2D diff = mousePosition-lastMousePosition;

    // Get Modifiers
    shift_modifier = false;
    if (e->modifiers() & Qt::ShiftModifier) shift_modifier = true;

    // Apply transformations
    if ((mouseButton == Qt::LeftButton)&(!shift_modifier)) {
        // Rotation axis is perpendicular to the mouse position difference
	QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized() * sensitivity;
	rotationAxis = n;
	
	// Update rotation
	rotation = QQuaternion::fromAxisAndAngle(rotationAxis, diff.length()) * rotation;
    }
    if ((mouseButton == Qt::RightButton) or ((mouseButton == Qt::LeftButton)&(shift_modifier))) {
	// faire la translation
	diff = 0.0017*diff*translation.length();
	QVector3D move(diff.x(), -diff.y(), 0);
	translation += move;
	// cf. SI code
	
    }
    //update();
    lastMousePosition = mousePosition;

}

void MainWidget::wheelEvent(QWheelEvent *ev)
{
    QPoint zoom = ev->angleDelta();
    QVector3D move(0., 0., -0.0001*zoom.y()*translation.length());
    translation += move;
}

void MainWidget::mouseReleaseEvent(QMouseEvent *e)
{
    // A enlever
    if (verbose) std::cout << "Mouse released" << std::endl;
}
//! [0]
//! [1]
void MainWidget::timerEvent(QTimerEvent *)
{
    // Drag force    
    angularSpeed *= 0.1;
    //std::cout << timer.interval() << std::endl;
    update();
}
//! [1]

void MainWidget::initializeGL() 
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    initShaders();
    initTextures();

//! [2]
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);
//! [2]

    geometries = new GeometryEngine(&_looping, &_animating);
    delta_time = geometries->get_delta_time();
    std::cout <<"dt : "<<delta_time<<std::endl;

    //// Use QBasicTimer because its faster than QTimer
    timer.start(delta_time*1000, this);
}

//! [3]
void MainWidget::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!program.link())
        close();

    // Bind shader pipeline for use
    if (!program.bind())
        close();
}
//! [3]

//! [4]
void MainWidget::initTextures()
{
    // Load cube.png image
    texture = new QOpenGLTexture(QImage(":/grid.png").mirrored());

    // Set nearest filtering mode for texture minification
    texture->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    texture->setWrapMode(QOpenGLTexture::Repeat);
}
//! [4]

//! [5]
void MainWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 1.0, zFar = 15000.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}
//! [5]

void MainWidget::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    texture->bind();

//! [6]
    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.translate(translation); //0.0, 0.0, -5.0);
    matrix.rotate(rotation);

    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", projection * matrix);
//! [6]

    // Use texture unit 0 which contains cube.png
    program.setUniformValue("texture", 0);

    // Draw cube geometry
	if (verbose) std::cout << "Draw plan ..." << std::endl;
	geometries->drawPlanGeometry(&program);
	if (verbose) std::cout << "Draw Skeleton ..." << std::endl;
    geometries->drawSkeletonGeometry(&program); 
	if (verbose) std::cout << "Draw Model ..." << std::endl;
    geometries->drawModelGeometry(&program); 
    if (verbose) std::cout << translation.x() << ", " <<  translation.y() << ", " <<  translation.z() << ", " << std::endl;
    close();
    
}
