#include <libxml/parser.h>
#include <libxml/tree.h>

#include "hurricane/Technology.h"

#include "ATechnology.h"

#include "ATechnologyXmlParser.h"


using namespace Hurricane;


namespace {

void syntaxError(const string& reason) {
    throw Error(reason);
}

void readPhysicalRules(xmlNode* node, ATechnology* aTechnology) {
    if (node->type == XML_ELEMENT_NODE && node->children) {
        for (xmlNode* ruleNode = node->children;
            ruleNode;
            ruleNode = ruleNode->next) {
            if (ruleNode->type == XML_ELEMENT_NODE) {
                if (xmlStrEqual(ruleNode->name, (xmlChar*)"arule")) {
                    xmlChar* ruleNameC = xmlGetProp(ruleNode, (xmlChar*)"name");
                    xmlChar* valueC = xmlGetProp(ruleNode, (xmlChar*)"value");
                    xmlChar* refC = xmlGetProp(ruleNode, (xmlChar*)"ref");
                    xmlChar* layer1C = xmlGetProp(ruleNode, (xmlChar*)"layer1");
                    xmlChar* layer2C = xmlGetProp(ruleNode, (xmlChar*)"layer2");
                    if (ruleNameC && valueC && refC && layer1C && layer2C) {
                        string ruleName((const char*)ruleNameC);
                        double value = atof((const char*)valueC);
                        DbU::Unit unitValue= DbU::real(value);
                        string reference((const char*)refC);
                        Name layer1Name((const char*)layer1C);
                        Name layer2Name((const char*)layer2C);
                        aTechnology->addPhysicalRule(ruleName, layer1Name, layer2Name, false, unitValue, reference);
                    }
                } else if (xmlStrEqual(ruleNode->name, (xmlChar*)"rule")) {
                    xmlChar* ruleNameC = xmlGetProp(ruleNode, (xmlChar*)"name");
                    xmlChar* valueC = xmlGetProp(ruleNode, (xmlChar*)"value");
                    xmlChar* refC = xmlGetProp(ruleNode, (xmlChar*)"ref");
                    xmlChar* layerC = xmlGetProp(ruleNode, (xmlChar*)"layer");
                    xmlChar* layer1C = xmlGetProp(ruleNode, (xmlChar*)"layer1");
                    xmlChar* layer2C = xmlGetProp(ruleNode, (xmlChar*)"layer2");
                    if (ruleNameC && valueC && refC) {
                        string ruleName((const char*)ruleNameC);
                        double value = atof((const char*)valueC);
                        DbU::Unit unitValue= DbU::real(value);
                        string reference((const char*)refC);
                        if (layerC) {
                            Name layerName((const char*)layerC);
                            aTechnology->addPhysicalRule(ruleName, layerName, unitValue, reference);
                        } else if (layer1C && layer2C) {
                            Name layer1Name((const char*)layer1C);
                            Name layer2Name((const char*)layer2C);
                            aTechnology->addPhysicalRule(ruleName, layer1Name, layer2Name, true, unitValue, reference);
                        } else {
                            aTechnology->addPhysicalRule(ruleName, unitValue, reference);
                        }
                    } else {
                        syntaxError("");
                    }
                }
            }
        }
    }
}

ATechnology* parseFileAsTechnology(const char* filePath, Technology* technology) {
    ATechnology* aTechnology = ATechnology::create(technology);


    xmlDocPtr doc; /* the resulting document tree */

    doc = xmlReadFile(filePath, NULL, 0);
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse %s\n", filePath);
        exit(5);
    }

    /*Get the design element node */
    xmlNode* rootElement = xmlDocGetRootElement(doc);
    if (rootElement->type == XML_ELEMENT_NODE &&
            xmlStrEqual(rootElement->name, (xmlChar*)"technology")) {
        xmlNode* child = rootElement->children;
        for (xmlNode* node = child; node; node = node->next) {
            if (node->type == XML_ELEMENT_NODE) {
                if (xmlStrEqual(node->name, (xmlChar*)"physical_rules")) {
                    readPhysicalRules(node, aTechnology);
                } else {
                    syntaxError("unknown tag");
                }
            }
        }
    }
}

}

ATechnology* ATechnologyXmlParser::parse(const char* filePath, Technology* technology) {

    LIBXML_TEST_VERSION

    ATechnology* aTechnology = parseFileAsTechnology(filePath, technology);

    xmlCleanupParser();
    /*
     * this is to debug memory for regression tests
     */

    xmlMemoryDump();

    return NULL;
}
