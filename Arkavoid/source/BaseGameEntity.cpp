#include "BaseGameEntity.h"


int BaseGameEntity::m_iNextValidID = 0;

BaseGameEntity::BaseGameEntity(int ID):m_iType(default_entity_type),
                                       m_bTag(false)
{
  SetID(ID);
}

void BaseGameEntity::SetID(int val)
{
  //make sure the val is equal to or greater than the next available ID
  assert ( (val >= m_iNextValidID) && "<BaseGameEntity::SetID>: invalid ID");

  m_ID = val;
    
  m_iNextValidID = m_ID + 1;
}
