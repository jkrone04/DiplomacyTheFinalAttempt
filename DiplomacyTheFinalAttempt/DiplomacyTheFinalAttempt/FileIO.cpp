#include "FileIO.h"
std::ostream& FileIO::OUTPUT_STREAM = std::cout;
int FileIO::loadBoard(const std::string& fp, Board& b)
{
   using namespace std;
   /* STEP 1: Check to make sure file exists. If it doesnt
   return FILE_NOT_FOUND
   */
   ifstream file(fp);
   if (!file)
   {
      return FILE_NOT_FOUND;
   }
   /* STEP 2: Initialize XERCES components for parsing
   */
   using namespace xercesc;
   file.close();
   try {
      XMLPlatformUtils::Initialize();
      XercesDOMParser parser;
      parser.setValidationScheme(XercesDOMParser::Val_Never);
      parser.setDoNamespaces(false);
      parser.setDoSchema(false);
      parser.setLoadExternalDTD(false);
      parser.parse(fp.c_str());

      DOMDocument* xmlDoc = parser.getDocument();
      DOMElement* root = xmlDoc->getDocumentElement();
      char* str = XMLString::transcode(root->getNodeName());
      if (strcmp(str,"DiplomacyMap") == 0)
      {
         OUTPUT_STREAM << "FileIO| Now Parsing a diplomacy map!" << endl;
      }
      else { return FILE_NOPARSE; }
      Board backupBoard;// Initialize a new board which will be filled
      XMLString::release(&str);
      DOMNodeList* children = root->getChildNodes();
      DOMNode* gameRuleElement = nullptr;
      DOMNode* terraDataElement = nullptr;
      for (int i = 0; i < children->getLength(); i++)
      {
         DOMNode* current = children->item(i);
         if (current->getNodeType() == DOMNode::COMMENT_NODE)
            continue; // WE wont print it out
         else if (current->getNodeType() == DOMNode::ELEMENT_NODE)
         {
            char* name = XMLString::transcode(children->item(i)->getNodeName());
            OUTPUT_STREAM <<"FileIO| Parsing tag: " << name << endl;
            if (strcmp(name, "GameRules") == 0)
               gameRuleElement = current;
            else if (strcmp(name, "TerritoryData") == 0)
               terraDataElement = current;
            // Clean up the name string
            XMLString::release(&name);
         }         
      }
      // Check to be sure both major tags are present
      if (gameRuleElement == nullptr || terraDataElement == nullptr)
         return FILE_NOPARSE;
      // STEP 2: Parse Game rules element 
      int result;// = processGameRules((DOMElement*)gameRuleElement, b);
      /*if (result != FILE_SUCESS)
      {
         OUTPUT_STREAM << "FileIO| Error: Could not process game rules."
            << "(Terminating XML parsing)" << endl;
         return result;
      }*/
         
      // STEP 3: Parse TerritoryData element
      result = processTeraData((DOMElement*)terraDataElement, b);
      if (result != FILE_SUCESS) {
         OUTPUT_STREAM << "FileIO| Error: Could not process territory data."
            << "(Terminating XML parsing)" << endl;
         return result;
      }
         
      
   }
   catch (const XMLException& exc)
   {
      char* msg = XMLString::transcode(exc.getMessage());
      std::cerr << msg << endl;
      XMLString::release(&msg);
      return FILE_NOPARSE;
   }
   
   

   /*: Terminate XERCES
   */
   XMLPlatformUtils::Terminate();
   return FILE_SUCESS;
}

std::vector<std::string> FileIO::splitString(const char delim, const std::string & src)
{
   using namespace std;
   vector<string> parts;
   int newStart = 0;
   for (int i = 0; i < src.size(); i++)
   {
      if (src[i] == delim)
      {         
         parts.push_back(src.substr(newStart, i - newStart));
         newStart = i + 1;
      }
   }
   std::string lSub = src.substr(newStart, src.size() - newStart);
   if (lSub.size() > 0)
      parts.push_back(lSub);
   return parts;
}

