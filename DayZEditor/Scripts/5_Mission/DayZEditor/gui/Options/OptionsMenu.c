modded class TabberUI
{
	protected ref map<string, int> m_TabsByName = new map<string, int>();
	
	override int AddTab(string name)
	{
		int id = super.AddTab(name);
		name.ToLower();
		m_TabsByName[name] = id;
		return id;	
	}
	
	int FindTabIndex(string name)
	{
		name.ToLower();
		if (!m_TabsByName.Contains(name)) {
			return -1;
		}
		
		return m_TabsByName[name];
	}
}

modded class OptionsMenu
{
	int FindTabIndex(string tab_name)
	{
		return m_Tabber.FindTabIndex(tab_name);
	}
	
	void SetOpenTab(int tab_index)
	{
		if (m_Tabber) {
			m_Tabber.PerformSwitchTab(tab_index);
		}
	}
}