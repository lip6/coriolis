#ifndef STEPPARAMETER_H
#define STEPPARAMETER_H

template <Type>
class StepParameter : public DeviceParameter {
    public:
        StepParameter(Type min, Type max, Type step):
            _min(min), _max(max), _step(step) {} 
        Type getMin() const { return _min; }
        Type getMax() const { return _max, }
        Type getStep() const { return _step; }
        Type getValue() const { return _value, }
        void setValue(Type value);

    private:
        Type _min;
        Type _max;
        Type _step;
        Type _value;
};

typedef StepParameter<int> StepIntParameter;

#endif // STEPPARAMETER_H
