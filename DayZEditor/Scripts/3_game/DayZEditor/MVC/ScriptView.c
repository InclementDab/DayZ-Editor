
// Abstract Class
// Inherit from this if you want to create UI elements with 100% script
/*

Example:

class CustomDialogWindow: ScriptView
{
	override string GetLayoutFile() {
		return "MyMod/gui/Layouts/dialogs/Dialog.layout";
	}
}

	CustomDialogWindow window = new CustomDialogWindow();
	window.Show();
	....
	....
	window.Close();

*/

class ScriptView : ScriptedViewBase
{
	protected ref Controller m_Controller;
	Controller GetController()
	{
		return m_Controller;
	}

	// Maybe one day we'll get constructor overloading :)
	void ScriptView()
	{
		m_LayoutRoot = CreateWidget(null);

		LoadViewProperties(this, PropertyTypeHashMap.FromType(Type()), m_LayoutRoot);

		m_LayoutRoot.GetScript(m_Controller);

		// If no Controller is specified in the WB Root
		if (!m_Controller || !m_Controller.IsInherited(Controller))
		{

			Log("Controller not found on %1, creating...", m_LayoutRoot.GetName());
			if (!GetControllerType().IsInherited(Controller))
			{
				Error("%1 is invalid. Must inherit from Controller!", GetControllerType().ToString());
				return;
			}

			if (!Class.CastTo(m_Controller, GetControllerType().Spawn()))
			{
				Error("Could not create Controller %1", GetControllerType().ToString());
				return;
			}

			// Since its not loaded in the WB, needs to be called here
			LoadViewProperties(m_Controller, PropertyTypeHashMap.FromType(GetControllerType()), m_LayoutRoot);
			m_Controller.OnWidgetScriptInit(m_LayoutRoot);
		}

		m_Controller.Debug_Logging = Debug_Logging;
		m_Controller.SetParent(this);
		//m_LayoutRoot.SetHandler(this);
	}

	void ~ScriptView()
	{
		delete m_Controller;

		if (m_LayoutRoot)
		{
			Print("~" + m_LayoutRoot.GetName());
			m_LayoutRoot.Unlink();
			
			if (m_LayoutRoot) 
			{
				delete m_LayoutRoot;
			}
		}
	}

	void SetParent(Widget parent)
	{
		m_LayoutRoot = CreateWidget(parent);
	}

	private	Widget CreateWidget(Widget parent)
	{
		Widget result;
		if (!GetLayoutFile())
		{
			Error("Layout file not found! Are you overriding GetLayoutFile?");
			return result;
		}

		WorkspaceWidget workspace = GetWorkbenchGame().GetWorkspace();
		if (!workspace)
		{
			Error("Workspace was null, try reloading Workbench");
			return result;
		}

		Log("Loading %1", GetLayoutFile());
		result = workspace.CreateWidgets(GetLayoutFile(), parent);
		if (!result)
		{
			Error("Invalid layout file %1", GetLayoutFile());
			return result;
		}

		return result;
	}

	// Useful if you want to set to an existing controller
	void SetController(Controller controller)
	{
		m_Controller = controller;
		m_Controller.Debug_Logging = Debug_Logging;
		m_Controller.OnWidgetScriptInit(m_LayoutRoot);
		m_Controller.SetParent(this);
	}

	// Virtual Methods
	protected string GetLayoutFile();

	protected typename GetControllerType()
	{
		return Controller;
	}
};