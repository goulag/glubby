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

#include "glubby.h"
#include "baselayout.h"
#include <ogdf/fileformats/GmlParser.h>
#include <ogdf/fileformats/GraphIO.h>

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

Glubby::Glubby(QObject *parent) :
    QObject(parent)
{
    _G = new ogdf::Graph();
    _GA = new ogdf::GraphAttributes(*_G,
         ogdf::GraphAttributes::nodeGraphics     |
         ogdf::GraphAttributes::edgeGraphics     |
         ogdf::GraphAttributes::edgeIntWeight    |
         ogdf::GraphAttributes::edgeDoubleWeight |
         ogdf::GraphAttributes::edgeLabel        |
         ogdf::GraphAttributes::nodeLabel        |
         ogdf::GraphAttributes::edgeType         |
         ogdf::GraphAttributes::nodeType         |
         ogdf::GraphAttributes::nodeId           |
         ogdf::GraphAttributes::edgeArrow        |
         ogdf::GraphAttributes::edgeStyle        |
         ogdf::GraphAttributes::nodeStyle        |
         ogdf::GraphAttributes::nodeTemplate     |
         ogdf::GraphAttributes::edgeSubGraphs    |
         ogdf::GraphAttributes::nodeWeight       |
         ogdf::GraphAttributes::threeD
     );
    _nodeWidth = 25.0;
    _nodeHeight = 25.0;
}

Glubby::~Glubby(void)
{
    qDebug() << "Glubby::~Glubby(void)";
}

bool Glubby::layout(const QString & layoutName,
            const QString & inputGml,
            const QString & outputFilename,
            const QString & outputFormat,
            const QString & outputSvgFilename,
            const QString & outputGmlFilename)
{
    qDebug() << qPrintable(QString("Glubby::layout(%1,%2,%3,%4,%5,%6)")
                .arg(layoutName)
                .arg(inputGml)
                .arg(outputFilename)
                .arg(outputFormat)
                .arg(outputSvgFilename)
                .arg(outputGmlFilename));

    bool doCheck = true;
    ogdf::GmlParser parser = ogdf::GmlParser(inputGml.toStdString().c_str(), doCheck);
    if (parser.error()) {
        qCritical() << "CRITICAL: Error in parsing gml file:" << inputGml << ". Error is: " << parser.errorString().c_str();
        return false;
    }
    if (!parser.read(*_G, *_GA)) {
        qCritical() << "CRITICAL: Error in reading Graph and GraphAttribute in gml file:" << inputGml << ". Error is: " << parser.errorString().c_str();
        return false;

    }
    ogdf::node n;
    forall_nodes( n, *_G ){ // iterate through all the node in the graph
        /*
        double x = _GA->x(n);
        double y = _GA->y(n);
        double w = _GA->width(n);
        double h = _GA->height(n);
        */
        if (_GA->width(n)==0) _GA->width(n) = _nodeWidth;
        if (_GA->height(n)==0) _GA->height(n) = _nodeHeight;
    }

    if (!BaseLayout::getLayoutHash()->contains(layoutName)) {
        qCritical("FATAL: No such layout name '%s' allowed in glubby !",qPrintable(layoutName));
        return false;
    }

    QMetaObject meta = BaseLayout::getLayoutHash()->value(layoutName);
    QObject * o = meta.newInstance();

    BaseLayout * layout = qobject_cast<BaseLayout*>(o);
    layout->readSettings();

    if (!layout->layout(_GA)) {
        qCritical("FATAL: Unable to compute \"layout\" for such gml.");
        return false;
    }

    layout->writeSettings();

    QJsonObject jsonGraph;

    QJsonObject jsonNodes;
    forall_nodes(n, *_G ) {
        QJsonObject json;
        json["x"] = _GA->x(n);
        json["y"] = _GA->y(n);
        jsonNodes[_GA->label(n).c_str()] = json;
    }    
    jsonGraph["nodes"] = jsonNodes;

    QJsonObject jsonBbox;
    ogdf::DRect boundingBox = _GA->boundingBox();
    ogdf::DPoint p1 = boundingBox.p1();
    ogdf::DPoint p2 = boundingBox.p2();
    jsonBbox["x1"] = p1.m_x;
    jsonBbox["y1"] = p1.m_y;
    jsonBbox["x2"] = p2.m_x;
    jsonBbox["y2"] = p2.m_y;
    jsonGraph["bbox"] = jsonBbox;

    QJsonDocument jsonDoc(jsonGraph);

    QFile jsonFile(outputFilename);
    if (!jsonFile.open(QIODevice::WriteOnly)) {
        qCritical("FATAL: Couldn't open save file '%s'.", qPrintable(outputFilename));
        return false;
    }
    jsonFile.write(jsonDoc.toJson());

    if (!outputGmlFilename.isEmpty())
        ogdf::GraphIO::writeGML( *_GA, outputGmlFilename.toStdString().c_str());
    if (!outputSvgFilename.isEmpty())
        ogdf::GraphIO::drawSVG( *_GA, outputSvgFilename.toStdString().c_str());

    return true;
}

void Glubby::readSettings(void)
{
    QSettings settings;
    settings.beginGroup("General");
    _nodeWidth = settings.value("node/width", _nodeWidth).toDouble();
    _nodeHeight = settings.value("node/height", _nodeHeight).toDouble();
    qDebug() << _nodeWidth << _nodeHeight;
    settings.endGroup();
}

void Glubby::writeSettings(void)
{
    QSettings settings;
    settings.beginGroup("General");
    settings.setValue("node/width", _nodeWidth);
    settings.setValue("node/height", _nodeHeight);
    settings.endGroup();
}

void Glubby::exit(void)
{

}
