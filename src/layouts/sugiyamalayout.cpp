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

#include "sugiyamalayout.h"
#include <QDebug>
#include <QSettings>
#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>

static RegisterLayout<SugiyamaLayout> registerMe("sugiyama");

SugiyamaLayout::SugiyamaLayout(QObject *parent) :
    BaseLayout(parent)
{
    _runs = 15;
    _transpose = true;
    _fails = 4;
    _arrangeCCs = true;
    _minDistCC = 20.0;
    _alignBaseClasses = false;
    _alignSiblings = false;
}

bool SugiyamaLayout::layout(ogdf::GraphAttributes * GA)
{
    ogdf::SugiyamaLayout SL; //Compute a hierarchical drawing of G (using SugiyamaLayout)
    SL.setRanking( new ogdf::OptimalRanking );
    SL.setCrossMin( new ogdf::MedianHeuristic );

    ogdf::OptimalHierarchyLayout *ohl = new ogdf::OptimalHierarchyLayout;
    ohl->layerDistance(30.0);
    ohl->nodeDistance(25.0);
    ohl->weightBalancing(0.8);

    SL.setLayout( ohl );
    SL.call(*GA );

    return true;
}

void SugiyamaLayout::readSettings(void)
{
    QSettings settings;
    settings.beginGroup(metaObject()->className());
    _runs = settings.value("runs", _runs).toInt();
    _transpose = settings.value("transpose", _transpose).toBool();
    _fails = settings.value("fails", _fails).toInt();
    _arrangeCCs = settings.value("arrangeCCs", _arrangeCCs).toBool();
    _minDistCC = settings.value("minDistCC", _minDistCC).toDouble();
    _alignBaseClasses = settings.value("alignBaseClasses", _alignBaseClasses).toBool();
    _alignSiblings = settings.value("alignSiblings", _alignSiblings).toBool();
    settings.endGroup();
}

void SugiyamaLayout::writeSettings(void)
{
    QSettings settings;
    settings.beginGroup(metaObject()->className());
    settings.setValue("runs", _runs);
    settings.setValue("transpose", _transpose);
    settings.setValue("fails", _fails);
    settings.setValue("arrangeCCs", _arrangeCCs);
    settings.setValue("minDistCC", _minDistCC);
    settings.setValue("alignBaseClasses", _alignBaseClasses);
    settings.setValue("alignSiblings", _alignSiblings);
    settings.endGroup();
}
