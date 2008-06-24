#ifndef AENV_H_
#define AENV_H_

class ATechnology;

class AEnv {
    public:
        static void create(const char* symbTechnoFilePath,
                const char* realTechnoFilePath,
                const char* graphicFilePath,
                const char* analogTechnoFilePath);
        static ATechnology* getATechnology();
};

#endif /* AENV_H_ */
