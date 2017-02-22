
#include "pch.h"
#include "Entity/BaseEntity.h"
#include <EntityDef/EntityType.h>
#include <pyscript/PyScriptAutoRegister.h>

#include "Script_BaseEntity.h"
#include "Script_BaseEntityManager.h"
#include "Script_BaseApp.h"


namespace Aurora
{
	using namespace boost::python;

	static BaseEntityManagerWarpper sGetEntities(){return BaseEntityManagerWarpper();}
	static BaseAppWarpper sGetApp(){return BaseAppWarpper();}

	BOOST_PYTHON_MODULE(Aurora)
	{

#include "EntityDef/DataTypes/PyExTypes.h"

		class_<BaseEntity ,bases<AttributeOwnerBase> >("BaseEntity",init< EntityID,EntityTypePtr >())
			.def("Client",&BaseEntity::CallClientMethod)
			.def("Cell",&BaseEntity::CallCellMethod)
			.add_property("id",	&BaseEntity::GetID)
			.add_property("tid",&BaseEntity::GetTypeID)
			.def("EnterSpace", &BaseEntity::EnterSpace, args("x", "y"),"asdb")
			;

		register_ptr_to_python<BaseEntityPtr >();

		class_<BaseEntityManagerWarpper >("BaseEntityManager")
			.def("__len__",&BaseEntityManagerWarpper::Len)
			.def("__getitem__",&BaseEntityManagerWarpper::Get)
			.def("has_key",&BaseEntityManagerWarpper::HasKey)
			.def("get",&BaseEntityManagerWarpper::Get)
			.def("keys",&BaseEntityManagerWarpper::GetKeys)
			.def("values",&BaseEntityManagerWarpper::GetValues)
			.def("items",&BaseEntityManagerWarpper::GetItems)
			;

		def("entities",sGetEntities);

		def("app",sGetApp);

		class_<BaseAppWarpper >("BaseApp",
			"Represent The App Instance")
			.def("Stop",		&BaseAppWarpper::Stop)
			;
	}
	AUTO_REGISTER_PYTHON_MODULE(Aurora);
}




