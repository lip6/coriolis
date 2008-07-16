#ifndef PARAMETER_H
#define PARAMETER_H

#include "CallBack.h"

class Parameter {
    public:
        const string _id;
    protected:
        Parameter(string id, CallBack* callBack): _id(id), _callBack(callBack) {}
    private:
        CallBack* _callBack;
};

#endif // PARAMETER_H
