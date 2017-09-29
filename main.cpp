/* main.cpp
 *
 * Copyright (C) 2016 UTHealth MRI Research
 *
 * This file is part of the GRAPE Diagram Editor.
 *
 * GRAPE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GRAPE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GRAPE.  If not, see <http://www.gnu.org/licenses/>
 */

#include <MainWindow.h>
#include <iostream>
#include <stdlib.h>

#include "Pipeline.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setDoubleClickInterval(250);

//    QDir::addSearchPath("", QCoreApplication::applicationDirPath());
//    std::setenv("PATH",QCoreApplication::applicationDirPath().toStdString(),1);
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    // Command-line arguments
    bool inputFileOptionSet;
    QString filePath;

    bool graphicalOptionSet;
    bool isGraphical;

    bool runOptionSet;
    bool isRun;

    // Parsing inputs
    QCommandLineParser parser;
    QString GRAPE_help =
            QString("GRAPE 0.1, Graphical Pipeline Environment, UTHealth School of Medicine\n") +
            QString("THIS SOFTWARE IS PROVIDED 'AS IS'. IN NO EVENT SHALL UTHEALTH OR ANY OF THEIR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES ARISING BECUASE OF THE USE OF THIS SOFTWARE.");
    parser.setApplicationDescription(GRAPE_help);
    parser.addHelpOption();
    parser.addVersionOption();
    //parser.addPositionalArgument("input", QCoreApplication::translate("main", "pipeline file."));


    // An option with a value
    QCommandLineOption inputFileOption(QStringList() << "i" << "input-file",
                                       QCoreApplication::translate("main", "input file."),
                                       QCoreApplication::translate("main", "input"));
    parser.addOption(inputFileOption);


    QCommandLineOption isNotGraphicalOption(QStringList() << "s" << "suppress-gui",
                                       QCoreApplication::translate("main", "suppress the GUI."));
//                                        QCoreApplication::translate("main", "yes/no"));
    parser.addOption(isNotGraphicalOption);

    QCommandLineOption runOption(QStringList() << "r" << "run-pipeline",
                                       QCoreApplication::translate("main", "run the pipeline after loading."));
//                               QCoreApplication::translate("main", "yes/no"));
    parser.addOption(runOption);

    // Process the actual command line arguments given by the user
    parser.process(app);

    //bool showProgress = parser.isSet(showProgressOption);
    inputFileOptionSet = parser.isSet(inputFileOption);
    if(inputFileOptionSet)
        filePath = parser.value(inputFileOption);
    else
        filePath = "";


    isGraphical = !parser.isSet(isNotGraphicalOption);
    isRun = parser.isSet(runOption);

    // End Parsing inputs

    // to be
//    if(0)
//    {
//        Pipeline mainPipeline;
//        Diagram mDiagram;

//        mainPipeline.setDiagram(mDiagram);

//        if(isGraphical)
//        {
//            MainWindow window;
//            window.setDiagram(mDiagram);
//            window.setPipeline(mainPipeline);
//        }
//    }


    // Create main window and run

    MainWindow window(filePath, isRun, isGraphical);

    emit window.diagram()->mainPipeline.pipelineMessage("GRAPE started...");

    //std::cout << "GRAPE started..." << std::endl; ;

//    GrapePipeline GP(filePath, isRun, isGraphical);
    if(isGraphical)
    {
//        MainWindow window(filePath, isRun, isGraphical);
//        GrapeGraphicsInterface GGI(window);
        window.show();
    }


    return app.exec();

}
//--------------------------------------------------------------------------------------------------
