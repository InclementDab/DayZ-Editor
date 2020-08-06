class EditorUITooltip extends ScriptedWidgetEventHandler
{
	protected Widget				m_Root;
	protected RichTextWidget		m_Title;
	protected Widget				m_Seperator;
	protected RichTextWidget		m_Text;

	protected string				m_TooltipTitle;
	protected string				m_TooltipText;

	protected ref Timer				m_ToolTipUpdateTimer;

	void EditorUITooltip(string text, string title = "")
	{
		m_Root = Widget.Cast( GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/layouts/EditorTooltip.layout") );
		m_Title = RichTextWidget.Cast(m_Root.FindAnyWidget("TooltipTitle"));
		m_Seperator = Widget.Cast(m_Root.FindAnyWidget("Separator"));
		m_Text = RichTextWidget.Cast(m_Root.FindAnyWidget("TooltipText"));
		
		m_TooltipTitle = title;
		m_TooltipText = text;

		m_Root.SetHandler( this );

		HideTooltip();
	}
	
	void ~EditorUITooltip()
	{
		HideTooltip();
		delete m_Root;
	}
		
	void SetToolTip()
	{	
		if (m_TooltipTitle != "")
		{
			m_Title.SetText(m_TooltipTitle);
		}
		else
		{
			m_Title.Show(false);
			m_Seperator.Show(false);
		}

		m_Text.SetText(m_TooltipText);
	}
	
	void SetPos(int x, int y)
	{
		m_Root.SetPos(x, y);
	}
		
	void ShowTooltip()
	{		
		if (!m_ToolTipUpdateTimer)
		{
			m_ToolTipUpdateTimer = new Timer();
			m_ToolTipUpdateTimer.Run( 0.01, this, "Update", NULL, true ); // Call Update all 0.01 seconds
		}
		
		m_Root.Show( true );
		
		SetToolTip();
		
		int x, y;
		GetMousePos(x,y);
		m_Root.SetPos(x, y);
		
		m_Text.Update();
		m_Root.Update();
	}
		
	void HideTooltip()
	{		
		if (m_ToolTipUpdateTimer)
			m_ToolTipUpdateTimer.Stop();
		
		m_Root.Show(false);
		m_Root.SetPos(0, 0);
		m_Title.SetText("");
		m_Text.SetText("");
	}
	
	void SetColor(int color)
	{		
		m_Root.SetColor( color );
	}
	
	void SetTextColor(int color)
	{		
		m_Title.SetColor( color );
		m_Text.SetColor( color );
	}
	
	void Update( float timeslice )
	{		
		if ( m_Root && m_Root.IsVisible() ) 
			UpdateTooltip();
	}
	
	void UpdateTooltip()
	{
		int mouse_x;
		int mouse_y;
			
		GetGame().GetMousePos( mouse_x, mouse_y );
		m_Root.SetPos( mouse_x, mouse_y );
		m_Root.Update();
	}
	
	bool IsVisable()
	{
		return m_Root.IsVisible();
	}
}