modded class ScriptView
{
	void ScriptView()
	{
		if (m_LayoutRoot) {
			m_LayoutRoot.SetUserData(this);
		}
	}
}