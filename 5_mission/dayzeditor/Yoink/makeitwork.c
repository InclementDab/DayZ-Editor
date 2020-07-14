class ThisGameIsStr
{
	private Widget 					 m_SavedPresetsWidget;
	protected ref VPPDropDownMenu    m_SavedPresetsDropDown;
	private Widget M_SUB_WIDGET;

	void ThisGameIsStr()
	{
		M_SUB_WIDGET = GetGame().GetWorkspace().CreateWidgets( "AmazingGame.layout");
		m_SavedPresetsWidget = M_SUB_WIDGET.FindAnyWidget( "PresetsDropDown");
		m_SavedPresetsDropDown = new VPPDropDownMenu( m_SavedPresetsWidget, "Saved Item Presets" );
		m_SavedPresetsDropDown.m_OnSelectItem.Insert( OnSelectItemPreset );
	}

	void OnSelectItemPreset(int index)
	{
		m_SavedPresetsDropDown.SetText("I selected index: " + index);
		m_SavedPresetsDropDown.Close();
	}
};