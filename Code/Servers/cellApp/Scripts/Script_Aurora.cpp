
#include "pch.h"
#include <boost/python.hpp>
#include "Entity/CellEntity.h"
#include <EntityDef/EntityType.h>
#include <pyscript/PyScriptAutoRegister.h>

#include "Script_CellEntity.h"
#include "Script_CellEntityManager.h"
#include "Script_CellApp.h"

//#include "EntityDef/DataTypes/PyExTypes.h"


namespace Aurora
{
	using namespace boost::python;

	static CellEntityManagerWarpper sGetEntities(){return CellEntityManagerWarpper();}
	static CellAppWarpper sGetApp(){return CellAppWarpper();}
	BOOST_PYTHON_MODULE(Aurora)
	{
#include "EntityDef/DataTypes/PyExTypes.h"
		class_<CellEntity ,bases<DataTypeFixDict_PyObject> >("CellEntity",init< EntityID ,EntityTypePtr ,AttributeFlagT, AppID >())
			.def("__setattr__",&CellEntity::py_setattr)
			.def("Base",&CellEntity::CallBaseMethod)
			.def("Cell",&CellEntity::CallCellMethod)
			.def("OwnClient",&CellEntity::CallOwnClientMethod)
			.def("OtherClient",&CellEntity::CallOtherClientMethod)
			.def("AllClient",&CellEntity::CallAllClientMethod)
			.add_property("id",	&CellEntity::GetID)
			.add_property("tid",&CellEntity::GetTypeID)
			.add_property("_x",&CellEntity::GetPosX,&CellEntity::PySetPosX)
			.add_property("_y",&CellEntity::GetPosY,&CellEntity::PySetPosY)
			.add_property("_z",&CellEntity::GetPosZ,&CellEntity::PySetPosZ)
			.add_property("_Y",&CellEntity::GetRotY,&CellEntity::PySetRotY)
			.add_property("_P",&CellEntity::GetRotP,&CellEntity::PySetRotP)
			.add_property("_R",&CellEntity::GetRotR,&CellEntity::PySetRotR)
			.def("SetXYZ",&CellEntity::SetPosXYZ)
			.def("SetYPR",&CellEntity::SetRotYPR)
			;

		class_<CellEntityManagerWarpper >("BaseEntityManager")
			.def("__len__",&CellEntityManagerWarpper::Len)
			.def("__getitem__",&CellEntityManagerWarpper::Get)
			.def("has_key",&CellEntityManagerWarpper::HasKey)
			.def("get",&CellEntityManagerWarpper::Get)
			.def("keys",&CellEntityManagerWarpper::GetKeys)
			.def("values",&CellEntityManagerWarpper::GetValues)
			.def("items",&CellEntityManagerWarpper::GetItems)
			;

		def("entities",sGetEntities);
		def("app",sGetApp);

		class_<CellAppWarpper >("CellApp")
			.def("Stop",		&CellAppWarpper::Stop)
			;
	}
	AUTO_REGISTER_PYTHON_MODULE(Aurora);
}


