#ifndef ATECHNOLOGYXMLPARSER_H_
#define ATECHNOLOGYXMLPARSER_H_

namespace Hurricane {
    class Technology;
}

class ATechnology;

class ATechnologyXmlParser {
    public:
        static ATechnology* parse(const char* filePath, Hurricane::Technology* technology);
};

#endif /*ATECHNOLOGYXMLPARSER_H_*/
