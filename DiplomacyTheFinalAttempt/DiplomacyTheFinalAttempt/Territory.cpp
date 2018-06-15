#include "Territory.h"



Territory::Territory()
{
   fullName = "???";
   abrev = "???";
   pointValue = -1;
   type = TerritoryType::INLAND;
   armySet.lbl = "Land";
}

Territory::Territory(const std::string& name, const std::string& abrev,
   TerritoryType t,int pv)
{
   fullName = name;
   this->abrev = abrev;
   pointValue = pv;   
   type = t;
   armySet.lbl = "Land";
}
Territory& Territory::operator=(Territory& old)
{
   fullName = old.fullName;
   this->abrev = old.abrev;
   pointValue = old.pointValue;
   type = old.type;
   return *this;
}
Territory::Territory(const Territory& old)
{
   fullName = old.fullName;
   this->abrev = old.abrev;
   pointValue = old.pointValue;
   type = old.type;
   armySet.lbl = "Land";
}
bool Territory::operator==(const Territory& other)
{
   return (this->abrev == other.abrev);
}
bool Territory::operator!=(const Territory& other)
{
   return !(*this == other);
}
bool Territory::operator<(const Territory& other)
{
   return(abrev < other.abrev);
}
Territory::~Territory()
{
}
