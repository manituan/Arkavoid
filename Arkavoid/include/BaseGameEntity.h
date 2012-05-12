#ifndef BASE_GAME_ENTITY_H
#define BASE_GAME_ENTITY_H
#pragma warning (disable:4786)

#include <assert.h>

class BaseGameEntity
{
public:
  
  enum {default_entity_type = -1};

private:
  
	int         m_ID;
	int         m_iType;
	bool        m_bTag;

	static int  m_iNextValidID;

	void		SetID(int val);

protected:  
				//para que solo las clases descendentes puedan asignarle un ID.
				BaseGameEntity(int ID);

public:
  virtual		~BaseGameEntity(){}

  virtual void	Update( float delta ){}; 
  virtual void	Render()=0;


  
  //use this to grab the next valid ID
  static int	GetNextValidID(){return m_iNextValidID;}
  
  //this can be used to reset the next ID
  static void	ResetNextValidID(){m_iNextValidID = 0;}

  int          ID()const{return m_ID;}

  bool         IsTagged()const{return m_bTag;}
  void         Tag(){m_bTag = true;}
  void         UnTag(){m_bTag = false;}

  int          EntityType()const{return m_iType;}
  void         SetEntityType(int new_type){m_iType = new_type;}
};



      
#endif




