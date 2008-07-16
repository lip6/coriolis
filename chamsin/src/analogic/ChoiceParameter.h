#ifndef CHOICEPARAMETER_H
#define CHOICEPARAMETER_H

#include "DeviceParameter.h"

template <class Class>
class ChoiceParameter : public Parameter<Class> {
    public:
        typedef vector<string> Choices;
        ChoiceParameter<Class>(string id, Choices& choices,
                unsigned value, CallBack<Class>* callBack):
            Parameter<Class>(id, callBack), _choices(), _value(value) {
            if (_value > choices.size()) {
                throw Error("");
            }
        }
    private:
        Choices _choices;
        unsigned _value;

};

#endif // CHOICEPARAMETER_H
