#ifndef CHOICEPARAMETER_H
#define CHOICEPARAMETER_H

#include "DeviceParameter.h"

class ChoiceParameter : public DeviceParameter {
    public:
        typedef vector<string> Choices;
        ChoiceParameter(string id, Choices& choices, unsigned value):
            DeviceParameter(id), _choices(), _value(value) {
            if (_value > choices.size()) {
                throw Error("");
            }
        }
    private:
        Choices _choices;
        unsigned _value;

};

#endif // CHOICEPARAMETER_H
