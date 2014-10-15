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

#include "fmmmlayout.h"
#include <QDebug>
#include <QSettings>

static RegisterLayout<FMMMLayout> registerMe("fmmm");

FMMMLayout::FMMMLayout(QObject *parent) :
    BaseLayout(parent)
{
    qDebug() << "FMMMLayout::FMMMLayout()";
    _useHighLevelOptions = false;
    _unitEdgeLength = 100.0;
    _newInitialPlacement = false;
    _qualityVsSpeedHash["GorgeousAndEfficient"] = ogdf::FMMMLayout::qvsGorgeousAndEfficient;
    _qualityVsSpeedHash["BeautifulAndFast"] = ogdf::FMMMLayout::qvsBeautifulAndFast;
    _qualityVsSpeedHash["NiceAndIncredibleSpeed"] = ogdf::FMMMLayout::qvsNiceAndIncredibleSpeed;
    _qualityVsSpeed = _qualityVsSpeedHash["BeautifulAndFast"];
}

bool FMMMLayout::layout(ogdf::GraphAttributes * GA)
{
    ogdf::FMMMLayout fmmm;

    fmmm.useHighLevelOptions(true);
    fmmm.unitEdgeLength(50.0);
    fmmm.newInitialPlacement(true);
    fmmm.qualityVersusSpeed(ogdf::FMMMLayout::qvsGorgeousAndEfficient);

    fmmm.call(*GA);

    return true;
}

void FMMMLayout::readSettings(void)
{
    QSettings settings;
    settings.beginGroup(metaObject()->className());
    _useHighLevelOptions = settings.value("useHighLevelOptions", _useHighLevelOptions).toBool();
    _unitEdgeLength = settings.value("_unitEdgeLength", _unitEdgeLength).toDouble();
    _newInitialPlacement = settings.value("newInitialPlacement", _newInitialPlacement).toBool();
    QString qualityVsSpeedString = settings.value("qualityVsSpeed", _qualityVsSpeedHash.key(_qualityVsSpeed)).toString();
    _qualityVsSpeed = _qualityVsSpeedHash[qualityVsSpeedString];
    settings.endGroup();
}

void FMMMLayout::writeSettings(void)
{
    QSettings settings;
    settings.beginGroup(metaObject()->className());
    settings.setValue("useHighLevelOptions", _useHighLevelOptions);
    settings.setValue("unitEdgeLength", _unitEdgeLength);
    settings.setValue("newInitialPlacement", _newInitialPlacement);
    settings.setValue("qualityVsSpeed", _qualityVsSpeedHash.key(_qualityVsSpeed));
    settings.endGroup();
}
