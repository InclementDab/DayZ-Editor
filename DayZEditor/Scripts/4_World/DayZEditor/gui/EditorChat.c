// #include "Scripts\Classes\Gui\ChatLine.c"

/*!
	channel type, possible values
	CCSystem(1)
	CCAdmin(2)
	CCDirect(4)
	CCMegaphone(8)
	CCTransmitter(16)
	CCPublicAddressSystem(32)
	CCBattlEye(64)
*/

// sorry.
class EditorChatLine
{
	// Consts
	const float FADE_TIMEOUT = 30;
	const float FADE_OUT_DURATION = 3;
	const float FADE_IN_DURATION = 0.5;
	
	private const string RADIO_PREFIX = "(" + "#str_radio" + ") ";
	private const string GAME_PREFIX = "(" + "#layout_chat_game" + ") ";
	private const string ADMIN_PREFIX = "(" + "#STR_MP_MASTER" + ") ";
	private const int 	 DEFAULT_COLOUR = ARGB(255, 255, 255, 255);
	private const int 	 GAME_TEXT_COLOUR = ARGB(255, 255, 0, 0);
	private const int 	 ADMIN_TEXT_COLOUR = ARGB(255, 255, 255, 0);
	
	// Widgets
	Widget							m_RootWidget;
	TextWidget						m_NameWidget;
	TextWidget						m_TextWidget;
	
	private ref WidgetFadeTimer 	m_FadeTimer;
	private ref Timer 				m_TimeoutTimer;

	void EditorChatLine(Widget root_widget)
	{
		m_RootWidget	= GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_chat_item.layout", root_widget);
	
		m_NameWidget	= TextWidget.Cast( m_RootWidget.FindAnyWidget( "ChatItemSenderWidget" ) );
		m_TextWidget	= TextWidget.Cast( m_RootWidget.FindAnyWidget( "ChatItemTextWidget" ) );
		
		m_FadeTimer		= new WidgetFadeTimer;
		m_TimeoutTimer	= new Timer(CALL_CATEGORY_GUI);
	}

	void ~EditorChatLine()
	{
        // Stop timers before destroying widgets
        if (m_FadeTimer) m_FadeTimer.Stop();
        if (m_TimeoutTimer) m_TimeoutTimer.Stop();

        // Safe deletion
		if (m_TextWidget) delete m_TextWidget;
        // m_RootWidget is managed, best to unlink or let parent destroy
        if (m_RootWidget) m_RootWidget.Unlink(); 
	}

	void Set(ChatMessageEventParams params)	// Param 1 --> Channel, Param 2 --> sender name, Param 3 --> message, Param 4 ?? 
	{
		int channel; 
		
		m_NameWidget.SetText("");
		m_TextWidget.SetText("");
		
		SetColour(DEFAULT_COLOUR);
		m_RootWidget.Show(true);
		
		channel = params.param1; // 0 = Survior   1 = Game/System   2 = Admin 

		/*
		if (params.param2 != "")
		{
			m_NameWidget.SetText( params.param2 + " : "); 
		}
		
		if( channel & CCSystem )
 		{
			if(params.param2 != "")
			{
				m_NameWidget.SetText(GAME_PREFIX + ": " );
			} 
			SetColour(GAME_TEXT_COLOUR);
 		}
		else if( channel & CCAdmin )
		{
			m_NameWidget.SetText(ADMIN_PREFIX + ": ");
			SetColour(ADMIN_TEXT_COLOUR);			
		}
		else if( channel & CCTransmitter )
		{
			m_NameWidget.SetText(RADIO_PREFIX + params.param2 + " : ");
		}		
		*/
		
		if ( channel & CCSystem )
		{
			// Game
			if (params.param2 != "")
			{
				m_NameWidget.SetText(GAME_PREFIX + ": " );
			}
			
			SetColorByParam(params.param4, GAME_TEXT_COLOUR);
		}
		else if ( channel & CCAdmin )
		{
			// Admin
			m_NameWidget.SetText(ADMIN_PREFIX + ": ");
			SetColorByParam(params.param4, ADMIN_TEXT_COLOUR);
		}
		else if ( channel & CCTransmitter )
		{
			// Radio - Trasnmitter
			m_NameWidget.SetText(RADIO_PREFIX + params.param2 + " : ");
		}
		else if ( channel == 0 || channel & CCDirect )
		{
			// Player
			if (params.param2 != "")
			{
				m_NameWidget.SetText(params.param2 + " : ");
			}
		}
		
		
		m_TextWidget.SetText(params.param3);		
		
		m_FadeTimer.FadeIn(m_RootWidget, FADE_IN_DURATION);
		m_TimeoutTimer.Run(FADE_TIMEOUT, m_FadeTimer, "FadeOut", new Param2<Widget, float>(m_RootWidget, FADE_OUT_DURATION));
	}
	
