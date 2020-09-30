class JMModuleBase
{
	protected bool m_Enabled;
	protected bool m_PreventInput;
	protected ref set< ref JMModuleBinding > m_Bindings;
	
	void JMModuleBase()
	{
		m_Enabled = true;
		m_PreventInput = false;
		m_Bindings = new set< ref JMModuleBinding >;
	}
	
	void ~JMModuleBase()
	{
	}
	
	void Init()
	{
		RegisterKeyMouseBindings();
	}

	void Toggle()
	{
		m_Enabled = !m_Enabled;
	}

	void PreventInput(bool prevent)
	{
		m_PreventInput = prevent;
	}

	string GetModuleName()
	{
		return ClassName();
	}

	typename GetModuleType()
	{
		return GetModuleName().ToType();
	}

	bool IsEnabled()
	{
		return m_Enabled;
	}

	bool IsPreventingInput()
	{
		return m_PreventInput;
	}

	/**
	 * Can't change dynamically, only used on module init, if false 
	 * and is mission host, the module is destroyed and forgotten.
	 */
	bool IsServer()
	{
		return true;
	}

	/**
	 * Can't change dynamically, only used on module init, if false 
	 * and is mission client, the module is destroyed and forgotten.
	 */
	bool IsClient()
	{
		return true;
	}

	void RegisterKeyMouseBindings() 
	{
	}
	
	/**
	 * Deprecated, use RegisterBinding instead.
	 */
	void RegisterKeyMouseBinding( JMModuleBinding binding ) 
	{
		RegisterBinding( binding );
		Error( GetModuleName() + "::RegisterKeyMouseBinding has been deprecated, use " + GetModuleName() + "::RegisterBinding instead." );
	}

	void RegisterBinding( JMModuleBinding binding ) 
	{
		m_Bindings.Insert( binding );
	}
	
	set< ref JMModuleBinding > GetBindings()
	{
		return m_Bindings;
	}

	// Events 

	/**
	 * Is called every mission frame
	 * 
	 * @timeslice time of previous update
	 */
	void OnUpdate( float timeslice ) 
	{
	}

	void OnSettingsUpdated()
	{
	}

	void OnInit()
	{
	}
	
	void OnMissionStart()
	{
	}
	
	void OnMissionFinish()
	{
	}
	
	void OnMissionLoaded()
	{
	}

	void OnRPC( PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx )
	{
	}

	int GetRPCMin()
	{
		return -1;
	}

	int GetRPCMax()
	{
		return -1;
	}

	/**
	 * @brief See: WorldCleaupEventTypeID
	 */
	void OnWorldCleanup()
	{
	}

	/**
	 * @brief See: MPSessionStartEventTypeID
	 */
	void OnMPSessionStart()
	{
	}

	/**
	 * @brief See: MPSessionPlayerReadyEventTypeID
	 */
	void OnMPSessionPlayerReady()
	{
	}

	/**
	 * @brief See: MPSessionFailEventTypeID
	 */
	void OnMPSessionFail()
	{
	}

	/**
	 * @brief See: MPSessionEndEventTypeID
	 */
	void OnMPSessionEnd()
	{
	}

	/**
	 * @brief See: ConnectingAbortEventTypeID
	 */
	void OnMPConnectAbort()
	{
	}

	/**
	 * @brief See: MPConnectionLostEventTypeID
	 */
	void OnMPConnectionLost( int duration )
	{
	}

	/**
	 * @brief See: RespawnEventTypeID
	 */
	void OnRespawn( int time )
	{
	}

	/**
	 * @brief See: LogoutCancelEventTypeID
	 */
	void OnClientLogoutCancelled( PlayerBase player, PlayerIdentity identity )
	{
	}

	/**
	 * @brief See: MissionServer::InvokeOnConnect
	 */
	void OnInvokeConnect( PlayerBase player, PlayerIdentity identity )
	{
	}

	/**
	 * @brief See: MissionServer::InvokeOnDisconnect
	 */
	void OnInvokeDisconnect( PlayerBase player )
	{
	}

	/**
	 * @brief See: ClientNewEventTypeID
	 */
	void OnClientNew( out PlayerBase player, PlayerIdentity identity, vector pos, ParamsReadContext ctx )
	{
	}

	/**
	 * @brief See: ClientRespawnEventTypeID
	 */
	void OnClientRespawn( PlayerBase player, PlayerIdentity identity )
	{
	}

	/**
	 * @brief See: ClientReadyEventTypeID
	 */
	void OnClientReady( PlayerBase player, PlayerIdentity identity )
	{
	}

	/**
	 * @brief See: ClientPrepareEventTypeID
	 */
	void OnClientPrepare( PlayerIdentity identity, out bool useDB, out vector pos, out float yaw, out int preloadTimeout )
	{
	}

	/**
	 * @brief See: ClientReconnectEventTypeID
	 */
	void OnClientReconnect( PlayerBase player, PlayerIdentity identity )
	{
	}

	/**
	 * @brief See: ClientDisconnectedEventTypeID
	 */
	void OnClientLogout( PlayerBase player, PlayerIdentity identity, int logoutTime, bool authFailed )
	{
	}

	/**
	 * @brief See: MissionServer::PlayerDisconnected - Fires when the player has disconnected from the server (OnClientReconnect won't fire)
	 */
	void OnClientDisconnect( PlayerBase player, PlayerIdentity identity, string uid )
	{
	}

	/**
	 * @brief See: LogoutCancelEventTypeID
	 */
	void OnClientLogoutCancelled( PlayerBase player )
	{
	}
}
