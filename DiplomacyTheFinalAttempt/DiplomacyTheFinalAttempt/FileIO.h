#pragma once
#include "stdafx.h"
#include <xercesc\util\PlatformUtils.hpp>
#include <xercesc\dom\DOM.hpp>
#include <xercesc\parsers\XercesDOMParser.hpp>
#include <xercesc\util\XMLUni.hpp>

#define FILE_NOT_FOUND 1
#define FILE_SUCESS 0
#define FILE_NOPARSE 2
#define FILE_OOM 3

struct FCON
{
   Territory* ptr;
   int coastSide;
   std::vector<std::string> abrevBank;
   
};
class FileIO
{
public:
   typedef std::vector<FCON> FUTURECON;
   // Loads a board from an xml file using XERCES
   static int loadBoard(const std::string& fp, Board& b);
   static std::ostream& OUTPUT_STREAM;
   static std::vector<std::string> splitString(const char delim, const std::string& src);
private:
   static int processGameRules(xercesc::DOMElement* grElement, Board& b);
   static int processTeraData(xercesc::DOMElement* grElement, Board& b);
   static int parseConnections(xercesc::DOMElement* tElement, Territory* t,Board& b, FUTURECON& listing);
};