	private void SetColour(int colour)
	{
		m_NameWidget.SetColor(colour);
		m_TextWidget.SetColor(colour);
	}
	
	protected void SetColorByParam(string pParamValue, int pFallback)
	{
		if (pParamValue != "")
		{
			SetColour(ColorNameToColor(pParamValue));
			return;
		}
		
		SetColour(pFallback);
	}

	void Clear()
	{
		m_RootWidget.Show( false );
		m_TimeoutTimer.Stop();
		m_FadeTimer.Stop();
	}
	
	protected int ColorNameToColor(string pColorName)
	{
		int color = 0xFFFFFFFF;
		switch (pColorName)
		{
		case "colorStatusChannel":
			color = COLOR_BLUE;
		break;
		case "colorAction":
			color = COLOR_YELLOW;
		break;
		case "colorFriendly":
			color = COLOR_GREEN;
		break;
		case "colorImportant":
			color = COLOR_RED;
		break;
		}
		
		return color;
	}
}

class EditorChat
{
	const int LINE_COUNT = 12;

	protected Widget					m_RootWidget;
	protected int						m_LineHeight;
	protected int						m_LastLine;
	protected ref array<ref EditorChatLine>	m_Lines;

	void EditorChat()
	{
		m_Lines = new array<ref EditorChatLine>;
	}

	void ~EditorChat()
	{
		Destroy();
	}
	
	void Init(Widget root_widget)
	{
		Destroy();

		m_RootWidget = root_widget;

		if (m_RootWidget)
		{
			float w;
			float h;
			m_RootWidget.GetSize(w,h);
			m_LineHeight = h / LINE_COUNT;
			m_LastLine = 0;

			for (int i = 0; i < LINE_COUNT; i++)
			{
				EditorChatLine line = new EditorChatLine(m_RootWidget);
				m_Lines.Insert(line);
			}
		}
	}

	void Destroy()
	{
        if (m_Lines)
        {
		    m_Lines.Clear();
        }
	}
	
	void Clear()
	{
		for (int i = 0; i < LINE_COUNT; i++)
		{
			m_Lines.Get(i).Clear();
		}
	}
	
	void Add(ChatMessageEventParams params)
	{
		int max_lenght = ChatMaxUserLength;
		int name_lenght = params.param2.Length();
		int text_lenght = params.param3.Length();
		int total_lenght = text_lenght + name_lenght;
		int channel =  params.param1;

		if( channel & CCSystem || channel & CCBattlEye) //TODO separate battleye bellow
 		{
			if( g_Game.GetProfileOption( EDayZProfilesOptions.GAME_MESSAGES ) )
				return;
			
			max_lenght = ChatMaxSystemLength; // system messages can be longer
 		}
		//TODO add battleye filter to options
		/*else if( channel & CCBattlEye ) 
		{
			if( g_Game.GetProfileOption( EDayZProfilesOptions.BATTLEYE_MESSAGES ) )
				return;
		}*/
		else if( channel & CCAdmin )
		{
			if( g_Game.GetProfileOption( EDayZProfilesOptions.ADMIN_MESSAGES ) )
				return;
		}
		else if( channel & CCDirect || channel & CCMegaphone || channel & CCTransmitter || channel & CCPublicAddressSystem ) 
		{
			if( g_Game.GetProfileOption( EDayZProfilesOptions.PLAYER_MESSAGES ) )
				return;
		}
		else if( channel != 0 ) // 0 should be local messages to self
		{
			Print("Chat: Unknown channel " + channel);
			return;
		}
		
		if (total_lenght > max_lenght)
		{
			int pos = 0;
			int lenght = Math.Clamp(max_lenght - name_lenght, 0, text_lenght);
			ChatMessageEventParams tmp = new ChatMessageEventParams(params.param1, params.param2, "", params.param4);
			
			while (pos < text_lenght)
			{
				tmp.param3 = params.param3.Substring(pos, lenght);
				AddInternal(tmp);
				
				tmp.param2 = "";
				pos += lenght;
				lenght = Math.Clamp(text_lenght - pos, 0, max_lenght);			
			}
		}
		else
		{
			AddInternal(params);
		}
	}
	
	void AddInternal(ChatMessageEventParams params)
	{
		m_LastLine = (m_LastLine + 1) % m_Lines.Count();

		EditorChatLine line = m_Lines.Get(m_LastLine);
		line.Set(params);

		for (int i = 0; i < m_Lines.Count(); i++)
		{
			line = m_Lines.Get((m_LastLine + 1 + i) % LINE_COUNT);
			line.m_RootWidget.SetPos(0, i * m_LineHeight);
			
			float x = 0;
			float y = 0;
			
			line.m_RootWidget.GetPos(x, y);
		}
	}
}
