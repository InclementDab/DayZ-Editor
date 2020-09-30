enum SingeplayerExecutionType
{
	Server = 0,
	Client,
	Both
}

enum SingleplayerExecutionType
{
	Server = 0,
	Client,
	Both
}

enum CallType
{
	Server = 0,
	Client
}

class RPCMetaWrapper
{
	protected Class m_Instance;
	protected int m_SPExecType;

	void RPCMetaWrapper( Class instance, int spExecType ) 
	{
		m_Instance = instance;
		
		m_SPExecType = spExecType;
	}
	
	Class GetInstance() 
	{
		return m_Instance;
	}
	
	// Determines if the server or client function is what is called in singleplayer mode
	int GetSPExecutionType() 
	{
		return m_SPExecType;
	}
};

class RPCManager
{
	protected float m_Timer;

	protected int m_AmountSent;
	protected int m_AmountSentInUpdate;
	protected int m_AmountSentInSecond;

	protected int m_AmountRecieved;
	protected int m_AmountRecievedInUpdate;
	protected int m_AmountRecievedInSecond;

	protected int m_AmountSuccessRecieved;
	protected int m_AmountSuccessRecievedInUpdate;
	protected int m_AmountSuccessRecievedInSecond;

	protected int m_AmountPreviousSent;
	protected int m_AmountPreviousSentInUpdate;
	protected int m_AmountPreviousSentInSecond;

	protected int m_AmountPreviousRecieved;
	protected int m_AmountPreviousRecievedInUpdate;
	protected int m_AmountPreviousRecievedInSecond;

	protected int m_AmountPreviousSuccessRecieved;
	protected int m_AmountPreviousSuccessRecievedInUpdate;
	protected int m_AmountPreviousSuccessRecievedInSecond;

	protected string m_UpdateChecker;

	protected const int FRAMEWORK_RPC_ID = 10042;
	protected autoptr map< string, ref map< string, ref RPCMetaWrapper > > m_RPCActions;

	protected bool m_RPCManagerEnabled = false;

	void RPCManager()
	{
		m_RPCActions = new map< string, ref map< string, ref RPCMetaWrapper > >;
		//GetLogger().OnUpdate.Insert( OnLogger );

		m_UpdateChecker = "JM_CF_RPC";

		if ( GetGame().IsClient() )
			m_UpdateChecker = "JM_CF_RPC_Client";

		//OnLogger();
	}

	void ~RPCManager()
	{
		GetDayZGame().Event_OnRPC.Remove( OnRPC );
		//GetLogger().OnUpdate.Remove( OnLogger );
		delete m_RPCActions;
	}

	void OnLogger()
	{
		//if ( GetLogger().IsOn( m_UpdateChecker ) )
		//{
		//	GetDayZGame().GetUpdateQueue( CALL_CATEGORY_SYSTEM ).Insert( OnUpdate );
		//} else
		//{
		//	GetDayZGame().GetUpdateQueue( CALL_CATEGORY_SYSTEM ).Remove( OnUpdate );
		//}
	}
	
