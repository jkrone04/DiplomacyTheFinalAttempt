#include "Orders.h"

Order::Order()
{

}
Order::Order(UnitSMPL* me, OrderType ot, Order* supporterOf, std::list<Territory*>& moveChain)
{

}

Order formulate_basicAttack(UnitSMPL* me, Territory* destination)
{
   std::list<Territory*> c;
   c.push_front(destination);
   return Order(me, OrderType::ATTACK, nullptr, c);
}
Order formulate_basicSupportHold(UnitSMPL* me, Order* imSupporting)
{
   return Order(me, OrderType::SUPPORT, imSupporting);
}

Order formulate_basicConvoy(UnitSMPL* me, Order* movOrder)
{
   return Order(me, OrderType::CONVOY, movOrder);
}
Order formulate_basicHold(UnitSMPL* me)
{
   return Order(me, OrderType::ATTACK, nullptr);
}

// Expected: Attacks being convoyed havent been checked, they will be checked here
int check_basicAttack(Order * o)
{
   // Step 1: Check to be sure that the unit is an actual unit existing on a space.
   if (o->me == nullptr)
      return OR_UNITNO;
  
   // If unit chain length is > 1, this must be a convoy order, wait for further processing
   if (o->chain.size() > 1)
      return OR_WAIT;
   // Step 2: Differentiate hold order from attack order
   if (o->chain.size() == 1)
   {
      // Step 3: Check to make sure that
      //the territory the unit is on connects to the destination
   }
   return OR_OK;
}

int check_basicSupport(Order * o)
{
   return OR_OK;
}

int check_basicConvoy(Order * o)
{
   return OR_OK;
}

int check_convoyPath(UnitSMPL * me, std::list<Territory*> course, TERRDATA& tDat)
{
   // Step 1: Check that unit is a unit
   if (me == nullptr || me->place == "")
      return OC_INVALIDTERR;
   // Step 2: computer starting location
   TERRDATA::iterator itr = tDat.find(me->place);
   if (itr == tDat.end())
      return OC_INVALIDTERR;
   // Step 3: Add territory to the begining and process path
   course.push_front(&itr->second);
   for (std::list<Territory*>::const_iterator litr = course.begin();
      litr != --course.end(); litr++)
   {
      bool connectedToNext = false;
      if (me->type == UnitType::ARMY)
      {
         ConnectionSet* armySet = (*litr)->acessArmySet();
         for (int i = 0; i < armySet->connections.size(); i++)
         {
            Territory* tCon = armySet->connections[i];
            if (tCon == (*(litr++)))
            {
               connectedToNext = true;
               break;
            }
         }
      }
      else
      {
         for (int i = 0; i < 4; i++)
         {
            ConnectionSet* currentCoast = (*litr)->acessCoast(i);
            for (int j = 0; j < currentCoast->connections.size(); j++)
            {
               Territory* tCon = currentCoast->connections[j];
               if (tCon == (*(litr++)))
               {
                  connectedToNext = true;
                  break;
               }
            }
            if (connectedToNext) break;
         }
      }
      if (!connectedToNext)
      {
         return OC_NOPATH;
      }
   }
   // Ending territory doesnt need to connecty to anything else
   return OC_OK;
}


SENTANCE::SENTANCE()
{

}
SENTANCE::SENTANCE(const SUBJECT& s, const OPERATOR o, const PREDICATE& p)
{

}
bool SENTANCE::operator==(const SENTANCE& other)
{
   return false;
}
bool SENTANCE::operator!=(const SENTANCE& other)
{
   return false;
}
bool SENTANCE::operator<(const SENTANCE& other)
{
   return false;
}