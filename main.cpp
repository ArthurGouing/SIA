// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause


#include <QApplication>
#include <QLabel>
#include <QSurfaceFormat>

#ifndef QT_NO_OPENGL
#include "mainwidget.h"
#include "bvh_writer.h"
#endif

extern bool verbose = false;
extern std::string bvh_name ="ressources/xsens.bvh";

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	bool xsens = true;
    // parse argument
    if (app.arguments().size()>=2)
    {
        std::string arg = app.arguments().at(1).toStdString();
        if ((arg == "-v")or(arg == "--verbose"))
        {
            verbose = true;
			std::cout << "on set le verbose "<< std::endl;
			if (app.arguments().size()>2)
			{
				arg = app.arguments().at(2).toStdString();
				std::cout << arg << std::endl;
				bvh_name = arg;
				xsens = false;
			}
			std::cout << "on lenkfalenkf"<< std::endl;

        }
		else {
			bvh_name = arg;
			xsens = false;
		}
    }
	if (xsens)
	{
		std::vector<std::string> files;
		files.push_back("mouhcine-sia-xsens-data/MT_012005BA-000-000_00B47AB0.txt");
		files.push_back("mouhcine-sia-xsens-data/MT_012005BA-000-000_00B47ACA.txt");
		files.push_back("mouhcine-sia-xsens-data/MT_012005BA-000-000_00B47F0C.txt");
		files.push_back("mouhcine-sia-xsens-data/MT_012005BA-000-000_00B48506.txt");
		write_bvh( files, "ressources/xsens.bvh");
	}

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
