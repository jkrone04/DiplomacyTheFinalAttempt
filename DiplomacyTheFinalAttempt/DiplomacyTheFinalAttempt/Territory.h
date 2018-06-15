#pragma once
#include "stdafx.h"

enum Side
{
   NORTH,EAST,SOUTH,WEST
};
class Territory;
class FileIO;
struct ConnectionSet
{
   std::string lbl;
   std::vector<Territory*> connections;
};
enum TerritoryType
{
   OCEAN,COAST,INLAND
};
class Territory
{
public:
   friend class Board;
   friend class FileIO;
   Territory();
   Territory(const std::string& name, const std::string& abrev,
      TerritoryType t,int pv = 0);
   Territory(const Territory& old);
   Territory& operator=(Territory& old);
   bool operator==(const Territory& other);
   bool operator!=(const Territory& other);
   bool operator<(const Territory& other);
   ~Territory();
   const int getPointValue() { return pointValue; }
   const std::string getAbreviation() { return abrev; } // REturns an abreviation length 3
   const std::string getFullName() { return fullName; }
   ConnectionSet* acessCoast(int dValue) { return &coasts[dValue]; }
   ConnectionSet* acessArmySet() { return &armySet; }
private:
   int pointValue;
   TerritoryType type;
   std::string abrev;
   std::string fullName;
   ConnectionSet coasts[4]; // NORTH,EAST,SOUTH,WEST
   ConnectionSet armySet;
};

