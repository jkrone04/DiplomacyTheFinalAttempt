#pragma once
enum UnitType
{ TANK,FLEET };
class Unit
{
public:
   Unit(UnitType t,int ID);
   ~Unit();
   UnitType getUnitType() { return m_type; }
   int getID() { return myID; }
private:
   UnitType m_type;
   int myID;
};

