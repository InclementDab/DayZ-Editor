class EditorDropdownPrefab extends ScriptedWidgetEventHandler
{
	protected Widget				m_Root;
	protected ScrollWidget			m_Scroller;
	protected Widget				m_ContentContainer;
	protected ref array<Widget>		m_Content		= new array<Widget>>;
	
	protected Widget				m_Button;
	protected TextWidget			m_Text;
	protected ImageWidget			m_ImageExpand;
	protected ImageWidget			m_ImageCollapse;
	
	protected bool					m_IsExpanded;
	ref ScriptInvoker				m_OnSelectItem	= new ScriptInvoker();

	void EditorDropdownPrefab( Widget root, string text = "" )
	{
		m_Root				= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/dropdown_prefab/dropdown_prefab.layout", root );
		
		m_Scroller			= ScrollWidget.Cast( m_Root.FindAnyWidget( "dropdown_container" ) );
		m_ContentContainer	= m_Root.FindAnyWidget( "dropdown_content" );
		m_Text				= TextWidget.Cast( m_Root.FindAnyWidget( "dropdown_text" ) );
		SetText( text );
		
		m_Button			= m_Root.FindAnyWidget( "dropdown_selector_button" );
		m_ImageExpand		= ImageWidget.Cast( m_Root.FindAnyWidget( "expand_image" ) );
		m_ImageCollapse		= ImageWidget.Cast( m_Root.FindAnyWidget( "collapse_image" ) );
		
		m_Root.SetHandler( this );
		
		RefreshContent();
	}
	
	void RefreshContent()
	{
		Widget child = m_ContentContainer.GetChildren();
		while( child )
		{
			if( m_Content.Find( child ) > -1 )
			{
				m_Content.Insert( child );
			}
		}
		
		m_ContentContainer.Update();
		m_Root.Update();
		
		float x, y;
		m_ContentContainer.GetScreenSize( x, y );
		if( y > m_Scroller.GetContentHeight() )
		{
			m_Scroller.SetAlpha( 1 );
		}
		else
		{
			m_Scroller.SetAlpha( 0 );
		}
	}
	
	int AddElement( string text, Widget content = null )
	{
		ButtonWidget element = ButtonWidget.Cast( GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/dropdown_prefab/dropdown_element.layout", m_ContentContainer ) );
		element.SetText( text );
		
		if( content )
		{
			element.AddChild( content, false );
		}
		m_ContentContainer.Update();
		m_Root.Update();
		
		m_Content.Insert( element );
		return m_Content.Count() - 1;
	}
	
	void RemoveElement( int index )
	{
		if( 0 < index && index < m_Content.Count() )
		{
			delete m_Content.Get( index );
			m_ContentContainer.Update();
			m_Root.Update();
		}
	}
	
	void Close()
	{
		if( m_IsExpanded )
		{
			m_Scroller.Show( false );
			m_ImageExpand.Show( false );
			m_ImageCollapse.Show( true );
		}
	}
	
	void SetText( string text )
	{
		m_Text.SetText( text );
	}
	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		int index = m_Content.Find( w );
		if( index > -1 )
		{
			m_OnSelectItem.Invoke( index );
			m_IsExpanded = false;
			m_Scroller.Show( m_IsExpanded );
			m_ImageExpand.Show( !m_IsExpanded );
			m_ImageCollapse.Show( m_IsExpanded );
			return true;
		}
		return false;
	}
	
	override bool OnMouseButtonUp( Widget w, int x, int y, int button )
	{
		if( w == m_Root && button == MouseState.LEFT )
		{
			m_IsExpanded = !m_IsExpanded;
			m_Scroller.Show( m_IsExpanded );
			m_ImageExpand.Show( !m_IsExpanded );
			m_ImageCollapse.Show( m_IsExpanded );
			
			m_Root.Update();
			return true;
		}
		return false;
	}
}

class EditorExportDialog: ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	protected Widget m_DirectorySelectorPanel;
	protected WrapSpacerWidget m_ListEntryWrapper;
	
	protected ref EditorDropdownPrefab m_Dropdown;
	protected ref array<string> m_Files;
	
	private string m_PathToMissions;
	
	void EditorExportDialog(Widget parent)
	{
		m_Root	= Widget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZEditor/gui/layouts/EditorExportDialog.layout", parent ) );
			
		m_DirectorySelectorPanel = Widget.Cast(m_Root.FindAnyWidget("DirectorySelectorPanel"));
		m_ListEntryWrapper = WrapSpacerWidget.Cast(m_Root.FindAnyWidget("LeftbarSpacer"));
				
		m_Root.SetHandler(this);
		
		InitDialog();
	}
	
	static array< string > FindFilesInLocation( string folder )
	{
		array< string > files = new array< string >;
		string fileName;
		FileAttr fileAttr;
		FindFileHandle findFileHandle = FindFile( folder + "*", fileName, fileAttr, 0 );
		if ( findFileHandle )
		{
			if ( fileName.Length() > 0 && !( fileAttr & FileAttr.DIRECTORY) )
			{
				files.Insert( fileName );
			}
			
			while ( FindNextFile( findFileHandle, fileName, fileAttr ) )
			{
				if ( fileName.Length() > 0 && !( fileAttr & FileAttr.DIRECTORY) )
				{
					files.Insert( fileName );
				}
			}
		}
		CloseFindFile( findFileHandle );
		return files;
	}
	
	void InitDialog()
	{
		m_Dropdown		= new EditorDropdownPrefab( m_DirectorySelectorPanel, "Directory" );
		m_Dropdown.m_OnSelectItem.Insert( OnSelectDirectory );
						
		m_PathToMissions = "missions";//"$saves:\\missions";
		string path_find_pattern = m_PathToMissions + "\\*";	
			
		m_Files = new array<string>;
		m_Files = FindFilesInLocation(path_find_pattern);	
		
		for( int i = 0; i < m_Files.Count(); i++ )
		{
			m_Dropdown.AddElement( m_Files[i] );
		}
	}
	
	void OnSelectDirectory()
	{
	
	}
	
	void Show(bool state)
	{
		m_Root.Show(state);
	}
}