#pragma once

#include "stdafx.h"
struct UnitSMPL;
class Territory;
class Order;
typedef std::map<std::string, Territory> TERRDATA;
typedef std::unordered_map<std::string, Order> ORDERDATA;
enum OrderType
{
   ATTACK,CONVOY,SUPPORT
};
class Order
{
public:
   Order();
   Order(UnitSMPL* me, OrderType ot,Order* supporterOf = nullptr,std::list<Territory*>& moveChain = std::list<Territory*>());
   OrderType getType() { return type; }
   Order* orderISupport() { return supporterOf; }
   friend int check_basicAttack(Order* o);
   friend int check_basicSupport(Order* o);
   friend int check_basicConvoy(Order* o);
private:
   OrderType type;
   Order* supporterOf; // Used if this is a support order
   std::list<Territory*> chain; // The chain from origin to finish 
                                 // this unit will travel
   UnitSMPL* me;
};
#define OC_OK 0 // Order chain was formulated sucesfully
#define OC_NOPATH 1
#define OC_INVALIDTERR 2

/* Order formulation theory

- All orders have a type
- All orders revolve around a specific units actions
- Multiple orders cannot be given for the same unit
- You cannot issue a support order for an action a unit doesnt take
- If a convoy, path must exist for order being convoyed.
   Determined by searching through orders for similar "suporterOf". This field
   is used by convoys as a common linker to make the chain of land movement valid
*/

Order formulate_basicAttack(UnitSMPL* me, Territory* destination);
Order formulate_basicSupportHold(UnitSMPL* me, Order* imSupporting);
Order formulate_basicConvoy(UnitSMPL* me, Order* movementLandOrder);
Order formulate_basicHold(UnitSMPL* me); // Order type will be set to attack

/* Rectify Process: Compare sway of attack orders over others.
*/
#define OR_OK 0
#define OR_WAIT 1
#define OR_UNITNO 2
#define OR_NOCONNECT 3
int check_basicAttack(Order* o);
int check_basicSupport(Order* o);
int check_basicConvoy(Order* o);

// Takes unit (gets origin territory) and computes if the end of the course can be reached by the steps
int check_convoyPath(UnitSMPL* me, std::list<Territory*> course,TERRDATA& tDat);

/* ORDER AS STRING
*/
typedef char OPERATOR; 
/* Possible operators
   '-' : To
   'S': supports
   'C': convoy

*/
/* SENTANCE: The whole text representation of an order
complete with a subject, an operator, and a predicate
*/

typedef std::string PREDICATE;
// Predicate can be an existing order string or a destination territory
// If predicate has an operator inside, it is in reference to another sentance.
// If predicate has no operator inside, it is just in reference to destination
typedef std::string SUBJECT;
class SENTANCE
{
public:
   SENTANCE();
   SENTANCE(const SUBJECT&, const OPERATOR,const PREDICATE&);
   bool operator==(const SENTANCE& other);
   bool operator!=(const SENTANCE& other);
   bool operator<(const SENTANCE& other);
   SUBJECT getSubject() { return subj; }
   OPERATOR getOperator() { return oper; }
   PREDICATE getPredicate() { return pred; }
private:

   SUBJECT subj;
   OPERATOR oper;
   PREDICATE pred;
};
// Predicate hasher

struct PHASH
{
   std::size_t operator()(const PREDICATE& k) const
   {  
      int numLetters = k.size();
      int shift = k[0] >> numLetters;
      for (int i = 0; i < k.size(); i++)
      {
         shift ^= k[i];
      }
      return shift;
   }
};

typedef std::unordered_map<PREDICATE, SENTANCE,PHASH>;
