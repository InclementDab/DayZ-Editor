class XMLCallback : Managed
{
    void OnStart( ref XMLDocument document );

    void OnSuccess( ref XMLDocument document );

    void OnFailure( ref XMLDocument document );
};