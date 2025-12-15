[RegisterEditorCommand(EditorCameraCollisionCommand)]
class EditorCameraCollisionCommand: EditorCommand
{
    protected override bool Execute(Class sender, CommandArgs args)
    {
        super.Execute(sender, args);
        
        GetEditor().CameraCollision = !GetEditor().CameraCollision;
        
        if (GetEditor().CameraCollision) {
            GetEditor().GetEditorHud().CreateNotification("Camera Collision: ENABLED");
        } else {
            GetEditor().GetEditorHud().CreateNotification("Camera Collision: DISABLED");
        }
        
        return true;
    }
    
    override string GetName() 
    {
        return "Camera Collision Mode";
    }

    override ShortcutKeys GetShortcut() 
    {
        return { KeyCode.KC_LCONTROL, KeyCode.KC_LMENU, KeyCode.KC_C };
    }

    override Symbols GetSymbol()
    {
        return Symbols.CAMERA_SLASH; 
    }

    override LinearColor GetColor()
    {
        return LinearColor.ORANGE; 
    }

    override bool IsToggled()
    {
        return GetEditor().CameraCollision;
    }
}