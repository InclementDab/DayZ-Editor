
// Essentially the same as ScriptView except GetType is now a Template
class ScriptViewTemplate<Class T> : ScriptView
{
	protected T m_TemplateController;

	T GetTemplateController()
	{
		return m_TemplateController;
	}

	void ScriptViewTemplate()
	{
		if (!Class.CastTo(m_TemplateController, m_Controller))
		{
			Error("Error in casting of Template Controller %1", T.ToString());
			return;
		}
	}

	private	override typename GetControllerType()
	{
		return T;
	}
}