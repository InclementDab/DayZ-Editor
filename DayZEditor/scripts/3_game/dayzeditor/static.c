


#define EDITORPRINT


class EditorLog
{
	
	static EditorLogLevel CurrentLogLevel = EditorLogLevel.TRACE;
	
	private static void EditorPrint(string msg, EditorLogLevel level)
	{
		
		if (level >= CurrentLogLevel)
			Print(string.Format("[EDITOR::%1] %2", typename.EnumToString(EditorLogLevel, level), msg));
	}
	
	static void Trace(string msg)
	{
#ifdef EDITORPRINT
		EditorPrint(msg, EditorLogLevel.TRACE);
#endif
	}
	
	static void Debug(Class msg)
	{
#ifdef EDITORPRINT
		EditorPrint(msg.ToString(), EditorLogLevel.DEBUG);
#endif
	}
	
	static void Debug(string msg)
	{
#ifdef EDITORPRINT
		EditorPrint(msg, EditorLogLevel.DEBUG);
#endif
	}
	
	static void Info(string msg)
	{
#ifdef EDITORPRINT
		EditorPrint(msg, EditorLogLevel.INFO);
#endif
	}
	
	static void Info(Class msg)
	{
#ifdef EDITORPRINT
		EditorPrint(msg.ToString(), EditorLogLevel.INFO);
#endif
	}
	
	
	static void Warning(string msg)
	{
#ifdef EDITORPRINT
		EditorPrint(msg, EditorLogLevel.WARNING);
#endif
	}
	
	static void Error(string msg)
	{
#ifdef EDITORPRINT
		EditorPrint(msg, EditorLogLevel.ERROR);
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

private static ref WorkspaceWidget stored_workspace;
static Widget SafeCreateWidget(string file, Widget parent = null)
{
	CGame cgame = GetGame();
	if (cgame == null) {
		cgame = new CGame();
		Print("Created New Game");
	}
	
	WorkspaceWidget workspace = cgame.GetWorkspace();
	if (workspace == null) {
		workspace = cgame.GetLoadingWorkspace();
		if (workspace == null) {
			Print("AW SHIT BOYS, THIS SHIPS SINKIN AND YOUR GOIN DOWN WITH IT");
		} else {
			Print("LoadingWorkspace");
		}
	} else {
		Print("Workspace");
	}
		
	
	return workspace.CreateWidgets(file, parent);
	
	
}