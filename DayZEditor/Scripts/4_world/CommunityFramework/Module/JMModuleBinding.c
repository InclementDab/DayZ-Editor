class JMModuleBinding
{
	protected string m_CallbackFunction;

	protected string m_UAInputName;

	protected bool m_CanBeUsedInMenu;
	
	void JMModuleBinding( string callback, string input, bool menu = false ) 
	{		
		m_CallbackFunction = callback;

		m_UAInputName = input;

		m_CanBeUsedInMenu = menu;
	}
	
	bool CanBeUsedInMenu() 
	{
		return m_CanBeUsedInMenu;
	}
	
	string GetCallBackFunction() 
	{
		return m_CallbackFunction;
	}

	string GetUAInputName()
	{
		return m_UAInputName;
	}
}