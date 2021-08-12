class EditorObjectPropertiesDialog: EditorDialogBase
{
	static bool GeneralGroup = true;
	static bool Object_Group = true;
	static bool HumanGroup = true;
	static bool LightGroup = true;
	static bool DebugGroup = false;
	
	protected ref GroupPrefab m_GeneralGroup;
	protected ref GroupPrefab m_ObjectGroup;
	protected ref GroupPrefab m_HumanGroup;
	protected ref GroupPrefab m_DebugGroup;
	protected ref GroupPrefab m_LightGroup;
	
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
		
		m_GeneralGroup = new GroupPrefab("#STR_EDITOR_GENERAL", m_EditorObject, string.Empty);
		m_GeneralGroup.Insert(new CheckBoxPrefab("#STR_EDITOR_SHOW", m_EditorObject, "Show"));
		m_GeneralGroup.Insert(new EditBoxPrefab("#STR_EDITOR_NAME", m_EditorObject, "Name"));
		m_GeneralGroup.Insert(new VectorPrefab("#STR_EDITOR_POSITION", m_EditorObject, "Position"));
		m_GeneralGroup.Insert(new VectorPrefab("#STR_EDITOR_ORIENTATION", m_EditorObject, "Orientation"));
		m_GeneralGroup.Insert(new EditBoxNumberPrefab("#STR_EDITOR_SCALE", m_EditorObject, "Scale", 0.01));
		//m_GeneralGroup.Insert(new CheckBoxPrefab("#STR_EDITOR_EDITOR_ONLY", m_EditorObject, "EditorOnly", m_EditorObject.EditorOnly));
		m_GeneralGroup.Open(GeneralGroup);
		AddContent(m_GeneralGroup);
		
		// All of these bindings are directly on the world object
		if (editor_object.GetWorldObject().IsInherited(EditorScriptedLight)) {			
			m_LightGroup = new GroupPrefab("Light Controls", m_EditorObject.GetWorldObject(), string.Empty);
			m_LightGroup.Insert(new SliderPrefab("Radius", m_EditorObject.GetWorldObject(), "Radius", 0, 1000));
			m_LightGroup.Insert(new CheckBoxPrefab("Cast Shadow", m_EditorObject.GetWorldObject(), "CastShadow"));
			m_LightGroup.Insert(new CheckBoxPrefab("Enable Specular", m_EditorObject.GetWorldObject(), "EnableSpecular"));
			m_LightGroup.Insert(new CheckBoxPrefab("Enable Linear", m_EditorObject.GetWorldObject(), "EnableLinear"));
			m_LightGroup.Insert(new SliderPrefab("Pulse Coef", m_EditorObject.GetWorldObject(), "PulseCoef"));
			m_LightGroup.Insert(new CheckBoxPrefab("Visible During Day", m_EditorObject.GetWorldObject(), "VisibleDuringDay"));
			
			m_LightGroup.Insert(new CheckBoxPrefab("Enable Heat Haze", m_EditorObject.GetWorldObject(), "HeatHaze"));
			m_LightGroup.Insert(new SliderPrefab("Heat Haze Radius", m_EditorObject.GetWorldObject(), "HeatHazeRadius"));
			m_LightGroup.Insert(new SliderPrefab("Heat Haze Power", m_EditorObject.GetWorldObject(), "HeatHazePower", 0, 1));
			
			m_LightGroup.Insert(new ColorPickerPrefab("Diffuse Color", m_EditorObject.GetWorldObject(), "DiffuseColor"));
			m_LightGroup.Insert(new ColorPickerPrefab("Ambient Color", m_EditorObject.GetWorldObject(), "AmbientColor"));
			m_LightGroup.Insert(new SliderPrefab("Brightness", m_EditorObject.GetWorldObject(), "Brightness"));
			
			m_LightGroup.Insert(new CheckBoxPrefab("Enable Flare", m_EditorObject.GetWorldObject(), "FlareVisible"));
			m_LightGroup.Insert(new VectorPrefab("Flare Position", m_EditorObject.GetWorldObject(), "FlareRelativePosition"));

			m_LightGroup.Insert(new SliderPrefab("Spot Light Angle", m_EditorObject.GetWorldObject(), "SpotLightAngle", 0, 180));
			m_LightGroup.Open(LightGroup);
			AddContent(m_LightGroup);
		}
		
		m_ObjectGroup = new GroupPrefab("#STR_EDITOR_OBJECT", m_EditorObject, string.Empty);
		m_ObjectGroup.Insert(new EditBoxNumberPrefab("#STR_EDITOR_HEALTH", m_EditorObject, "Health"));
		m_ObjectGroup.Insert(new CheckBoxPrefab("#STR_EDITOR_ENABLE_SIMULATION", m_EditorObject, "Simulate"));
		m_ObjectGroup.Insert(new CheckBoxPrefab("#STR_EDITOR_LOCK", m_EditorObject, "Locked"));
		m_ObjectGroup.Insert(new CheckBoxPrefab("#STR_EDITOR_ENABLE_PHYSICS", m_EditorObject, "Physics"));
		m_ObjectGroup.Insert(new CheckBoxPrefab("#STR_EDITOR_ENABLE_DAMAGE", m_EditorObject, "AllowDamage"));
		//m_ObjectGroup.Insert(new CheckBoxPrefab("#STR_EDITOR_ENABLE_COLLISION", m_EditorObject, "Collision"));
		string expansion_check = "ExpansionMarketModule";
		if (expansion_check.ToType() && m_EditorObject.GetWorldObject().IsInherited(EntityAI)) {
			m_ObjectGroup.Insert(new EditBoxPrefab("Trader Type", m_EditorObject, "ExpansionTraderType"));
		}
		
		m_ObjectGroup.Open(Object_Group);
		AddContent(m_ObjectGroup);
		
		if (editor_object.HasAnimations()) {
			GroupPrefab animations_group = new GroupPrefab("Object Animations", m_EditorObject, string.Empty);
			
			map<string, ref EditorObjectAnimationSource> object_animations = editor_object.GetObjectAnimations();
			foreach (string name, EditorObjectAnimationSource anim: object_animations) {
				animations_group.Insert(new SliderPrefab(name, anim, "AnimationPhase", 0, 1));
			}
			
			AddContent(animations_group);
		}
		
		if (editor_object.GetWorldObject().IsMan()) {
			m_HumanGroup = new GroupPrefab("#STR_EDITOR_HUMAN", m_EditorObject, string.Empty);
			m_HumanGroup.Insert(new CheckBoxPrefab("#STR_EDITOR_SELECTED_PLAYER", m_EditorObject, "Control"));
			DropdownListPrefab<int> animations = new DropdownListPrefab<int>("#STR_EDITOR_ANIMATION", m_EditorObject, "CurrentAnimation");
			map<string, int> emote_list = PlayerBase.GetEmoteList();
			foreach (string emote_name, int emote_id: emote_list) {
				animations[emote_name] = emote_id;
			}
			
			m_HumanGroup.Insert(animations);
			m_HumanGroup.Insert(new ButtonPrefab("#STR_EDITOR_CINEMATIC_CAMERA_RUN", m_EditorObject, "Animate"));
			
			m_HumanGroup.Open(HumanGroup);
			AddContent(m_HumanGroup);
		}
		
		m_DebugGroup = new GroupPrefab("Debug", m_EditorObject.GetData(), string.Empty);
		m_DebugGroup.Insert(new TextBoxPrefab("#STR_EDITOR_TYPE", m_EditorObject.GetData(), "Type"));
		m_DebugGroup.Insert(new TextBoxPrefab("#STR_EDITOR_ID", m_EditorObject, "ObjectID"));
		m_DebugGroup.Insert(new TextBoxPrefab("Flags", m_EditorObject.GetData(), "Flags"));
		m_DebugGroup.Insert(new TextBoxPrefab("#STR_EDITOR_MODEL", m_EditorObject.GetData(), "Model"));
		m_DebugGroup.Open(DebugGroup);
		AddContent(m_DebugGroup);
	}
	
	private void UpdateViewContext()
	{
		if (m_GeneralGroup)
			GeneralGroup = m_GeneralGroup.IsOpen();
		
		if (m_ObjectGroup)
			Object_Group = m_ObjectGroup.IsOpen();
		
		if (m_DebugGroup)
			DebugGroup = m_DebugGroup.IsOpen();
		
		if (m_HumanGroup)
			HumanGroup = m_HumanGroup.IsOpen();
		
		if (m_LightGroup)
			LightGroup = m_LightGroup.IsOpen();
		
		delete m_GeneralGroup;
		delete m_LightGroup;
		delete m_ObjectGroup;
		delete m_DebugGroup;
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