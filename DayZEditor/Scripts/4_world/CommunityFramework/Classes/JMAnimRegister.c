/*
modded class ModItemRegisterCallbacks
{
	ref JMAnimRegister m_AnimRegister;

	void ModItemRegisterCallbacks()
	{
		m_AnimRegister = new JMAnimRegister();
	}

	void ~ModItemRegisterCallbacks()
	{
		delete m_AnimRegister;
	}

	override void RegisterEmptyHanded( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.RegisterEmptyHanded( pType, pBehavior );

		m_AnimRegister.OnRegisterEmptyHanded( pType, pBehavior );
		m_AnimRegister.RegisterEmptyHanded( pType, pBehavior );
	}

    override void RegisterOneHanded( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.RegisterOneHanded( pType, pBehavior );

		m_AnimRegister.OnRegisterOneHanded( pType, pBehavior );
		m_AnimRegister.RegisterOneHanded( pType, pBehavior );
	}

    override void RegisterTwoHanded( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.RegisterTwoHanded( pType, pBehavior );

		m_AnimRegister.OnRegisterTwoHanded( pType, pBehavior );
		m_AnimRegister.RegisterTwoHanded( pType, pBehavior );
	}

    override void RegisterRestrained( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.RegisterRestrained( pType, pBehavior );

		m_AnimRegister.OnRegisterRestrained( pType, pBehavior );
		m_AnimRegister.RegisterRestrained( pType, pBehavior );
	}

    override void RegisterSurrender( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.RegisterSurrender( pType, pBehavior );

		m_AnimRegister.OnRegisterSurrender( pType, pBehavior );
		m_AnimRegister.RegisterSurrender( pType, pBehavior );
	}

    override void RegisterHeavy( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.RegisterHeavy( pType, pBehavior );

		m_AnimRegister.OnRegisterHeavy( pType, pBehavior );
		m_AnimRegister.RegisterHeavy( pType, pBehavior );
	}

    override void RegisterPistol( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.RegisterPistol( pType, pBehavior );

		m_AnimRegister.OnRegisterPistol( pType, pBehavior );
		m_AnimRegister.RegisterPistol( pType, pBehavior );
	}

    override void RegisterFireArms( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.RegisterFireArms( pType, pBehavior );

		m_AnimRegister.OnRegisterFireArms( pType, pBehavior );
		m_AnimRegister.RegisterFireArms( pType, pBehavior );
	}

    override void RegisterArcheryItem( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.RegisterArcheryItem( pType, pBehavior );

		m_AnimRegister.OnRegisterArcheryItem( pType, pBehavior );
		m_AnimRegister.RegisterArcheryItem( pType, pBehavior );
	}

    override void RegisterLauncher( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.RegisterLauncher( pType, pBehavior );

		m_AnimRegister.OnRegisterLauncher( pType, pBehavior );
		m_AnimRegister.RegisterLauncher( pType, pBehavior );
	}

	override void RegisterCustom( DayZPlayerType pType )
	{
		super.RegisterCustom( pType );

		m_AnimRegister.OnRegisterCustom( pType, pBehavior );
		m_AnimRegister.RegisterCustom( pType, pBehavior );
	}

	override void CustomBoneRemapping( DayZPlayerType pType )
	{
		super.CustomBoneRemapping( pType );

		m_AnimRegister.OnBoneRemapping( pType );
		m_AnimRegister.BoneRemapping( pType );
	}
};
*/

// 107 compatibility
class JMAnimRegister
{
	static JMAnimRegister Register( DayZPlayerType pType )
	{
		JMAnimRegister animReg = new JMAnimRegister;

		DayzPlayerItemBehaviorCfg behaviour;

		behaviour = new DayzPlayerItemBehaviorCfg;
		behaviour.SetEmptyHanded();
		animReg.OnRegisterEmptyHanded( pType, behaviour );
		animReg.RegisterEmptyHanded( pType, behaviour );
		
		behaviour = new DayzPlayerItemBehaviorCfg;
		behaviour.SetTwoHanded();
		animReg.OnRegisterTwoHanded( pType, behaviour );
		animReg.RegisterTwoHanded( pType, behaviour );
		
		behaviour = new DayzPlayerItemBehaviorCfg;
		behaviour.SetToolsOneHanded();
		animReg.OnRegisterOneHanded( pType, behaviour );
		animReg.RegisterOneHanded( pType, behaviour );
		
		behaviour = new DayzPlayerItemBehaviorCfg;
		behaviour.SetRestrained();
		animReg.OnRegisterRestrained( pType, behaviour );
		animReg.RegisterRestrained( pType, behaviour );
		
		behaviour = new DayzPlayerItemBehaviorCfg;
		behaviour.SetSurrender();
		animReg.OnRegisterSurrender( pType, behaviour );
		animReg.RegisterSurrender( pType, behaviour );
		
		behaviour = new DayzPlayerItemBehaviorCfg;
		behaviour.SetHeavyItems();
		animReg.RegisterHeavy( pType, behaviour );

		behaviour = new DayzPlayerItemBehaviorCfg;
		behaviour.SetPistols();
		animReg.OnRegisterPistol( pType, behaviour );
		animReg.RegisterPistol( pType, behaviour );
		
		behaviour = new DayzPlayerItemBehaviorCfg;
		behaviour.SetFirearms();
		animReg.OnRegisterFireArms( pType, behaviour );
		animReg.RegisterFireArms( pType, behaviour );

		behaviour = new DayzPlayerItemBehaviorCfg;
		animReg.OnRegisterArcheryItem( pType, behaviour );
		animReg.RegisterArcheryItem( pType, behaviour );

		behaviour = new DayzPlayerItemBehaviorCfg;
		animReg.OnRegisterLauncher( pType, behaviour );
		animReg.RegisterLauncher( pType, behaviour );

		animReg.OnRegisterCustom( pType );
		animReg.RegisterCustom( pType );

		animReg.OnBoneRemapping( pType );
		animReg.BoneRemapping( pType );

		return animReg;
	}

	void RegisterEmptyHanded( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior );
	void OnRegisterEmptyHanded( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior );
	void RegisterOneHanded( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior );
	void OnRegisterOneHanded( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior );
	void RegisterTwoHanded( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior );
	void OnRegisterTwoHanded( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior );
	void RegisterRestrained( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior );
	void OnRegisterRestrained( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior );
	void RegisterSurrender( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior );
	void OnRegisterSurrender( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior );
	void RegisterHeavy( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior );
	void OnRegisterHeavy( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior );
	void RegisterPistol( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior );
	void OnRegisterPistol( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior );
	void RegisterFireArms( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior );
	void OnRegisterFireArms( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior );
	void RegisterArcheryItem( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior );
	void OnRegisterArcheryItem( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior );
	void RegisterLauncher( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior );
	void OnRegisterLauncher( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior );
	void RegisterCustom( DayZPlayerType pType );
	void OnRegisterCustom( DayZPlayerType pType );
	void BoneRemapping( DayZPlayerType pType );
	void OnBoneRemapping( DayZPlayerType pType );
};