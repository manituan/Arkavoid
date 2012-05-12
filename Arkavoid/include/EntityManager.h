#ifndef _ENTITYMANAGER_H_
#define _ENTITYMANAGER_H_ 1

#include <vector>
#include <list>

class BaseGameEntity;

class EntityManager
{
public:
	static EntityManager* Instance();

	~EntityManager();

	void Add( BaseGameEntity* entity );
	void Remove( BaseGameEntity* entity );
	void Remove( int ID );
	void RemoveType( int Type );
	void RemoveAll();

	BaseGameEntity* GetEntity( int ID );
	std::vector<BaseGameEntity*>& GetType( int Type );

	void Update( float delta );
	void Render();

protected:
	std::list<BaseGameEntity*> entitiesList;
	std::vector<BaseGameEntity*> garbage;
	std::vector<BaseGameEntity*> newEntities;

private:
	EntityManager() {}
	EntityManager(EntityManager& ) { }
	EntityManager& operator=(EntityManager& ) {}
};
#endif