	void OnUpdate( float timeSlice )
	{
		string str = "";
		bool anyChanged = false;

		if ( m_AmountPreviousSent != m_AmountSent )
		{
			anyChanged = true;
			str = str + "S T: " + m_AmountSent + " | ";
		}
		if ( m_AmountPreviousSentInUpdate != m_AmountSentInUpdate )
		{
			anyChanged = true;
			str = str + "S 1s: " + m_AmountSentInSecond + " | ";
		}
		if ( m_AmountPreviousSentInSecond != m_AmountSentInSecond )
		{
			anyChanged = true;
			str = str + "S " + timeSlice + "s: " + m_AmountSentInUpdate + " | ";
		}

		if ( m_AmountPreviousRecieved != m_AmountRecieved )
		{
			anyChanged = true;
			str = str + "R T: " + m_AmountRecieved + " | ";
		}
		if ( m_AmountPreviousRecievedInUpdate != m_AmountRecievedInUpdate )
		{
			anyChanged = true;
			str = str + "R 1s: " + m_AmountRecievedInSecond + " | ";
		}
		if ( m_AmountPreviousRecievedInSecond != m_AmountRecievedInSecond )
		{
			anyChanged = true;
			str = str + "R " + timeSlice + "s: " + m_AmountRecievedInUpdate + " | ";
		}

		if ( m_AmountPreviousSuccessRecieved != m_AmountSuccessRecieved )
		{
			anyChanged = true;
			str = str + "S R T: " + m_AmountSuccessRecieved + " | ";
		}
		if ( m_AmountPreviousSuccessRecievedInUpdate != m_AmountSuccessRecievedInUpdate )
		{
			anyChanged = true;
			str = str + "S R 1s: " + m_AmountSuccessRecievedInSecond + " | ";
		}
		if ( m_AmountPreviousSuccessRecievedInSecond != m_AmountSuccessRecievedInSecond )
		{
			anyChanged = true;
			str = str + "S R " + timeSlice + "s: " + m_AmountSuccessRecievedInUpdate;
		}

		//if ( anyChanged )
			//GetLogger().Log( str, m_UpdateChecker );

		m_AmountSentInUpdate = 0;
		m_AmountRecievedInUpdate = 0;
		m_AmountSuccessRecievedInUpdate = 0;

		m_Timer += timeSlice;

		if ( m_Timer >= 1.0 )
		{
			m_Timer = 0;

			m_AmountSentInSecond = 0;
			m_AmountRecievedInSecond = 0;
			m_AmountSuccessRecievedInSecond = 0;
		}

		m_AmountPreviousSent = m_AmountSent;
		m_AmountPreviousSentInUpdate = m_AmountSentInUpdate;
		m_AmountPreviousSentInSecond = m_AmountSentInSecond;
		m_AmountPreviousRecieved = m_AmountRecieved;
		m_AmountPreviousRecievedInUpdate = m_AmountRecievedInUpdate;
		m_AmountPreviousRecievedInSecond = m_AmountRecievedInSecond;
		m_AmountPreviousSuccessRecieved = m_AmountSuccessRecieved;
		m_AmountPreviousSuccessRecievedInUpdate = m_AmountSuccessRecievedInUpdate;
		m_AmountPreviousSuccessRecievedInSecond = m_AmountSuccessRecievedInSecond;
	}

	void OnRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	{
		if ( rpc_type != FRAMEWORK_RPC_ID )
		{
			return;
		}
		
		Param2< string, string > metaData;
		if ( !ctx.Read( metaData ) )
		{
			//Error( "Failed reading the RPC metadata!");
			return;
		}

		m_AmountRecieved++;
		m_AmountRecievedInUpdate++;
		m_AmountRecievedInSecond++;
		
		string modName = metaData.param1;
		string funcName = metaData.param2;

		string recievedFrom = "server";

		if ( GetGame().IsServer() && GetGame().IsMultiplayer() )
		{
			if ( sender == NULL )
			{
				recievedFrom = "someone";
			} else 
			{
				recievedFrom = sender.GetPlainId();
			}
		}

		//GetLogger().Log( "Recieved RPC " + modName + "::" + funcName + " from " + recievedFrom + ", target " + target, m_UpdateChecker );
		
		map< string, ref RPCMetaWrapper > functions;
		if ( m_RPCActions.Find( modName, functions ) )
		{
			RPCMetaWrapper wrapper;
			if ( functions.Find( funcName, wrapper ) )
			{
				if ( wrapper.GetInstance() )
				{
					m_AmountSuccessRecieved++;
					m_AmountSuccessRecievedInUpdate++;
					m_AmountSuccessRecievedInSecond++;

					auto functionCallData = new Param4< CallType, ref ParamsReadContext, ref PlayerIdentity, ref Object >( CallType.Server, ctx, sender, target );
				
					if ( ( GetGame().IsServer() && GetGame().IsMultiplayer() ) || ( GetGame().IsServer() && !GetGame().IsMultiplayer() && ( wrapper.GetSPExecutionType() == SingleplayerExecutionType.Server || wrapper.GetSPExecutionType() == SingleplayerExecutionType.Both ) ) ) 
					{
						GetGame().GameScript.CallFunctionParams( wrapper.GetInstance(), funcName, NULL, functionCallData );
					}

					if ( ( GetGame().IsClient() && GetGame().IsMultiplayer() ) || ( GetGame().IsServer() && !GetGame().IsMultiplayer() && ( wrapper.GetSPExecutionType() == SingleplayerExecutionType.Client || wrapper.GetSPExecutionType() == SingleplayerExecutionType.Both ) ) ) 
					{
						//Update call type
						functionCallData.param1 = CallType.Client;
						GetGame().GameScript.CallFunctionParams( wrapper.GetInstance(), funcName, NULL, functionCallData );
					}
				}
			} else
			{
				Error( recievedFrom + " tried sending " + modName + "::<" + funcName + "> which does not seem to exist!");
			}
		} else
		{
			Error( recievedFrom + " tried sending <" + modName + ">::" + funcName + " which does not seem to exist!");
		}
	}

