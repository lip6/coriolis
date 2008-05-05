#include <libxml/parser.h>
#include <libxml/tree.h>

#include "Technology.h"
using namespace Hurricane;

#include "TechnologyXmlParser.h"

namespace {

ATechnology* parseFileAsTechnology(const char* filePath, Technology* technology) {
    xmlDocPtr doc; /* the resulting document tree */

    doc = xmlReadFile(filePath, NULL, 0);
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse %s\n", filePath);
        exit(5);
    }

    /*Get the design element node */
    xmlNode* rootElement = xmlDocGetRootElement(doc);
    if (rootElement->type == XML_ELEMENT_NODE && xmlStrEqual(rootElement->name, (xmlChar*)"technology")) {
    }
}

}

ATechnology* parseTechnologyFromXml(const char* filePath, Technology* technology) {

    LIBXML_TEST_VERSION

    ATechnology* aTechnology = parseFileAsTechnology(filePath, technology);

    xmlCleanupParser();
    /*
     * this is to debug memory for regression tests
     */

    xmlMemoryDump();

    return NULL;
}
