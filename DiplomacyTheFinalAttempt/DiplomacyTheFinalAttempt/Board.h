#pragma once
#include "stdafx.h"
class Territory;
class Order;
typedef std::map<std::string, Territory> TERRDATA;
typedef std::unordered_map<std::string, Order> ORDERDATA;

enum UnitType
{
   ARMY,FLEET
};

struct UnitSMPL
{
   std::string place;
   UnitType type;
};
class Board
{
public:
   friend class FileIO;
   Board();
   ~Board();
   // Gets a specific territory: returns nullptr for invalid request
   Territory* getTPtr(const std::string& abrev);
private:
   //Stores abreviation with territory
   TERRDATA spaces;
   ORDERDATA orders;
   void addConnection(int coast, Territory* orig,Territory* t);
   
};
typedef struct
{
   Territory* orig = nullptr;
   Territory* dest = nullptr;
   bool isConnect = false; // Are these connected
} ACON_DATA;
typedef struct
{
   Territory* orig = nullptr;
   Territory* dest = nullptr;
   bool isConnect = false; // Are these connected
   int coastNumOrig = -1; // Coast on which the origin ship connects to dest
   int coastNumDest = -1; // Coast on which the destination connects to the origin
} FCON_DATA;

bool doTheseConnect(TERRDATA& tDat,const std::string& orig, const std::string& dest);
ACON_DATA dtc_a(Territory* orig, Territory* dest,const bool dualMode = true); // Army
FCON_DATA dtc_f(Territory* orig, Territory* dest = nullptr, int origC = -1,int destC = -1,const bool dualMode = true); // Fleet


