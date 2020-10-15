/*
class EditorListItemTooltip extends ScriptedWidgetEventHandler
{
	private Widget m_Root;
	private TextWidget m_Title;
	private ItemPreviewWidget m_Preview;

	private ref Timer	m_ToolTipUpdateTimer;
	private ref EditorPlaceableObjectData m_PlaceableObject;
	private EntityAI m_PreviewObj;
	
	void EditorListItemTooltip(EditorPlaceableObjectData obj)
	{
		#ifdef EDITORPRINT
		EditorPrint("EditorListItemTooltip::EditorListItemTooltip - Start");
		#endif
		
		m_Root = Widget.Cast( GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/layouts/EditorListItemTooltip.layout") );
		m_Title = TextWidget.Cast(m_Root.FindAnyWidget("EditorListItemTooltipTitle"));
		m_Preview = ItemPreviewWidget.Cast(m_Root.FindAnyWidget("EditorListItemTooltipPreview"));

		m_PlaceableObject = obj;
		
		m_Root.SetHandler( this );
		
		#ifdef EDITORPRINT
		EditorPrint("EditorListItemTooltip::EditorListItemTooltip - End");
		#endif
	}
	
	void ~EditorListItemTooltip()
	{
		HideTooltip();
		delete m_Root;
	}
		
	void SetToolTip()
	{
		#ifdef EDITORPRINT
		EditorPrint("EditorListItemTooltip::SetToolTip - Start");
		#endif
		
		if(m_PlaceableObject)
		{
			m_Title.SetText(m_PlaceableObject.GetType());
			m_PreviewObj = EntityAI.Cast(GetGame().CreateObjectEx(m_PlaceableObject.GetType(), vector.Zero, ECE_NONE));
			
			#ifdef EDITORPRINT
			EditorPrint("EditorListItemTooltip::SetToolTip - m_PreviewObj: " + m_PreviewObj);
			#endif
			
			m_Preview.SetItem(m_PreviewObj);
			m_Preview.SetView(m_Preview.GetItem().GetViewIndex());
		}
		
		#ifdef EDITORPRINT
		EditorPrint("EditorListItemTooltip::SetToolTip - End");
		#endif
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
		
		m_Root.Update();
	}
		
	void HideTooltip()
	{		
		if (m_ToolTipUpdateTimer)
			m_ToolTipUpdateTimer.Stop();
		
		m_Root.Show(false);
		m_Root.SetPos(0, 0);
		//m_Title.SetText("");
		
		if(m_PreviewObj)
		{
			m_Preview.SetItem(null);
			GetGame().ObjectDelete(m_PreviewObj);
		}
	}
	
	void SetColor(int color)
	{		
		m_Root.SetColor( color );
	}
	
	void SetTextColor(int color)
	{		
		m_Title.SetColor( color );
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
*/