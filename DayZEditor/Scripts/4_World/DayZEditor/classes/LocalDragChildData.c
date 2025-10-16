/**
 * @class LocalDragChildData
 * @brief A simple, robust data container for a client-side drag session.
 *
 * This replaces a complex nested Param2 structure to avoid potential
 * EnforceScript garbage collection issues and improve code clarity.
 */
class LocalDragChildData
{
    EditorObject m_ChildObject;
    vector m_RelativePos;
    vector m_RelativeOri;

    void LocalDragChildData(EditorObject child, vector relPos, vector relOri)
    {
        m_ChildObject = child;
        m_RelativePos = relPos;
        m_RelativeOri = relOri;
    }
}