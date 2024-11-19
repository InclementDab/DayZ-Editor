class EditorFileDialogController: ViewController
{
	ref ObservableCollection<ref EditorFileView> Files = new ObservableCollection<ref EditorFileView>(this);
	ref ObservableCollection<ref EditorDirectoryView> Directories = new ObservableCollection<ref EditorDirectoryView>(this);
	ref ObservableCollection<ref EditorFileQuickView> FolderViews = new ObservableCollection<ref EditorFileQuickView>(this);
}

class EditorFileView: ScriptView
{
	override string GetLayoutFile()
	{
		return "DayZEditor/GUI/layouts/dialogs/File.layout";
	}
}

class EditorDirectoryView: ScriptView
{
	override string GetLayoutFile()
	{
		return "DayZEditor/GUI/layouts/dialogs/Directory.layout";
	}
}

class EditorFileQuickView: ScriptView
{
	override string GetLayoutFile()
	{
		return "DayZEditor/GUI/layouts/dialogs/FileDialogFolderQuickView.layout";
	}
}

class EditorFileDialog: ScriptViewMenu
{
	protected EditorFileDialogController m_TemplateController;

	protected string m_CurrentDirectory;
	protected ref ScriptCaller m_ScriptCallback;
	
	TextWidget TitleText;

	void EditorFileDialog(string title, string filter, ScriptCaller on_file_selected)
	{
		m_TemplateController = EditorFileDialogController.Cast(m_Controller);
		m_CurrentDirectory = SystemPath.Saves();
		
		m_ScriptCallback = on_file_selected;
	}
	
	EditorFileDialogController GetTemplateController()
	{
		return m_TemplateController;
	}
		
	override typename GetControllerType()
	{
		return EditorFileDialogController;
	}
	
	override string GetLayoutFile()
	{
		return "DayZEditor/GUI/layouts/dialogs/FileDialog.layout";
	}
}