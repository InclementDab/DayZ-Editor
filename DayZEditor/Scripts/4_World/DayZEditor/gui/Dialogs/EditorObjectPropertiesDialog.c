class EditorObjectPropertiesDialog: EditorDialogBase
{
	protected bool ExecuteCode;
	protected ref array<EditorObject> m_EditorObjects = {};
	protected ref EditorMultiObjectCommandController m_EditorMultiObjectCommandController;
	
	void EditorObjectPropertiesDialog(string title, notnull array<EditorObject> editor_objects)
	{
		m_EditorObjects.InsertArray(editor_objects);
		
		UpdateViewContext();
		
		AddButton(DialogResult.OK);
		
		EditorEvents.OnObjectSelected.Insert(OnObjectSelected);
		EditorEvents.OnObjectDeselected.Insert(OnObjectDeselected);
	}
	
	void ~EditorObjectPropertiesDialog()
	{
		delete m_EditorObjects;
		delete m_EditorMultiObjectCommandController;
	}
	
	void PropertyChanged(string property_name)
	{
		switch (property_name) {
			case "ExecuteCode": {
				ExecuteCode = false;
				if (!m_EditorObjects[0]) {
					break;
				}
				
				m_EditorObjects[0].ExecuteCode();
				break;
			}
		}
	}
		
	protected void OnObjectSelected(Class context, EditorObject editor_object)
	{		
		m_EditorObjects.Insert(editor_object);
		UpdateViewContext();
	}
	
	protected void OnObjectDeselected(Class context, EditorObject editor_object)
	{
		m_EditorObjects.RemoveItem(editor_object);
		UpdateViewContext();
	}
	
	void UpdateViewContext()
	{
		// cleanup content
		ClearContent();
		delete m_EditorMultiObjectCommandController;
		
		if (m_EditorObjects.Count() == 0) {
			return;
		}
		
		if (m_EditorObjects.Count() == 1) {
			SetEditorObject(m_EditorObjects[0]);
			return;
		}
		
		SetMultipleEditorObjects(m_EditorObjects);
	}
			
	void SetMultipleEditorObjects(array<EditorObject> editor_objects)
	{
		m_EditorMultiObjectCommandController = new EditorMultiObjectCommandController(editor_objects);
		
		GroupPrefab general_group = new GroupPrefab("#STR_EDITOR_GENERAL", m_EditorMultiObjectCommandController, string.Empty);
		general_group.Insert(new CheckBoxPrefab("#STR_EDITOR_SHOW", m_EditorMultiObjectCommandController, "Show"));
		general_group.Insert(new EditBoxPrefab("#STR_EDITOR_NAME", m_EditorMultiObjectCommandController, "Name"));
		general_group.Insert(new VectorPrefab("#STR_EDITOR_POSITION", m_EditorMultiObjectCommandController, "Position"));
		general_group.Insert(new VectorPrefab("#STR_EDITOR_ORIENTATION", m_EditorMultiObjectCommandController, "Orientation"));
		general_group.Insert(new EditBoxNumberPrefab("#STR_EDITOR_SCALE", m_EditorMultiObjectCommandController, "Scale", 0.01));
		
		GroupPrefab object_group = new GroupPrefab("#STR_EDITOR_OBJECT", m_EditorMultiObjectCommandController, string.Empty);
		object_group.Insert(new EditBoxNumberPrefab("#STR_EDITOR_HEALTH", m_EditorMultiObjectCommandController, "Health"));
		object_group.Insert(new CheckBoxPrefab("#STR_EDITOR_EDITOR_ONLY", m_EditorMultiObjectCommandController, "EditorOnly"));
		object_group.Insert(new CheckBoxPrefab("#STR_EDITOR_ENABLE_SIMULATION", m_EditorMultiObjectCommandController, "Simulate"));
		object_group.Insert(new CheckBoxPrefab("#STR_EDITOR_LOCK", m_EditorMultiObjectCommandController, "Locked"));
		object_group.Insert(new CheckBoxPrefab("#STR_EDITOR_ENABLE_PHYSICS", m_EditorMultiObjectCommandController, "Physics"));
		object_group.Insert(new CheckBoxPrefab("#STR_EDITOR_ENABLE_DAMAGE", m_EditorMultiObjectCommandController, "AllowDamage"));
		
		AddContent(general_group);
		AddContent(object_group);
		
		AutoSize();
	}
	
	// This function is a mess
	void SetEditorObject(EditorObject editor_object)
	{		
		GroupPrefab general_group = new GroupPrefab("#STR_EDITOR_GENERAL", editor_object, string.Empty);
		general_group.Insert(new CheckBoxPrefab("#STR_EDITOR_SHOW", editor_object, "Show"));
		general_group.Insert(new EditBoxPrefab("#STR_EDITOR_NAME", editor_object, "Name"));
		general_group.Insert(new VectorPrefab("#STR_EDITOR_POSITION", editor_object, "Position"));
		general_group.Insert(new VectorPrefab("#STR_EDITOR_ORIENTATION", editor_object, "Orientation"));
		general_group.Insert(new EditBoxNumberPrefab("#STR_EDITOR_SCALE", editor_object, "Scale", 0.01));
		if (editor_object.GetWorldObject().IsInherited(EditorLootPoint)) {
			general_group.Insert(new EditBoxNumberPrefab("Height", editor_object.GetWorldObject(), "Height"));
			general_group.Insert(new EditBoxNumberPrefab("Range", editor_object.GetWorldObject(), "Range"));
		}
		
		//general_group.Insert(new CheckBoxPrefab("#STR_EDITOR_EDITOR_ONLY", editor_object, "EditorOnly", editor_object.EditorOnly));
		AddContent(general_group);
		
		// All of these bindings are directly on the world object
		if (editor_object.GetWorldObject().IsInherited(NetworkLightBase)) {			
			GroupPrefab light_group = new GroupPrefab("Light Controls", editor_object.GetWorldObject(), string.Empty);
			light_group.Insert(new SliderPrefab("Radius", editor_object.GetWorldObject(), "Radius", 0, 1000));
			light_group.Insert(new CheckBoxPrefab("Cast Shadow", editor_object.GetWorldObject(), "CastShadow"));
			light_group.Insert(new CheckBoxPrefab("Enable Specular", editor_object.GetWorldObject(), "EnableSpecular"));
			light_group.Insert(new CheckBoxPrefab("Enable Linear", editor_object.GetWorldObject(), "EnableLinear"));
			light_group.Insert(new SliderPrefab("Pulse Coef", editor_object.GetWorldObject(), "PulseCoef"));
			light_group.Insert(new CheckBoxPrefab("Visible During Day", editor_object.GetWorldObject(), "VisibleDuringDay"));
			
			light_group.Insert(new CheckBoxPrefab("Enable Heat Haze", editor_object.GetWorldObject(), "HeatHaze"));
			light_group.Insert(new SliderPrefab("Heat Haze Radius", editor_object.GetWorldObject(), "HeatHazeRadius"));
			light_group.Insert(new SliderPrefab("Heat Haze Power", editor_object.GetWorldObject(), "HeatHazePower", 0, 1));
			
			light_group.Insert(new ColorPickerPrefab("Diffuse Color", editor_object.GetWorldObject(), "DiffuseColor"));
			light_group.Insert(new ColorPickerPrefab("Ambient Color", editor_object.GetWorldObject(), "AmbientColor"));
			light_group.Insert(new SliderPrefab("Brightness", editor_object.GetWorldObject(), "Brightness"));
			
			light_group.Insert(new CheckBoxPrefab("Enable Flare", editor_object.GetWorldObject(), "FlareVisible"));
			light_group.Insert(new VectorPrefab("Flare Position", editor_object.GetWorldObject(), "FlareRelativePosition"));

			light_group.Insert(new SliderPrefab("Spot Light Angle", editor_object.GetWorldObject(), "SpotLightAngle", 0, 180));
			AddContent(light_group);
		}
		
		GroupPrefab object_group = new GroupPrefab("#STR_EDITOR_OBJECT", editor_object, string.Empty);
		object_group.Insert(new EditBoxNumberPrefab("#STR_EDITOR_HEALTH", editor_object, "Health"));
		object_group.Insert(new CheckBoxPrefab("#STR_EDITOR_EDITOR_ONLY", editor_object, "EditorOnly"));
		object_group.Insert(new CheckBoxPrefab("#STR_EDITOR_ENABLE_SIMULATION", editor_object, "Simulate"));
		object_group.Insert(new CheckBoxPrefab("#STR_EDITOR_LOCK", editor_object, "Locked"));
		object_group.Insert(new CheckBoxPrefab("#STR_EDITOR_ENABLE_PHYSICS", editor_object, "Physics"));
		object_group.Insert(new CheckBoxPrefab("#STR_EDITOR_ENABLE_DAMAGE", editor_object, "AllowDamage"));
		//object_group.Insert(new CheckBoxPrefab("#STR_EDITOR_ENABLE_COLLISION", editor_object, "Collision"));
		string expansion_check = "ExpansionMarketModule";
		if (expansion_check.ToType() && editor_object.GetWorldObject().IsInherited(EntityAI)) {
			object_group.Insert(new EditBoxPrefab("Trader Type", editor_object, "ExpansionTraderType"));
		}
		
		AddContent(object_group);
		
		if (editor_object.HasAnimations()) {
			GroupPrefab animations_group = new GroupPrefab("Object Animations", editor_object, string.Empty);
			
			map<string, ref EditorObjectAnimationSource> object_animations = editor_object.GetObjectAnimations();
			foreach (string name, EditorObjectAnimationSource anim: object_animations) {
				animations_group.Insert(new SliderPrefab(name, anim, "AnimationPhase", 0, 1));
			}
			
			AddContent(animations_group);
		}
		
		if (editor_object.GetWorldObject().IsMan()) {
			GroupPrefab human_group = new GroupPrefab("#STR_EDITOR_HUMAN", editor_object, string.Empty);
			human_group.Insert(new CheckBoxPrefab("#STR_EDITOR_SELECTED_PLAYER", editor_object, "Control"));
			DropdownListPrefab<int> animations = new DropdownListPrefab<int>("#STR_EDITOR_ANIMATION", editor_object, "CurrentAnimation");
			map<string, int> emote_list = PlayerBase.GetEmoteList();
			foreach (string emote_name, int emote_id: emote_list) {
				animations[emote_name] = emote_id;
			}
			
			human_group.Insert(animations);
			human_group.Insert(new ButtonPrefab("#STR_EDITOR_CINEMATIC_CAMERA_RUN", editor_object, "Animate"));
			
			AddContent(human_group);
		}
		
		//AddContent(new MultilineEditBoxPrefab("Execute Code", editor_object, "TestingScript"));
		//AddContent(new ButtonPrefab("Execute", this, "ExecuteCode"));
		
		if (GetEditor().Settings.DebugMode) {
			GroupPrefab debug_group = new GroupPrefab("Debug", editor_object.GetData(), string.Empty);
			debug_group.Insert(new TextBoxPrefab("#STR_EDITOR_TYPE", editor_object.GetData(), "Type"));
			debug_group.Insert(new TextBoxPrefab("#STR_EDITOR_ID", editor_object, "ObjectID"));
			debug_group.Insert(new TextBoxPrefab("Flags", editor_object.GetData(), "Flags"));
			debug_group.Insert(new TextBoxPrefab("#STR_EDITOR_MODEL", editor_object.GetData(), "Model"));
			AddContent(debug_group);
		}
		
		// Auto resize
		AutoSize();
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