
/**
 * @enum eDragPhase
 * @brief Defines the state of a drag-and-drop session within the DRAG_SESSION RPC.
 */
enum eDragPhase
{
	START = 0,
	UPDATE = 1,
	END = 2,
};

/**
 * @enum EditorRPC
 * @brief Defines RPC type identifiers for all editor-related multiplayer communication.
 *
 * Placed in 3_Game module to be accessible by both 4_World and 5_Mission scripts.
 */
enum EditorRPC
{
	EDITOR_CREATE = 39250,
	EDITOR_DESTROY = 39251,
	
	// create and delete objects
	OBJECT_CREATE = 39252,
	OBJECT_DELETE = 39253,
	
	// update an existing object
	OBJECT_UPDATE = 39254,
	
	// hide and unhide objects
	OBJECT_HIDE = 39255,
	OBJECT_UNHIDE = 39256,
	
	// update camera position
	CAMERA_UPDATE = 39257,
	
	UPDATE_WEATHER = 39258,
	PLAYER_TELEPORT = 39259,
	SERVER_CHAT = 39260,
	CAMERA_CONTROL = 39261,
	PLAYER_JOINED = 39262,
	OBJECT_UPDATE_TRANSFORM = 39263, // Deprecated by BATCH_UPDATE_TRANSFORM_PACKED for gizmo
	CLIENT_SYNC_COMPLETE = 39264,
	BATCH_UPDATE_TRANSFORM_PACKED = 39265, // New batched transform update
	DRAG_SESSION = 39266
};