class Sandbox: TreeNode
{
	// structure of actual mission files
	static const string FILES = "Files";
	static const string EDITORS = "Editors";
	static const string COMMANDS = "Commands";
	
	void Sandbox(string uuid, string display_name, Symbols icon)
	{		
		Add(new FileNode(FILES, "Files", Symbols.FOLDER_USER, uuid));
		
		Add(new TreeNode(EDITORS, "Editors", Symbols.PEOPLE_SIMPLE));
		Add(new TreeNode(COMMANDS, "Commands", Symbols.COMMAND));
				
		this[COMMANDS].Add(new AfterlifeToggle("Afterlife", "View Hidden", Symbols.GHOST));
		this[COMMANDS].Add(new AddLayerCommand("AddLayer", "Add Layer", Symbols.LAYER_PLUS));
		this[COMMANDS].Add(new SetLayerActiveCommand("SetLayerActive", "Set Layer Active", string.Empty));
		this[COMMANDS].Add(new CommandNode("Bolt", "Lightning Bolt", Symbols.BOLT));
		this[COMMANDS].Add(new PianoCommand("Piano", "Drop Piano", Symbols.PIANO));
		this[COMMANDS].Add(new BoxCommand("Box", "Box Selection", Symbols.SQUARE_DASHED));
		this[COMMANDS].Add(new CommandNode("Camera", "Camera", Symbols.CAMERA));
		this[COMMANDS].Add(new EllipseCommand("Ellipse", "Ellipse Selection", Symbols.CIRCLE_DASHED));
		this[COMMANDS].Add(new CommandNode("Copy", "Copy", Symbols.COPY));
		this[COMMANDS].Add(new CommandNode("Cut", "Cut", Symbols.SCISSORS));
		this[COMMANDS].Add(new DeleteCommand("Delete", "Delete", Symbols.TRASH));
		this[COMMANDS].Add(new CommandNode("Ground", "Ground Mode", Symbols.IMAGE_LANDSCAPE));
		this[COMMANDS].Add(new LassoCommand("Lasso", "Lasso Select", Symbols.LASSO));
		this[COMMANDS].Add(new CommandNode("Lock", "Lock", Symbols.LOCK));
		this[COMMANDS].Add(new CommandNode("Magnet", "Magnet", Symbols.MAGNET));
		this[COMMANDS].Add(new CommandNode("New", "New", Symbols.FILE));
		this[COMMANDS].Add(new CommandNode("Open", "Open", Symbols.FOLDER_OPEN));
		this[COMMANDS].Add(new CommandNode("Paste", "Paste", Symbols.PASTE));
		this[COMMANDS].Add(new CommandNode("Redo", "Redo", Symbols.ROTATE_RIGHT));
		this[COMMANDS].Add(new RenameCommand("Rename", "Rename", string.Empty));
		this[COMMANDS].Add(new CommandNode("SaveAs", "Save As", Symbols.FLOPPY_DISK_PEN));
		this[COMMANDS].Add(new CommandNode("Save", "Save", Symbols.FLOPPY_DISK));
		this[COMMANDS].Add(new CommandNode("Snap", "Snapping Mode", Symbols.THUMBTACK));
		this[COMMANDS].Add(new CommandNode("Undo", "Undo", Symbols.ROTATE_LEFT));
		this[COMMANDS].Add(new CommandNode("Unlock", "Unlock", Symbols.LOCK_OPEN));
		this[COMMANDS].Add(new WeatherToggle("Weather", "Weather", Symbols.SUN));
		this[COMMANDS].Add(new CursorToggle("Cursor", "Toggle Cursor", Symbols.ARROW_POINTER));
		this[COMMANDS].Add(new HudToggle("Hud", "Toggle Hud", Symbols.EYE));
		this[COMMANDS].Add(new MapToggle("Map", "Toggle Map", Symbols.MAP));
		this[COMMANDS].Add(new ControlToggle("Control", "Toggle Control", Symbols.PERSON_PREGNANT));
		this[COMMANDS].Add(new CreateFolder("CreateFolder", "Create Folder", Symbols.FOLDER_PLUS));
		this[COMMANDS].Add(new CommandNode("Console", "Toggle Console", Symbols.XBOX));
	}
	
	TreeNode GetEditors()
	{
		return this[EDITORS];
	}
	
	TreeNode GetCommand(string uuid)
	{
		return this[COMMANDS][uuid];
	}
}