int FileIO::processGameRules(xercesc::DOMElement * grElement, Board & b)
{
   using namespace std;
   OUTPUT_STREAM << "FileIO| Now processing game rules" << endl;
   return FILE_SUCESS;
}
int FileIO::processTeraData(xercesc::DOMElement * grElement, Board & b)
{
   using namespace std;
   OUTPUT_STREAM << "FileIO| Now processing territory data" << endl;
   using namespace xercesc;
   DOMNodeList* contents = grElement->getChildNodes();
   std::list<DOMElement*> recognizedSpots;

   for (int i = 0; i < contents->getLength(); i++)
   {
      DOMNode* current = contents->item(i);
      if (current->getNodeType() == DOMNode::ELEMENT_NODE)
      {
         // Cast the current node to DOMElement 
         // and place in recognized element list for
         // later use
         recognizedSpots.push_back((DOMElement*)current);
      }
   }
   // Stroke 2: Parse each individual DOM element
   /* STRUCTURE:
   <name (attributes)>
   (Required: abrev,type,value)
   <Connections>
   <defaultSet> or <c_0> to <c_3>
   c_0 coresponds to NORTH, c_1 to EAST, etc...
   <GraphicsData>
   <Static>
   <Interaction>

   */
   FUTURECON queuedConResolutions;
   for (std::list<DOMElement*>::iterator itr = recognizedSpots.begin();
      itr != recognizedSpots.end(); itr++)
   {
      DOMElement* current = *itr;
      DOMNamedNodeMap* attr = current->getAttributes();
      // Get pointers to the attributes
      DOMAttr* abrevAttr = (DOMAttr*)attr->getNamedItem(L"abrev");
      DOMAttr* typeAttr = (DOMAttr*)attr->getNamedItem(L"type");
      DOMAttr* valueAttr = (DOMAttr*)attr->getNamedItem(L"value");
      int value = 0;
      TerritoryType tType;
      if (!abrevAttr)
      {
         OUTPUT_STREAM << "FileIO| Error: Not all attributes found. \n";
         return FILE_NOPARSE;
      }
      char* valueValue = nullptr;
      char* typeValue = nullptr;
      if (!typeAttr)
      {
         // Provide default type
         tType = TerritoryType::OCEAN;

      }
      else {
         typeValue = XMLString::transcode(typeAttr->getValue());
         std::string tTypeStr(typeValue);
         if (tTypeStr == "OCEAN")
            tType = TerritoryType::OCEAN;
         else if (tTypeStr == "COAST")
            tType = TerritoryType::COAST;
         else
            tType = TerritoryType::INLAND;
      }
      if (!valueAttr)
      {
         value = 0;
      }
      else {
         valueValue = XMLString::transcode(valueAttr->getValue());
         try {
            value = stoi(std::string(valueValue));

         }
         catch (exception e) {
            OUTPUT_STREAM << "FileIO| Error: Failed to parse numeric attributes\n";
            return FILE_NOPARSE;
         }
      }
      char* abrevValue = XMLString::transcode(abrevAttr->getValue());
      char* nameValue = XMLString::transcode(current->getTagName());

      const std::string abrev(abrevValue);


      if (abrev.size() != 3)
         return FILE_NOPARSE;
      Territory t(std::string(nameValue), abrev, tType, value);
      std::pair<const std::string, Territory> p = std::make_pair(abrev, t);
      std::pair<map<string,Territory>::iterator,bool> r = b.spaces.insert(p);

      if (parseConnections(current,&r.first->second, b, queuedConResolutions) != FILE_SUCESS)
      {
         OUTPUT_STREAM << "FileIO| Error: Could not parse connections\n";
         b.spaces.clear();
         return FILE_NOPARSE;
      }
      XMLString::release(&abrevValue);
      XMLString::release(&nameValue);
      XMLString::release(&valueValue);
      XMLString::release(&typeValue);
   }
   // Stage 3: Now use the queued connections to form real ones
   for (FUTURECON::const_iterator citr = queuedConResolutions.begin();
      citr != queuedConResolutions.end(); citr++)
   {
      FCON fcon = (*citr);
      int cIndx = fcon.coastSide;
      Territory* t = fcon.ptr;
      if (fcon.coastSide == -1)
      {
         // We are dealing with an army connection, this one is special.
         for (vector<string>::const_iterator sitr = fcon.abrevBank.begin(); sitr != fcon.abrevBank.end();
            sitr++)
         {
            map<string, Territory>::iterator found = b.spaces.find(*sitr);
            if (found != b.spaces.end())
            {
               t->armySet.connections.push_back(&found->second);
            }
         }
      }
      else {
         for (vector<string>::const_iterator sitr = fcon.abrevBank.begin(); sitr != fcon.abrevBank.end();
            sitr++)
         {
            map<string, Territory>::iterator found = b.spaces.find(*sitr);

            if (found != b.spaces.end())
            {
               b.addConnection(cIndx, t, &found->second);
            }
         }
      }
         
   }
   return FILE_SUCESS;
}
int FileIO::parseConnections(xercesc::DOMElement * tElement,Territory* t, Board & b,
   FileIO::FUTURECON& listing)
{
   // Step 1: Locate the "Connections" tag
   using namespace std;
   using namespace xercesc;
   DOMNodeList* children = tElement->getChildNodes();
   DOMElement* conElement = nullptr;
   for (int i = 0; i < children->getLength(); i++)
   {
      DOMNode* cr = children->item(i);
      if (cr->getNodeType() == DOMNode::ELEMENT_NODE) {
         // This is a confirmed element type
         char* tName = XMLString::transcode(cr->getNodeName());
         if (strcmp(tName, "Connections") == 0)
         {            
            conElement = (DOMElement*)cr;
            break;
         }
         XMLString::release(&tName);
      }
   }
   // Make sure we identified connection element before continuing
   if (!conElement)
      return FILE_NOPARSE;
   
   // Step 2: Parse coastal sets

   std::vector<DOMElement*> coastalSets;
   children = conElement->getChildNodes();
   for (int i = 0; i < children->getLength(); i++)
   {
      if (children->item(i)->getNodeType() == DOMNode::ELEMENT_NODE)
         coastalSets.push_back((DOMElement*)children->item(i));
   }
   if (coastalSets.size() <= 0 || coastalSets.size() > 4)
   {
      OUTPUT_STREAM << "FileIO| Error: Excess coastal definitions" << endl;
      return FILE_NOPARSE;
   }
   FCON east, south, west, north,army; // Future connection set structs
   east.ptr = nullptr; south.ptr = nullptr; west.ptr = nullptr; north.ptr = nullptr;
   army.ptr = nullptr;
   // Iterate through coastal sets (there should be at max 4)
   for (vector<DOMElement*>::iterator itr = coastalSets.begin();
      itr != coastalSets.end(); itr++)
   {
      char* tagName = XMLString::transcode((*itr)->getNodeName());
      
      if (strcmp(tagName, "c_1") == 0)
      {
         char* v = XMLString::transcode((*itr)->getTextContent());
         std::string content(v);
         content.erase(std::remove_if(content.begin(), content.end(), ::isspace), content.end());
         vector<string> abrevs = FileIO::splitString(',', content);
         east.coastSide = 1;
         east.ptr = t;
         east.ptr->coasts[1].lbl = "East";
         for (int i = 0; i < abrevs.size(); i++)
         {
            if (abrevs[i].size() != 3)
            {
               OUTPUT_STREAM << "FileIO| Error: Abreviation was not 3 char\n";
               return FILE_NOPARSE;
            }
            
            east.abrevBank.push_back(abrevs[i]);
         }
         XMLString::release(&v);
      }
      else if (strcmp(tagName, "c_2") == 0)
      {
         char* v = XMLString::transcode((*itr)->getTextContent());
         if (v == NULL)
         {
            OUTPUT_STREAM << "FileIO| Error: Null content" << endl;
            return FILE_NOPARSE;
         }
         std::string content(v);
         content.erase(std::remove_if(content.begin(), content.end(), ::isspace), content.end());
         vector<string> abrevs = FileIO::splitString(',', content);
         south.coastSide = 2;
         south.ptr = t;
         south.ptr->coasts[2].lbl = "South";
         for (int i = 0; i < abrevs.size(); i++)
         {
            if (abrevs[i].size() != 3)
            {
               OUTPUT_STREAM << "FileIO| Error: Abreviation was not 3 char\n";
               return FILE_NOPARSE;
            }
            south.abrevBank.push_back(abrevs[i]);
         }
         XMLString::release(&v);

      }
      else if (strcmp(tagName, "c_3") == 0)
      {
         char* v = XMLString::transcode((*itr)->getTextContent());
         std::string content(v);
         content.erase(std::remove_if(content.begin(), content.end(), ::isspace), content.end());
         vector<string> abrevs = FileIO::splitString(',', content);
         west.coastSide = 3;
         west.ptr = t;
         west.ptr->coasts[3].lbl = "West";
         for (int i = 0; i < abrevs.size(); i++)
         {
            if (abrevs[i].size() != 3)
            {
               OUTPUT_STREAM << "FileIO| Error: Abreviation was not 3 char\n";
               return FILE_NOPARSE;
            }
            west.abrevBank.push_back(abrevs[i]);
         }
         XMLString::release(&v);
      }
      else if (strcmp(tagName, "army") == 0)
      {
         char* v = XMLString::transcode((*itr)->getTextContent());
         std::string content(v);
         content.erase(std::remove_if(content.begin(), content.end(), ::isspace), content.end());
         vector<string> abrevs = FileIO::splitString(',', content);
         army.coastSide = -1;
         army.ptr = t;
         
         for (int i = 0; i < abrevs.size(); i++)
         {
            if (abrevs[i].size() != 3)
            {
               OUTPUT_STREAM << "FileIO| Error: Abreviation was not 3 char\n";
               return FILE_NOPARSE;
            }
            army.abrevBank.push_back(abrevs[i]);
         }
         XMLString::release(&v);
      }
      else{
         // The default set or north set
         north.coastSide = 0;
         if (strcmp(tagName, "defaultSet") != 0 && strcmp(tagName, "c_0") != 0)
         {
            OUTPUT_STREAM << "FileIO| Warning: Default or northset mislabeled" <<
               "process will continue..." << endl;
         }
         char* v = XMLString::transcode((*itr)->getTextContent());
         std::string content(v);
         content.erase(std::remove_if(content.begin(), content.end(), ::isspace), content.end());
         vector<string> abrevs = FileIO::splitString(',', content);
         north.coastSide = 0;
         north.ptr = t;
         if (strcmp(tagName, "defaultSet") == 0)
            north.ptr->coasts[0].lbl = "";
         else
            north.ptr->coasts[0].lbl = "North"; 

         for (int i = 0; i < abrevs.size(); i++)
         {
            if (abrevs[i].size() != 3)
            {
               OUTPUT_STREAM << "FileIO| Error: Abreviation was not 3 char\n";
               return FILE_NOPARSE;
            }
            north.abrevBank.push_back(abrevs[i]);
         }
         XMLString::release(&v);
      }

      XMLString::release(&tagName);
   }
   if (army.ptr)
      listing.push_back(army);
   if (east.ptr)
      listing.push_back(east);
   if (west.ptr)
      listing.push_back(west);
   if (south.ptr)
      listing.push_back(south);
   if (north.ptr)
      listing.push_back(north);
   // Because there should only be two children, we may
   // use getFirstElementChild and getLastElementChild

   return FILE_SUCESS;
}