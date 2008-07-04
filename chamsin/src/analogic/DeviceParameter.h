#ifndef DEVICEPARAMETER_H
#define DEVICEPARAMETER_H

class DeviceParameter {
    public:
        const string _id;
        string _getString() const;
        Record* _getRecord() const;
    protected:
        DeviceParameter(string id): _id(id) {}
};

#endif // DEVICEPARAMETER_H
