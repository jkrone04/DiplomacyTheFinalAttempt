#include "Board.h"



Board::Board()
{
   spaces = std::map<std::string, Territory>();

}


Board::~Board()
{
}
Territory* Board::getTPtr(const std::string& abrev)
{
   if (abrev.size() != 3)
      return nullptr;
   using namespace std;
   map<string, Territory>::iterator itr = spaces.find(abrev);
   if (itr == spaces.end())
      return nullptr;
   else
      return &(*itr).second;
}
void Board::addConnection(int coast, Territory* orig,Territory * t)
{
   using namespace std;   
   ConnectionSet* cSet = &orig->coasts[coast];
   vector<Territory*>::iterator res = find(cSet->connections.begin(), cSet->connections.end(), t);
   if (res == cSet->connections.end())
   {
      cSet->connections.push_back(t);
   }
   
}


bool doTheseConnect(TERRDATA& tData,const std::string& orig, const std::string& dest)
{
   TERRDATA::iterator search = tData.find(orig);
   TERRDATA::iterator s2 = tData.find(dest);
   if (search == tData.end() || s2 == tData.end()) return false;
   Territory* o = &search->second;
   Territory* d = &s2->second;


   return false;
}

ACON_DATA dtc_a(Territory * orig, Territory * dest,const bool dualMode)
{
   ACON_DATA ad;
   ad.dest = dest;
   ad.orig = orig;
   ad.isConnect = false;
   // Pass 1: origin -> destination
   ConnectionSet* aSet = orig->acessArmySet();
   bool otod = false; bool dtoo = false;
   for (int i = 0; i < aSet->connections.size(); i++)
   {
      if (otod) break;
      if (aSet->connections[i] == dest)
         otod = true;
   }
   if (!dualMode && otod) ad.isConnect = true; // If dual mode is off and otod...
   if (!dualMode) return ad; // If dual mode is off and we can stop checking
   // Pass 2: dest -> origin
   ConnectionSet* bSet = dest->acessArmySet();
   for (int i = 0; i < bSet->connections.size(); i++)
   {
      if (dtoo) break;
      if (bSet->connections[i] == orig)
         dtoo = true;
   }
   if (otod && dtoo) ad.isConnect = true;
   return ad;
}

FCON_DATA dtc_f(Territory * orig, Territory * dest, int origC,int destC,const bool dualMode)
{
   // Initialize return packet
   FCON_DATA fc;
   fc.orig = orig;
   fc.dest = dest;
   fc.isConnect = false;
   if (origC < 0 || origC > 3) origC = 0; // We will use default coast in case no valid specified
   if (destC < 0 || destC > 3) destC = 0; // We will use default coast in case no valid specified
   fc.coastNumDest = origC;
   fc.coastNumOrig = destC;
   // Pass 1: orig - > dest via orig coast
   ConnectionSet* aSet = orig->acessCoast(origC);
   bool otod = false; bool dtoo = false;
   for (int i = 0; i < aSet->connections.size(); i++)
   {
      if (otod) break;
      if (aSet->connections[i] == dest) otod = true;
   }
   if (!dualMode && otod) fc.isConnect = true;
   if (!dualMode || dest == nullptr) return fc;
   // Pass 2: dest - > origin via dest coast
   
   ConnectionSet* bSet = dest->acessCoast(destC);
  
   for (int i = 0; i < bSet->connections.size(); i++)
   {
      if (dtoo) break;
      if (bSet->connections[i] == orig) dtoo = true;
   }
   if (otod && dtoo) fc.isConnect = true;
   return fc;
}
