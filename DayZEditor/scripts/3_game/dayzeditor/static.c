


#define EDITORPRINT

class EditorLog
{
	
	static EditorLogLevel CurrentLogLevel = EditorLogLevel.TRACE;
	static string ExclusiveLogMode;
	
	private static string m_LastCalledType;	
	
	static void EditorPrint(string msg, EditorLogLevel level)
	{
	
		if (level >= EditorLog.CurrentLogLevel) {
			
			if (level == EditorLogLevel.ERROR) {
				Error2("Editor Error", msg);
	#ifdef COMPONENT_SYSTEM
				Workbench.Dialog("Editor Log Error", msg);		
	#endif
				return;
			}
			
			string loglevel;
			switch (level) {
				case EditorLogLevel.TRACE:
					loglevel = "trace";
					break;
				
				case EditorLogLevel.DEBUG: {
					loglevel = "debug";
					break;
				}			
				
				case EditorLogLevel.INFO: {
					loglevel = "info";
					break;
				}
				
				case EditorLogLevel.WARNING: {
					loglevel = "warning";
					break;
				}
				
				case EditorLogLevel.ERROR: {
					loglevel = "error";
					break;
				}
			}
	
			
			if (msg.Contains("::")) {
				TStringArray msg_split();
				msg.Split(":", msg_split);
				
				if (ExclusiveLogMode != string.Empty && ExclusiveLogMode != msg_split[0]) {
					return;
				}
				
				if (m_LastCalledType != msg_split[0]) {
					m_LastCalledType = msg_split[0];
					PrintFormat("[%1::%2]:", loglevel, m_LastCalledType);
				}
				

				PrintFormat("	%1", msg.Substring(msg_split[0].Length() + 2, msg.Length() - msg_split[0].Length() - 2));
			} 
			else if (ExclusiveLogMode == string.Empty) {
				PrintFormat("[%1] %2", loglevel, msg);
			}
		}
	}
	
	
	static void Trace(string msg, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
#ifdef EDITORPRINT		
		EditorPrint(string.Format(msg, param1, param2, param3, param4, param5, param6, param7, param8, param9), EditorLogLevel.TRACE);
#endif
	}
	
	static void Debug(string msg, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
#ifdef EDITORPRINT
		EditorPrint(string.Format(msg, param1, param2, param3, param4, param5, param6, param7, param8, param9), EditorLogLevel.DEBUG);
#endif
	}	
	
	static void Info(string msg, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
#ifdef EDITORPRINT
		EditorPrint(string.Format(msg, param1, param2, param3, param4, param5, param6, param7, param8, param9), EditorLogLevel.INFO);
#endif
	}	
	
	static void Warning(string msg, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
#ifdef EDITORPRINT
		EditorPrint(string.Format(msg, param1, param2, param3, param4, param5, param6, param7, param8, param9), EditorLogLevel.WARNING);
#endif
	}	
	
	static void Error(string msg, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		// Special Case :)
		// ALWAYS bitch about errors
		EditorPrint(string.Format(msg, param1, param2, param3, param4, param5, param6, param7, param8, param9), EditorLogLevel.ERROR);

	}
	
	
	static void Debug(Class msg)
	{
#ifdef EDITORPRINT
		EditorPrint(msg.ToString(), EditorLogLevel.DEBUG);
#endif
	}
	

	
	static void Info(Class msg)
	{
#ifdef EDITORPRINT
		EditorPrint(msg.ToString(), EditorLogLevel.INFO);
#endif
	}
	

}


/* Used for Offline Editor Mission Creation */
static string CreateEditorMission(string map_name = "ChernarusPlus")
{
	EditorLog.Trace("EditorMissionGameplay::CreateEditorMission");
	string mission = "$saves:DayZEditor." + map_name;
	
	if (!FileExist(mission)) {
		EditorLog.Info("Editor Mission not found, creating....");
		MakeDirectory(mission);
	}
	
	FileHandle init = OpenFile(mission + "/init.c", FileMode.WRITE);
	FPrint(init, "\/\/ Returns Offline Editor Mission");
	FPrint(init, "Mission CreateCustomMission(string path)");
	FPrint(init, "{");
	FPrint(init, "	return new MissionGameplay();");
	FPrint(init, "}");
		
	CloseFile(init);
	
	return mission;
}

static ref ModStructure GetModFromObject(string object_name)
{
	EditorLog.Trace("GetModFromObject");
	ref array<ref ModStructure> mods = ModLoader.GetMods();
	string model_path = GetGame().ConfigGetTextOut("CfgVehicles " + object_name + " model");
	if (model_path == "UNKNOWN_P3D_FILE") return null;

	
	foreach (ModStructure mod: mods) {
		string dir;
		string path = mod.GetModPath();
		GetGame().ConfigGetText(string.Format("%1 dir", path), dir);
		dir.ToLower(); model_path.ToLower();
		if (model_path.Contains(dir))
			return mod;
		
	}
	
	return null;
}

static string GetIconFromMod(ref ModStructure m_ModInfo)
{
	EditorLog.Trace("GetIconFromMod");
	if (m_ModInfo != null) {
		string logo = m_ModInfo.GetModLogo();
		if (logo == string.Empty)
			logo = m_ModInfo.GetModLogoSmall();
		if (logo == string.Empty)
			logo = m_ModInfo.GetModLogoOver();
		if (logo == string.Empty)
			logo = m_ModInfo.GetModActionURL();
		if (logo != string.Empty)
			return logo;	
	}
	// default
	return "DayZEditor/gui/images/dayz_editor_icon_black.edds";
}

static void RecursiveGetParent(out ref Widget w, string name)
{
	if (w.GetName() == name) 
		return;
	
	w = w.GetParent();
	RecursiveGetParent(w, name);
}

static Widget GetWidgetRoot(ref Widget w)
{
	Widget parent = w;
	_GetWidgetRoot(parent);
	return parent;
}

static void _GetWidgetRoot(out ref Widget w)
{
	if (w.GetParent() == null) {
		return;	
	}
	
	w = w.GetParent();
	_GetWidgetRoot(w);
}



static bool CreateWidget(out Widget w, string layout_name, Widget parent = null)
{
	CGame game = GetGame();
	if (!game) {
		Error("CreateWidget: Game is null!");
		return false;
	}
	
	WorkspaceWidget workspace = game.GetWorkspace();
	if (!workspace) {
		Error("CreateWidget: Workspace is null! Try restarting Workbench!");
		return false;
	}
	
	w = workspace.CreateWidgets(layout_name, parent);
	return true;
}