#ifndef TECHNOLOGYXMLPARSER_H_
#define TECHNOLOGYXMLPARSER_H_

namespace Hurricane {
    class Technology;
}

class ATechnology;

class TechnologyXmlParser {
    public:
        static ATechnology* parseTechnologyFromXml(const char* filePath, Hurricane::Technology* technology);
};

#endif /*TECHNOLOGYXMLPARSER_H_*/
