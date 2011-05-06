#ifndef __SPICE_CIRCUIT_H
#define __SPICE_CIRCUIT_H

#include <string>
#include <vector>
#include <map>
class ifstream;

namespace SPICE {
class Value;
class Subckt;
class Instance;
class Source;

class Circuit {
    public:
    enum LineType { ENDFILE   = -2 // means the getline failed : should only happens at end of file
                  , UNDEF     = -1 // not recognized
                  , EMPTY     = 0  // empty line
                  , COMMENT   = 1  // begins with '*...'
                  , INCLUDE   = 2  // begins with '.include '
                  , LIBRARY   = 3  // begins with '.lib '
                  , OPTION    = 4  // begins with '.option '
                  , PARAM     = 5  // begins with '.param '
                  , SUBCKT    = 6  // begins with '.subckt '
                  , CONTINUE  = 7  // begins with '+ ' 
                  , ENDSUB    = 8  // begins with '.ends '
                  , END       = 9  // begins with '.end '
                  
                  , INSTANCE  = 20 // begins with 'x...'
                  , MOSFET    = 21 // begins with 'm...'
                  , CAPACITOR = 22 // begins with 'c...'
                  , RESISTOR  = 23 // begins with 'r...'
                  , INDUCTOR  = 24 // begins with 'l...'
                  , DIODE     = 25 // begins with 'd...'

                  , VOLTAGE   = 30 // begins with 'v...' voltage source
                  , CURRENT   = 31 // begins with 'i...' current source
    };

    typedef std::pair<std::string, std::string> string_pair;
    typedef std::vector<string_pair>            strpair_vector;
    typedef std::vector<std::string>            string_vector;
    typedef std::map<std::string, std::string>  strings_map;
    private:
    unsigned               _line;
    std::string            _title;
    string_vector          _includes;
    strpair_vector         _libraries;
    strings_map            _options;
    strings_map            _parameters;
    std::vector<Subckt*>   _subckts;
    std::vector<Instance*> _instances;
    std::vector<Source*>   _sources;

    public:
    Circuit(): _line(0), _title(""), _includes(), _libraries(), _options(), _parameters(), _subckts(), _instances() {};
    ~Circuit() {};

    inline std::string getTitle();
    inline const string_vector&          getIncludes();
    inline const strpair_vector&         getLibraries();
    inline const strings_map&            getOptions();
    inline const strings_map&            getParameters();
    inline const std::vector<Subckt*>&   getSubckts();
    inline const std::vector<Instance*>& getInstances();
    inline const std::vector<Source*>&   getSources();


    inline void setTitle   (std::string);
    inline void addInclude (std::string);
    inline void addLibrary (std::string file, std::string type = "");
    inline void addInstance(Instance*);
    inline void addSource  (Source*);

    void    addOption   (std::string, std::string);
    void    addParameter(std::string, std::string);
    Subckt* addSubckt   (std::string);

    static Circuit* readFromFile(const std::string&);
           void     writeToFile (const std::string&);

    private:
    static Circuit::LineType getNextLineType    (std::ifstream& file, std::string& line);
    static Circuit::LineType getNextFullLineType(std::ifstream& file, std::string& fullLine);
    static bool              lineBeginsWith     (std::string line, std::string substr);
    static void              cleanMultipleSpaces(std::string&);
    static void              tokenize           (std::string line, std::vector<std::string>& tokens);
    static void              parametrize        (std::string param, std::string& name, std::string& value);
    static void              writeInstance      (std::ofstream& file, Instance* inst);

};
inline       std::string              Circuit::getTitle()      { return _title;      }
inline const Circuit::string_vector&  Circuit::getIncludes()   { return _includes;   }
inline const Circuit::strpair_vector& Circuit::getLibraries()  { return _libraries;  }
inline const Circuit::strings_map&    Circuit::getOptions()    { return _options;    }
inline const Circuit::strings_map&    Circuit::getParameters() { return _parameters; }
inline const std::vector<Subckt*>&    Circuit::getSubckts()    { return _subckts;    }
inline const std::vector<Instance*>&  Circuit::getInstances()  { return _instances;  }
inline const std::vector<Source*>&    Circuit::getSources()    { return _sources;    }

inline void Circuit::setTitle   (std::string title)                  { _title = title; }
inline void Circuit::addInclude (std::string include)                { _includes.push_back(include); }
inline void Circuit::addLibrary (std::string file, std::string type) { _libraries.push_back(Circuit::string_pair(file, type)); }
inline void Circuit::addInstance(Instance* inst)                     { _instances.push_back(inst); }
inline void Circuit::addSource  (Source* source)                     {_sources.push_back(source); }
}
#endif
