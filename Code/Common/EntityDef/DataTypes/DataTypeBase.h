#ifndef _DATA_TYPE_BASE_H
#define _DATA_TYPE_BASE_H


namespace Aurora
{
	class MetaDataTypeBase;
	class AttributeOwnerBase;
	class DataTypeBase;
	typedef boost::shared_ptr<DataTypeBase> DataTypeBasePtr;

	class DataTypeBase
	{
	public:
		DataTypeBase(MetaDataTypeBase * pMetaType);
		virtual ~DataTypeBase(){}
	private:
		DataTypeBase(const DataTypeBase & );
		DataTypeBase & operator=(const DataTypeBase &);
	public:
		virtual void AddToMD5(MD5 & md5)const;
		
		virtual AttributeID GetChildCount()const{return 0;}
		virtual DataTypeBasePtr GetChildTypeByID(AttributeID nIndex)const{return DataTypeBasePtr(0);}

		virtual void OnAttach(bpy::object obj,AttributeOwnerBase * pOwner,uint32 indexInOwner){}
		virtual void OnDetach(bpy::object obj){}

		virtual bpy::object NewObj(AttributeFlagT flag)const =0;
		virtual bpy::object NewObj(BinaryIStream & iStream)const =0;

		virtual bool WriteToStream(BinaryOStream & oStream,bpy::object & obj,AttributeFlagT flag)const = 0;

		virtual AttributeOwnerBase * AsAttributeOwner(bpy::object obj){return  0;}
	public:
		static const AttributeFlagT EFlag_OtherClient	=	AttributeFlagT(BIT_(0));
		static const AttributeFlagT EFlag_OwnClient		=	AttributeFlagT(BIT_(1));
		static const AttributeFlagT EFlag_GhostCell		=	AttributeFlagT(BIT_(2));
		static const AttributeFlagT EFlag_RealCell		=	AttributeFlagT(BIT_(3));
		static const AttributeFlagT EFlag_OwnbyCell		=	AttributeFlagT(BIT_(4));
		static const AttributeFlagT EFlag_WriteDB		=	AttributeFlagT(BIT_(5));

		static const AttributeFlagT EFlag_All	= EFlag_OtherClient 
												| EFlag_OwnClient
												| EFlag_GhostCell
												| EFlag_RealCell
												| EFlag_OwnbyCell
												| EFlag_WriteDB;

		static const uint32 MAX_FLAGS	= 6;

		bool SetFlag(const std::string & strFlag);

		inline void SetFlag(AttributeFlagT flg){m_flag |= flg;}
		inline bool HasAnyFlag(AttributeFlagT flg)const{return (m_flag & flg) != 0;}
		inline bool HasAllFlag(AttributeFlagT flg)const{return (m_flag & flg) == flg;}

		inline bool IsOtherClient()const{return HasAnyFlag(EFlag_OtherClient);}
		inline bool IsOwnClient()const{return HasAnyFlag(EFlag_OwnClient);}

		inline bool IsGhostCell()const{return HasAnyFlag(EFlag_GhostCell);}
		inline bool IsRealCell()const{return HasAnyFlag(EFlag_RealCell);}

		inline bool IsCellOwn()const{return HasAnyFlag(EFlag_OwnbyCell);}
		inline bool IsWriteDB()const{return HasAnyFlag(EFlag_WriteDB);}


		inline bool IsBaseOnly()const{return m_flag == 0;}
		inline bool IsRealOnly()const{return HasAllFlag(EFlag_RealCell | EFlag_OwnbyCell);}

		inline const std::string & GetRefName()const{return m_strRefName;}
		inline void SetRefName(const std::string & strName){m_strRefName = strName;}

		inline AttributeID GetRefId()const{return m_nRefIndex;}
		inline void SetRefId(AttributeID id){m_nRefIndex = id;}

		void SetDftValue(const std::string & str){m_strDftValue = str;}
	public:
		static bool Init(const std::string & strAliasTypeFile);
		static void Fini();

		static DataTypeBasePtr GetType(const std::string & strTypeName);
		static DataTypeBasePtr CreateType(const std::string & strTypeName,tinyxml2::XMLHandle xml);
	protected:
		MetaDataTypeBase *	m_pMetaType;	//不拥有该指针;

		std::string			m_strRefName;
		AttributeID			m_nRefIndex;
		
		std::string			m_strDftValue;
		AttributeFlagT		m_flag;
	private:
		typedef std::map<std::string,boost::shared_ptr<DataTypeBase> >  MapTypesT;
		static MapTypesT s_mapTypes;	//拥有这些指针;
	};

	typedef boost::shared_ptr<DataTypeBase> DataTypeBasePtr;

	typedef std::vector< DataTypeBasePtr > VDataTypeT;
	typedef std::map<std::string,AttributeID> Name2IDMapT;
	typedef std::map<AttributeID,std::string> ID2NameMapT;
}

#endif
