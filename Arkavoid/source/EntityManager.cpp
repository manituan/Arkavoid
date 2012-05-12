#include "EntityManager.h"
#include "BaseGameEntity.h"

EntityManager* EntityManager::Instance()
{
	static EntityManager instance;
	return &instance;
}

EntityManager::~EntityManager()
{
	RemoveAll();
}

void EntityManager::Add( BaseGameEntity* entity )
{
	std::list<BaseGameEntity*>::iterator i = entitiesList.begin();
	while( i != entitiesList.end() )
	{
		if( (*i) == entity) return; //para ver si metemos dos veces el mismo objeto
		i++;
	}
	newEntities.push_back(entity);
	

	return ;
}

void EntityManager::Remove( BaseGameEntity* entity )
{
	garbage.push_back(entity);

	
}

void EntityManager::Remove( int ID )
{
	std::list<BaseGameEntity*>::iterator i = entitiesList.begin();
	while( i != entitiesList.end() )
	{
		if( (*i)->ID() == ID)
		{
			garbage.push_back(*i);
			//solo podrá haber una entidad con ese id, por eso se puede meter return
			return;
		}
		i++;
	}
}

void EntityManager::RemoveType( int Type )
{
	std::list<BaseGameEntity*>::iterator i = entitiesList.begin();
	while( i != entitiesList.end() )
	{
		if( (*i)->EntityType() == Type)
		{
			garbage.push_back(*i);
			
		}
		i++;
	}
}

void EntityManager::RemoveAll()
{
	std::list<BaseGameEntity*>::iterator i = entitiesList.begin();
	while( i != entitiesList.end() )
	{
		delete ((*i));
		i++;
	}
	entitiesList.clear();
}

BaseGameEntity* EntityManager::GetEntity( int ID )
{
	std::list<BaseGameEntity*>::iterator i = entitiesList.begin();
	while( i != entitiesList.end() )
	{
		if( (*i)->ID() == ID) return (*i);
		i++;
	}
	return NULL;
}

std::vector<BaseGameEntity*>& EntityManager::GetType( int Type )
{
	static std::vector<BaseGameEntity*> FindEntity;

	FindEntity.clear();

	std::list<BaseGameEntity*>::iterator i = entitiesList.begin();
	while( i != entitiesList.end() )
	{
		if( (*i)->EntityType() == Type) FindEntity.push_back( *i );
		i++;
	}

	return FindEntity;
}

void EntityManager::Update( float delta )
{
	//Add new entities
	for(unsigned int idx=0;idx<newEntities.size();idx++){
	
	entitiesList.push_back( newEntities[idx] );
	
	}
	newEntities.clear();
	

	//Del garbageEntities
	for(unsigned idx=0;idx<garbage.size();idx++)
	{
	
	delete garbage[idx];
	entitiesList.remove( garbage[idx] );
	}
	garbage.clear();

	std::list<BaseGameEntity*>::iterator i = entitiesList.begin();
	while( i != entitiesList.end() )
	{
		(*i)->Update( delta );
		i++;
	}
}

void EntityManager::Render()
{
	std::list<BaseGameEntity*>::iterator i = entitiesList.begin();
	while( i != entitiesList.end() )
	{
		(*i)->Render();
		i++;
	}
}

