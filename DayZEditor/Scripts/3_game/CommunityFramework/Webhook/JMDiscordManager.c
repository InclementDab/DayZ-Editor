class JMDiscordManager
{
/*
 * A rewrite of Dj's discord webhook to allow for more flexibility
 */
};

static ref JMDiscordManager g_jmDiscordManager;

static ref JMDiscordManager GetDiscordManager()
{
	return g_jmDiscordManager;
}

static void CreateDiscordManager()
{
	g_jmDiscordManager = new JMDiscordManager;
}