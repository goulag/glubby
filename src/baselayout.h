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

#ifndef BASELAYOUT_H
#define BASELAYOUT_H

#include <QObject>
#include <ogdf/basic/GraphAttributes.h>

typedef QHash<QString, QMetaObject> LayoutHash;

class BaseLayout : public QObject
{
    Q_OBJECT
public:
    BaseLayout(QObject *parent = 0);
    virtual ~BaseLayout() {}

    static void addLayout(const QString & name, QMetaObject meta);
    static LayoutHash * getLayoutHash(void);
    template<typename T> static void registerLayout(const QString & name) {
        QMetaObject meta = T::staticMetaObject;
        addLayout(name, meta);
    }

    virtual bool layout(ogdf::GraphAttributes * /* GA */) { return false;}
    virtual void readSettings(void) {}
    virtual void writeSettings(void) {}

signals:

public slots:

};


template<typename T>
struct RegisterLayout {
    RegisterLayout(const QString & name)
    {
        BaseLayout::registerLayout<T>(name);
    }
};

#endif // BASELAYOUT_H