	void SendRPC( string modName, string funcName, ref Param params = NULL, bool guaranteed = false, ref PlayerIdentity sendToIdentity = NULL, ref Object sendToTarget = NULL )
	{
		m_AmountSent++;
		m_AmountSentInUpdate++;
		m_AmountSentInSecond++;
		
		string sendTo = "server";

		if ( GetGame().IsServer() && GetGame().IsMultiplayer() )
		{
			if ( sendToIdentity == NULL )
			{
				sendTo = "everyone";
			} else 
			{
				sendTo = sendToIdentity.GetPlainId();
			}
		}

		//GetLogger().Log( "Sending RPC function " + modName + "::" + funcName + " to " + sendTo + ", target " + sendToTarget, m_UpdateChecker );

		auto sendData = new ref array< ref Param >;
		sendData.Insert( new ref Param2< string, string >( modName, funcName ) );
		sendData.Insert( params );
		
		//In case we are in the singleplayer and the data is consumed twice for both client and server, we need to add it twice. Better than making a deep copy with more complicated rules on receiving
		if ( !GetGame().IsMultiplayer() )
		{
			if ( m_RPCActions.Contains( modName ) )
			{
				if ( m_RPCActions[ modName ].Contains( funcName ) )
				{
					ref RPCMetaWrapper wrapper = m_RPCActions[ modName ][ funcName ];
					
					if ( ( wrapper.GetSPExecutionType() == SingleplayerExecutionType.Both ) )
					{
						sendData.Insert( params );
					}
				}
			}
		}

		GetGame().RPC( sendToTarget, FRAMEWORK_RPC_ID, sendData, guaranteed, sendToIdentity );
	}	

	/**
	 * Warning: Does not support "SingleplayerExecutionType.Both"
	 */
	void SendRPCs( string modName, string funcName, ref array< ref Param > params, bool guaranteed = false, ref PlayerIdentity sendToIdentity = NULL, ref Object sendToTarget = NULL )
	{
		m_AmountSent++;
		m_AmountSentInUpdate++;
		m_AmountSentInSecond++;

		string sendTo = "server";

		if ( GetGame().IsServer() && GetGame().IsMultiplayer() )
		{
			if ( sendToIdentity == NULL )
			{
				sendTo = "everyone";
			} else 
			{
				sendTo = sendToIdentity.GetPlainId();
			}
		}

		//GetLogger().Log( "Sending MRPC function " + modName + "::" + funcName + " to " + sendTo + ", target " + sendToTarget, m_UpdateChecker );

		params.InsertAt( new ref Param2< string, string >( modName, funcName ), 0 );

		GetGame().RPC( sendToTarget, FRAMEWORK_RPC_ID, params, guaranteed, sendToIdentity );

		if ( !GetGame().IsMultiplayer() )
		{
			if ( m_RPCActions.Contains( modName ) )
			{
				if ( m_RPCActions[ modName ].Contains( funcName ) )
				{
					ref RPCMetaWrapper wrapper = m_RPCActions[ modName ][ funcName ];
					
					if ( ( wrapper.GetSPExecutionType() == SingleplayerExecutionType.Both ) )
					{
						Error( modName + "::" + funcName + " does not support \"SingleplayerExecutionType.Both\" when using RPCManager::SendRPCs, use RPCManager::SendRPC instead!");
					}
				}
			}
		}
	}

	bool AddRPC( string modName, string funcName, Class instance, int singlePlayerExecType = SingleplayerExecutionType.Server )
	{
		if ( !m_RPCActions.Contains( modName ) )
		{
			//GetLogger().Log( "Creating RPC mod " + modName, m_UpdateChecker );
			m_RPCActions.Set( modName, new ref map< string, ref RPCMetaWrapper > );
		}
		
		//GetLogger().Log( "Creating RPC function " + modName + "::" + funcName, m_UpdateChecker );
		auto wrapper = new ref RPCMetaWrapper( instance, singlePlayerExecType );
		
		m_RPCActions[ modName ].Set( funcName, wrapper );

		if ( !m_RPCManagerEnabled )
		{
			GetDayZGame().Event_OnRPC.Insert( OnRPC );

			m_RPCManagerEnabled = true;
		}

		return true;
	}
};

static ref RPCManager g_RPCManager;

static ref RPCManager GetRPCManager()
{
	if ( !g_RPCManager )
	{
		g_RPCManager = new ref RPCManager;
	}

	return g_RPCManager;
}
