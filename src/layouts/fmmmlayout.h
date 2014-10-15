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

#ifndef FMMMLAYOUT_H
#define FMMMLAYOUT_H

#include <QObject>
#include <QHash>
#include "baselayout.h"
#include <ogdf/energybased/FMMMLayout.h>

class FMMMLayout : public BaseLayout
{
    Q_OBJECT
public:
    Q_INVOKABLE FMMMLayout(QObject *parent = 0);

    bool layout(ogdf::GraphAttributes * GA);
    void readSettings(void);
    void writeSettings(void);

signals:

public slots:

private:
    // Base parameters
    bool _useHighLevelOptions;
    double_t _unitEdgeLength;
    bool _newInitialPlacement;
    QHash<QString, ogdf::FMMMLayout::QualityVsSpeed> _qualityVsSpeedHash;
    ogdf::FMMMLayout::QualityVsSpeed _qualityVsSpeed;
    // Advanced parameters / General
    int _randSeed;
    ogdf::FMMMLayout::EdgeLengthMeasurement _edgeLengthMeasurement;
    ogdf::FMMMLayout::AllowedPositions _allowedPositions;
    int _maxIntPosExponent;
    // Advanced parameters / Divide et impera step
    double _pageRatio;
    int _stepsForRotatingComponents;
    ogdf::FMMMLayout::TipOver _tipOverCCs;
    double _minDistCC;
    ogdf::FMMMLayout::PreSort _presortCCs;
    // Advanced parameters / Multilevel step
    int _minGraphSize;
    ogdf::FMMMLayout::GalaxyChoice _galaxyChoice;
    int _randomTries;
    ogdf::FMMMLayout::MaxIterChange _maxIterChange;
    int _maxIterFactor;
    ogdf::FMMMLayout::InitialPlacementMult _initialPlacementMult;
    // Advanced parameters / Force calculation step
    ogdf::FMMMLayout::ForceModel _forceModel;
    double _springStrength;
    double _repForcesStrength;
    ogdf::FMMMLayout::RepulsiveForcesMethod _repulsiveForcesCalculation;
    ogdf::FMMMLayout::StopCriterion _stopCriterion;
    double _threshold;
    int _fixedIterations;
    double _forceScalingFactor;
    bool _coolTemperature;
    double _coolValue;
    ogdf::FMMMLayout::InitialPlacementForces _initialPlacementForces;
    // Advanced parameters / Force calculation step
    bool _resizeDrawing;
    double _resizingScalar;
    int _fineTuningIterations;
    double _fineTuneScalar;
    bool _adjustPostRepStrengthDynamically;
    double _postSpringStrength;
    double _postStrengthOfRepForces;
    // Advanced parameters / Repulsive force approximation methods
    int _frGridQuotient;
    ogdf::FMMMLayout::ReducedTreeConstruction _nmTreeConstruction;
    ogdf::FMMMLayout::SmallestCellFinding _nmSmallCell;
    int _nmParticlesInLeaves;
    int _nmPrecision;

};

#endif // FMMMLAYOUT_H
