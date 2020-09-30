enum NotificationSystemRPC
{
	INVALID = 9000,
	Create,
	COUNT
};

modded class NotificationSystem
{
	/**
	 * Sending of the notification
	 * 
	 * If on the client it will create the notification, otherwise on the server it sends the notification to the specified client(s)
	 * 
	 * @param title A localised supported way of creating a notification
	 * @param text A localised supported way of creating a notification
	 * @param icon The icon the notification will use
	 * @param color The colour of the notification
	 * @param time How long the notification will stay on the screen for (in seconds)
	 * @param sendTo The player to send the notification to (everyone if NULL)
	 */
	static void Create( ref StringLocaliser title, ref StringLocaliser text, string icon, int color, float time = 3, PlayerIdentity sendTo = NULL )
	{
		//Print("NotificationSystem::Create - Start");
		
		CreateNotification( title, text, icon, color, time, sendTo );
		
		//Print("NotificationSystem::Create - End");
	}

	/**
	 * Sending of the notification
	 * 
	 * If on the client it will create the notification, otherwise on the server it sends the notification to the specified client(s)
	 * 
	 * @param title A localised supported way of creating a notification
	 * @param text A localised supported way of creating a notification
	 * @param icon The icon the notification will use
	 * @param color The colour of the notification
	 * @param time How long the notification will stay on the screen for (in seconds)
	 * @param sendTo The player to send the notification to (everyone if NULL)
	 */
	static void CreateNotification( ref StringLocaliser title, ref StringLocaliser text, string icon, int color, float time = 3, PlayerIdentity sendTo = NULL )
	{
		//Print("NotificationSystem::CreateNotification - Start");
		
		if ( IsMissionHost() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( title );
			rpc.Write( text );
			rpc.Write( icon );
			rpc.Write( color );
			rpc.Write( time );
			rpc.Send( NULL, NotificationSystemRPC.Create, true, sendTo );
		} else
		{
			Exec_CreateNotification( title, text, icon, color, time );
		}
		
		//Print("NotificationSystem::CreateNotification - End");
	}

	/**
	 * Creation of the notification
	 * 
	 * @param title A localised supported way of creating a notification
	 * @param text A localised supported way of creating a notification
	 * @param icon The icon the notification will use
	 * @param color The colour of the notification
	 * @param time How long the notification will stay on the screen for (in seconds)
	 */
	private static void Exec_CreateNotification( ref StringLocaliser title, ref StringLocaliser text, string icon, int color, float time )
	{
		//Print("NotificationSystem::CreateNotification - Start");
		
		ref NotificationRuntimeData data = new NotificationRuntimeData( time, new NotificationData( icon, title.Format() ), text.Format() );
		data.SetColor( color );

		m_Instance.AddNotif( data );
		
		//Print("NotificationSystem::Exec_CreateNotification - End");
	}

	/**
	 * An RPC handler for the data that is to be read from the notification.
	 * 
	 * @param sender Always NULL
	 * @param target Always NULL
	 * @param ctx The data container for the rpc
	 */
	static void RPC_CreateNotification( PlayerIdentity sender, Object target, ref ParamsReadContext ctx )
	{
		//Print("NotificationSystem::RPC_CreateNotification - Start");
		
		ref StringLocaliser title = new StringLocaliser( "" );
		if ( !ctx.Read( title ) )
			return;

		ref StringLocaliser text = new StringLocaliser( "" );
		if ( !ctx.Read( text ) )
			return;

		string icon;
		if ( !ctx.Read( icon ) )
			return;

		int color;
		if ( !ctx.Read( color ) )
			return;

		float time;
		if ( !ctx.Read( time ) )
			return;

		Exec_CreateNotification( title, text, icon, color, time );
		
		//Print("NotificationSystem::RPC_CreateNotification - End");
	}

	/**
	 * Adds the notification to the deferred array or displays it if 
	 * the maximum has not yet been reached.
	 * 
	 * @param data The notification data to be displayed
	 */
	protected void AddNotif( ref NotificationRuntimeData data )
	{
		//Print("NotificationSystem::AddNotif - Start");
		
		if ( m_TimeArray.Count() < MAX_NOTIFICATIONS )
		{
			float time = GetGame().GetTickTime() + data.GetTime();
			data.SetTime( time + NOTIFICATION_FADE_TIME );

			m_TimeArray.Insert( data );
			m_OnNotificationAdded.Invoke( data );
		} 
		else
		{
			m_DeferredArray.Insert( data );
		}
		
		//Print("NotificationSystem::AddNotif - End");
	}

	/**
	 * Vanilla code to get the pre-created notification based on the type
	 * 
	 * Required to create due to how Enforce Script modding works
	 * 
	 * @param type The notification type
	 */
	override NotificationData GetNotificationData( NotificationType type )
	{		
		if ( m_DataArray.Contains( type ) )
		{
			return m_DataArray.Get( type );
		}
		
		return null;
	}


	/**
	 * Vanilla code to get the pre-created notification based on the type
	 * 
	 * Required to create due to how Enforce Script modding works
	 * 
	 * @param type The notification type
	 */
	override static void AddNotification( NotificationType type, float show_time, string detail_text = "" )
	{
		ref NotificationRuntimeData data = new NotificationRuntimeData( show_time, m_Instance.GetNotificationData( type ), detail_text );

		m_Instance.AddNotif( data );
	}

	override static void AddNotificationExtended( float show_time, string title_text, string detail_text = "", string icon = "" )
	{
		ref NotificationData temp_data = new NotificationData( icon, title_text );
		ref NotificationRuntimeData data = new NotificationRuntimeData( show_time, temp_data, detail_text );

		m_Instance.AddNotif( data );
	}
}