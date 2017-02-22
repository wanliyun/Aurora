

	class_< AttributeOwnerBase >("__AttributeOwnerBase")
		.def("__setattr__",&AttributeOwnerBase::py_setattr)	
		;

	class_< DataTypeFixDict,boost::noncopyable  >("__DataTypeFixDict",init<MetaDataTypeBase *>())
		;

	class_< EntityType,boost::noncopyable >("__EntityType",init<const std::string &>() )
		;
	
	class_< DataTypeFixDict_PyObject,bases<AttributeOwnerBase> >("FIXED_DICT",init<DataTypeFixDict*>() )
			.def("has_key",&DataTypeFixDict_PyObject::HasKey)
			.def("keys",&DataTypeFixDict_PyObject::GetKeys)
			.def("values",&DataTypeFixDict_PyObject::GetValues)
			.def("items",&DataTypeFixDict_PyObject::GetItems)
			.def("__str__",&DataTypeFixDict_PyObject::ToString)
			.def("__repr__",&DataTypeFixDict_PyObject::ToString)
			.def("__getattr__",&DataTypeFixDict_PyObject::py_getattr)
			;

	register_ptr_to_python<EntityTypePtr >();
	register_ptr_to_python<DataTypeFixDictPtr >();
	register_ptr_to_python<DataTypeFixDict_PyObjectPtr >();
