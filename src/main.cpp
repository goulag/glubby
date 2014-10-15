/*
Copyright (c) 2014, Goulag Parkinson
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
* Neither the name of the <organization> nor the
names of its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ESTEBAN PELLEGRINO BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>

#include "glubby.h"
#include "baselayout.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setOrganizationName("netglub");
    QCoreApplication::setOrganizationDomain("netglub.org");
    QCoreApplication::setApplicationName("glubby");
    QCoreApplication::setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("Very simple Qt/C++ layout engine based on OGDF");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("layout_name", QCoreApplication::translate("main", "layout name supported by glubby in ('fmmm','sugiyama','circular'), [DEFAULT=fmmm]."));

    QCommandLineOption inputOption(QStringList() << "i" << "input",
               QCoreApplication::translate("main", "gml file to read [DEFAULT=/run/shm/input.gml]."),
               QCoreApplication::translate("main", "gml"));
    parser.addOption(inputOption);

    QCommandLineOption outputOption(QStringList() << "o" << "output",
               QCoreApplication::translate("main", "file to write [DEFAULT=/run/shm/output.json]."),
               QCoreApplication::translate("main", "output"));
    parser.addOption(outputOption);

    QCommandLineOption outputFormatOption(QStringList() << "f" << "format",
               QCoreApplication::translate("main", "output format [DEFAULT=json]."),
               QCoreApplication::translate("main", "format"));
    parser.addOption(outputFormatOption);

    QCommandLineOption layoutOption(QStringList() << "l" << "layout",
               QCoreApplication::translate("main", "layout name [DEFAULT=fmmm]."),
               QCoreApplication::translate("main", "layout"));
    parser.addOption(layoutOption);

    QCommandLineOption svgOption(QStringList() << "s" << "svg",
               QCoreApplication::translate("main", "svg file to write."),
               QCoreApplication::translate("main", "svg"));
    parser.addOption(svgOption);

    QCommandLineOption gmlOption(QStringList() << "g" << "gml",
               QCoreApplication::translate("main", "gml file to write."),
               QCoreApplication::translate("main", "gml"));
    parser.addOption(gmlOption);

    // Process the actual command line arguments given by the user
    parser.process(app);

    const QStringList args = parser.positionalArguments();

    QString layoutName = "fmmm";
    if (args.size() != 1) {
        if (!parser.isSet(layoutOption)) {
            qCritical("WARNING: %s", qPrintable(QCoreApplication::translate("main", "No layout name specified ! DEFAULT=fmmm.")));
        } else {
            layoutName = parser.value(layoutOption);
        }
    } else layoutName = args.at(0);

    if (!BaseLayout::getLayoutHash()->contains(layoutName)) {
        qCritical("FATAL: No such layout name '%s' allowed in glubby !",qPrintable(layoutName));
        parser.showHelp(1);
    }

    QString inputGml = "/run/shm/input.gml";
    QString outputFilename = "/run/shm/output.json";
    QString outputFormat = "json";
    QList<QString> formatList;
    formatList << "json";

    bool input = parser.isSet(inputOption);
    if (input) inputGml = parser.value(inputOption);
    bool output = parser.isSet(outputOption);
    if (output) outputFilename = parser.value(outputOption);
    bool format = parser.isSet(outputFormatOption);
    if (format) {
        if (formatList.contains(parser.value(outputFormatOption)))
            outputFormat = parser.value(outputFormatOption);
        else qCritical("ERROR: no such output format allowed \"%s\", default json apply",
                       qPrintable(parser.value(outputFormatOption)));
    }
    QString outputSvgFilename;
    bool svg = parser.isSet(svgOption);
    if (svg) outputSvgFilename = parser.value(svgOption);
    QString outputGmlFilename;
    bool gml = parser.isSet(gmlOption);
    if (gml) outputGmlFilename = parser.value(gmlOption);

    Glubby g;

    g.readSettings();

    g.layout(layoutName, inputGml, outputFilename, outputFormat, outputSvgFilename, outputGmlFilename);

    g.writeSettings();

    //g.exit();
}
