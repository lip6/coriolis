#ifndef AENV_H_
#define AENV_H_

class ATechnology;

class AEnv {
    public:
        static void create(const char* technoFilePath);
        static ATechnology* getATechnology();
};

#endif /* AENV_H_ */
