
#include "pch.h"
#include <boost/python.hpp>
#include "Entity/ClientEntity.h"

#include "Entity/ClientEntityManager.h"
#include <EntityDef/EntityType.h>

#include <pyscript/PyScriptAutoRegister.h>

#include <boost/noncopyable.hpp>
#include "ClientApp.h"

#include "Script_ClientEntityManager.h"
#include "Script_ClientApp.h"



using namespace boost::python;
namespace Aurora
{
	static EntityManagerWarpper sGetEntities(){return EntityManagerWarpper();}
	static ClientAppWarpper sGetApp(){return ClientAppWarpper();}

	BOOST_PYTHON_MODULE(Aurora)
	{
#include "EntityDef/DataTypes/PyExTypes.h"

		class_<ClientEntity ,bases<AttributeOwnerBase> >("ClientEntity",init< XEntityID,boost::shared_ptr<EntityType> ,AttributeFlagT >())
			.def("Base",&ClientEntity::CallBaseMethod)
			.def("Cell",&ClientEntity::CallCellMethod)
			.add_property("id",	&ClientEntity::GetID)
			.add_property("tid",&ClientEntity::GetTypeID)
			.add_property("isControlled",&ClientEntity::IsControlled)
			.add_property("_x",&ClientEntity::GetPosX,&ClientEntity::PySetPosX)
			.add_property("_y",&ClientEntity::GetPosY,&ClientEntity::PySetPosY)
			.add_property("_z",&ClientEntity::GetPosZ,&ClientEntity::PySetPosZ)
			.add_property("_Y",&ClientEntity::GetRotY,&ClientEntity::PySetRotY)
			.add_property("_P",&ClientEntity::GetRotP,&ClientEntity::PySetRotP)
			.add_property("_R",&ClientEntity::GetRotR,&ClientEntity::PySetRotR)
			.def("SetXYZ",&ClientEntity::SetPosXYZ)
			.def("SetYPR",&ClientEntity::SetRotYPR)
			.def("SyncPosRot",&ClientEntity::SyncPosRotToServer)
			;

		register_ptr_to_python<ClientEntityPtr >();

		class_<EntityManagerWarpper >("EntityManager")
			.def("__len__",&EntityManagerWarpper::Len)
			.def("__getitem__",&EntityManagerWarpper::Get)
			.def("has_key",&EntityManagerWarpper::HasKey)
			.def("get",&EntityManagerWarpper::Get)
			.def("keys",&EntityManagerWarpper::GetKeys)
			.def("values",&EntityManagerWarpper::GetValues)
			.def("items",&EntityManagerWarpper::GetItems)
			.def("player",&EntityManagerWarpper::GetPlayer)
			;


		def("entities",sGetEntities);

		def("app",sGetApp);

		enum_<ClientApp::EState>("States")
			;
		
		class_<ClientAppWarpper >("ClientApp")
			.def("StartLogin",	&ClientAppWarpper::StartLogin)
			.def("Disconnect",	&ClientAppWarpper::Disconnect)
			.def("Stop",		&ClientAppWarpper::Stop)
			;
	}
	AUTO_REGISTER_PYTHON_MODULE(Aurora);

}




