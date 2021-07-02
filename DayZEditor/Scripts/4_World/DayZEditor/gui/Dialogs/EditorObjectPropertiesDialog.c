class EditorObjectPropertiesDialog: EditorDialogBase
{
	static bool GeneralGroup = true;
	static bool Object_Group = true;
	static bool HumanGroup = true;
	static bool AdvancedGroup = false;
	
	protected ref GroupPrefab m_GeneralGroup;
	protected ref GroupPrefab m_ObjectGroup;
	protected ref GroupPrefab m_HumanGroup;
	protected ref GroupPrefab m_AdvancedGroup;
	
	protected EditorObject m_EditorObject;
	
	void EditorObjectPropertiesDialog(string title, EditorObject editor_object)
	{		
		SetEditorObject(editor_object);
				
		AddButton(DialogResult.OK);
		
		EditorEvents.OnObjectSelected.Insert(OnObjectSelected);
		EditorEvents.OnObjectDeselected.Insert(OnObjectDeselected);
	}
	
	void ~EditorObjectPropertiesDialog()
	{
		UpdateViewContext();
	}
	
	private void OnObjectSelected(Class context, EditorObject editor_object)
	{
		SetEditorObject(editor_object);
	}
	
	private void OnObjectDeselected(Class context, EditorObject editor_object)
	{
		UpdateViewContext();
	}
	
	// Maximum Elegance
	void SetEditorObject(EditorObject editor_object)
	{
		m_EditorObject = editor_object;
		
		UpdateViewContext();
		
		m_GeneralGroup = new GroupPrefab("General", m_EditorObject, string.Empty);
		m_GeneralGroup.Insert(new CheckBoxPrefab("Show", m_EditorObject, "Show"));
		m_GeneralGroup.Insert(new EditBoxPrefab("Name", m_EditorObject, "Name"));
		m_GeneralGroup.Insert(new VectorPrefab("Position", m_EditorObject, "Position"));
		m_GeneralGroup.Insert(new VectorPrefab("Orientation", m_EditorObject, "Orientation"));
		m_GeneralGroup.Insert(new EditBoxNumberPrefab("Scale", m_EditorObject, "Scale", 0.01));
		//m_GeneralGroup.Insert(new CheckBoxPrefab("Editor Only", m_EditorObject, "EditorOnly", m_EditorObject.EditorOnly));
		m_GeneralGroup.Open(GeneralGroup);
		
		m_ObjectGroup = new GroupPrefab("Object Settings", m_EditorObject, string.Empty);
		m_ObjectGroup.Insert(new EditBoxNumberPrefab("Health", m_EditorObject, "Health"));
		m_ObjectGroup.Insert(new CheckBoxPrefab("Enable Simulation", m_EditorObject, "Simulate"));
		m_ObjectGroup.Insert(new CheckBoxPrefab("Lock", m_EditorObject, "Locked"));
		m_ObjectGroup.Insert(new CheckBoxPrefab("Enable Physics", m_EditorObject, "Physics"));
		m_ObjectGroup.Insert(new CheckBoxPrefab("Allow Damage", m_EditorObject, "AllowDamage"));
		m_ObjectGroup.Open(Object_Group);
		
		m_AdvancedGroup = new GroupPrefab("Debug", m_EditorObject, string.Empty);
		m_AdvancedGroup.Insert(new TextBoxPrefab("Classname", m_EditorObject, "")); //m_EditorObject.GetWorldObject().GetType()
		string id = "" + m_EditorObject.GetWorldObject().GetID();
		m_AdvancedGroup.Insert(new TextBoxPrefab("ID", m_EditorObject, "id"));
		m_AdvancedGroup.Insert(new TextBoxPrefab("Model", m_EditorObject, "model")); //m_EditorObject.GetWorldObject().ConfigGetString("model")
		m_AdvancedGroup.Open(AdvancedGroup);
		
		AddContent(m_GeneralGroup);
		AddContent(m_ObjectGroup);
		AddContent(m_AdvancedGroup);
		
		if (editor_object.GetWorldObject().IsMan()) {
			m_HumanGroup = new GroupPrefab("Human ViewController", m_EditorObject, string.Empty);
			m_HumanGroup.Insert(new CheckBoxPrefab("Selected Player", m_EditorObject, "Control"));
			m_HumanGroup.Insert(new EditBoxPrefab("Animation", m_EditorObject, "CurrentAnimation"));
			m_HumanGroup.Insert(new ButtonPrefab("Animate", m_EditorObject, "Animate"));
			m_HumanGroup.Open(HumanGroup);
			AddContent(m_HumanGroup);
		}
	}
	
	private void UpdateViewContext()
	{
		if (m_GeneralGroup)
			GeneralGroup = m_GeneralGroup.IsOpen();
		
		if (m_ObjectGroup)
			Object_Group = m_ObjectGroup.IsOpen();
		
		if (m_AdvancedGroup)
			AdvancedGroup = m_AdvancedGroup.IsOpen();
		
		if (m_HumanGroup)
			HumanGroup = m_HumanGroup.IsOpen();
		
		delete m_GeneralGroup;
		delete m_ObjectGroup;
		delete m_AdvancedGroup;
		delete m_HumanGroup;
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (w.IsInherited(TextWidget)) {
			string text = TextBoxPrefabGetString(w);
			
			if (text != string.Empty) {
				GetEditor().GetEditorHud().CreateNotification("Copied to clipboard!", COLOR_SALMON);
				GetGame().CopyToClipboard(text);
			}
		}
		
		return super.OnMouseButtonDown(w, x, y, button);
	}
	
	string TextBoxPrefabGetString(Widget text_widget)
	{
		TextBoxPrefab prefab;
		while (!prefab) {
			text_widget.GetUserData(prefab);
			text_widget = text_widget.GetParent();
			if (!text_widget) {
				return string.Empty;
			}
		}
		
		return prefab.GetPrefabController().Value;
	}
}