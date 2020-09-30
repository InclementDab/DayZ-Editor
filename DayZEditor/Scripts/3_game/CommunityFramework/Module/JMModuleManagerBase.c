class JMModuleManagerBase
{
	protected bool m_PreventModuleBindings;

	void JMModuleManagerBase()
	{
		//GetLogger().Log( "JMModuleManagerBase::JMModuleManagerBase()", "JM_COT_ModuleFramework" );
	}

	void ~JMModuleManagerBase()
	{
		//GetLogger().Log( "JMModuleManagerBase::~JMModuleManagerBase()", "JM_COT_ModuleFramework" );
	}

	bool IsPreventingModuleBindings()
	{
		return m_PreventModuleBindings;
	}

	void SetPreventModuleBindings( bool prevent )
	{
		m_PreventModuleBindings = prevent;
	}

	void InitModules()
	{
		//GetLogger().Log( "JMModuleManagerBase::InitModules()", "JM_COT_ModuleFramework" );
	}

	void Print_DumpModules()
	{
		//GetLogger().Log( "JMModuleManagerBase::Print_DumpModules()", "JM_COT_ModuleFramework" );
	}

	void OnSettingsUpdated()
	{
		//GetLogger().Log( "JMModuleManagerBase::OnSettingsUpdated()", "JM_COT_ModuleFramework" );
	}

	void OnInit()
	{
		//GetLogger().Log( "JMModuleManagerBase::OnInit()", "JM_COT_ModuleFramework" );
	}

	void OnMissionStart()
	{
		//GetLogger().Log( "JMModuleManagerBase::OnMissionStart()", "JM_COT_ModuleFramework" );
	}

	void OnMissionFinish()
	{
		//GetLogger().Log( "JMModuleManagerBase::OnMissionFinish()", "JM_COT_ModuleFramework" );
	}

	void OnMissionLoaded()
	{
		//GetLogger().Log( "JMModuleManagerBase::OnMissionLoaded()", "JM_COT_ModuleFramework" );
	}
	
	void OnRPC( PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx )
	{
	}

	void OnUpdate( float timeslice )
	{
		//GetLogger().Log( "JMModuleManagerBase::OnUpdate()", "JM_COT_ModuleFramework" );
	}

	void OnWorldCleanup()
	{
		//GetLogger().Log( "JMModuleManagerBase::OnWorldCleanup()", "JM_COT_ModuleFramework" );
	}

	void OnMPSessionStart()
	{
		//GetLogger().Log( "JMModuleManagerBase::OnMPSessionStart()", "JM_COT_ModuleFramework" );
	}

	void OnMPSessionPlayerReady()
	{
		//GetLogger().Log( "JMModuleManagerBase::OnMPSessionPlayerReady()", "JM_COT_ModuleFramework" );
	}

	void OnMPSessionFail()
	{
		//GetLogger().Log( "JMModuleManagerBase::OnMPSessionFail()", "JM_COT_ModuleFramework" );
	}

	void OnMPSessionEnd()
	{
		//GetLogger().Log( "JMModuleManagerBase::OnMPSessionEnd()", "JM_COT_ModuleFramework" );
	}

	void OnMPConnectAbort()
	{
		//GetLogger().Log( "JMModuleManagerBase::OnMPConnectAbort()", "JM_COT_ModuleFramework" );
	}

	void OnMPConnectionLost( int duration )
	{
		//GetLogger().Log( "JMModuleManagerBase::OnMPConnectionLost()", "JM_COT_ModuleFramework" );
	}

	void OnRespawn( int time )
	{
		//GetLogger().Log( "JMModuleManagerBase::OnRespawn()", "JM_COT_ModuleFramework" );
	}
}

ref JMModuleManagerBase g_cf_ModuleManager = NULL;

static void DestroyModuleManager()
{
	delete g_cf_ModuleManager;
}

static bool ModuleManagerExists()
{
	return g_cf_ModuleManager != NULL;
}