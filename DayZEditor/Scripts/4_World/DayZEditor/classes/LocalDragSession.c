/**
 * @class LocalDragSession
 * @brief Client-side data structure for managing a drag operation received from the server.
 */
class LocalDragSession
{
    EditorObject m_ParentObject;
    ref array<ref LocalDragChildData> m_ChildData;

    void LocalDragSession()
    {
        m_ChildData = new array<ref LocalDragChildData>();
    }
};