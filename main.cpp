// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause


#include <QApplication>
#include <QLabel>
#include <QSurfaceFormat>

#ifndef QT_NO_OPENGL
#include "mainwidget.h"
#endif

extern bool verbose = false;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // parse argument
    if (app.arguments().size()>=2)
    {
        std::string arg = app.arguments().at(1).toStdString();
        if ((arg == "-v")or(arg == "--verbose"))
        {
            std::cout << "mode verbose" << std::endl;
            verbose = true;
        }
    }
    else std::cout << "not verbose mode " << verbose << std::endl;
    //

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    app.setApplicationName("cube");
    app.setApplicationVersion("0.1");
#ifndef QT_NO_OPENGL
    MainWidget widget;
    widget.show();
#else
    QLabel note("OpenGL Support required");
    note.show();
#endif
    return app.exec();
}
