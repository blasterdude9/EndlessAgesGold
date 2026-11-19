#ifndef CDOTCLASS_H
#define CDOTCLASS_H

//Enumerate all our DoT types
enum DotTypes
{
	DOTTYPE_DO_DAMAGE = 0,
	DOTTYPE_AMP_DAMAGE,
	DOTTYPE_LOWER_DAMAGE,
	DOTTYPE_LOWER_MAXHP,
	DOTTYPE_LOWER_MAXMP,
	DOTTYPE_NULL_JETS,
	DOTTYPE_NULL_SHIELDS,
	DOTTYPE_DRAIN_HP,
	DOTTYPE_DRAIN_MP,
	DOTTYPE_MAX
};

//New class for the DoT (Damage Over Time) system
class CDotClass : public CObject
{
	DECLARE_SERIAL(CDotClass);

	public:
		char		m_dotType;			//Type of DoT being applied
		char		m_dotChannel;		//Channel the DoT is affecting
		char		m_dotApplyPer;		//Percentage chance of DoT being applied
		int			m_dotDuration;		//How long the DoT lasts
		int			m_dotEffectRef;		//Visual effect reference
		int			m_dotIndex;
		short		m_dotAmount;		//Amount that the DoT is providing
		short		m_dotPlayerID;		//Who applied the DoT
		CString		m_dotName;			//Name of our DoT
		DWORD		m_dotTimeStamp;		//Timestamp of our DoT

		CDotClass();
		void SafeDelete();
		void Clone(CDotClass **copy);
		void Serialize(CArchive &ar);
};

//List of our DoTs
class CDotClassList : public CObList
{
	DECLARE_SERIAL(CDotClassList);	
	
	public:
		unsigned char m_dotLimit;
		unsigned char m_currentDotAmount;

		CDotClassList();
		void Clone(CDotClassList **copy);
		void SafeDelete();

		void AddDotObject(CDotClass *dotInfo);
		void RemoveDotCheckByDuration();
		BOOL RemoveDotByType(int type);
		BOOL RemoveDotByChannel(int channel);
		void RemoveAllDots();
		void RemoveDotByID(int id);
		CDotClass *GetDotByID(int dotIndex);
};

#endif