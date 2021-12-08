stock void parse_palette(char[] path, int size) {
    GetConfigFileByGame(path, size);
    
    if(!FileExists(path))
        SetFailState("Where is my palette: %s", path);

    if(!GET_PALETTE()) {
        SET_PALETTE(new ArrayList(16, 0));
        SET_USERMESSAGE_TYPE(GetUserMessageType());
    }

    int iLine;
    if(create_palette_parser().ParseFile(path, iLine) != SMCError_Okay)
        LogError("An error was detected on line '%i' while reading", iLine);
}

static void GetConfigFileByGame(char[] szConfig, int size)
{
    // [a]ddons/
    if(szConfig[0] != 'a')
        BuildPath(Path_SM, szConfig, size, szConfig);

    ReplaceString(szConfig, size, "\\", "/");
    int len = strlen(szConfig);

    if(szConfig[len-1] != 'i')
    {
        GetGameFolderName(szConfig[len], size - (len-1));
        Format(szConfig, size, "%s.ini", szConfig);
    }
}

stock void replace_colors_palette(char[] t, int s, bool c = false) {
    int i;
    char szKey[16];
    char szColor[16];

    while(i < GET_PALETTE().Length) {
        GET_PALETTE().GetString(i, szKey, sizeof(szKey));
        GET_PALETTE().GetString(i+1, szColor, sizeof(szColor));

        ReplaceString(t, s, szKey, (!c) ? szColor : NULL_STRING, true);
        if(c)
            ReplaceString(t, s, szColor, NULL_STRING, true);

        i+=2;
    }
}

stock bool prepare_usermessage_palette(int sender, int[] recipients, int repCount, ArrayList t, const char[] channel = "TextMsg") {
    if(!t)
        return false;

    StringMap m;
    char text[512];
    ArrayList queue = new ArrayList(16, 0);
    
    for(int i = 0; i < repCount; i++) {
        if(!IsClientConnected(recipients[i]))
            continue;
        
        t.GetString(i, text, sizeof(text));

        m = new StringMap();
        m.SetValue("sender", sender);
        m.SetValue("recipient", recipients[i]);
        m.SetString("channel", channel);
        m.SetString("text", text);

        queue.Push(m);
    }

    RequestFrame(next_frame_palette, queue);
    return true;
}

stock void next_frame_palette(ArrayList queue) {
    if(!queue)
        return;
    
    int sender;
    int recipient;
    char text[512];
    char channel[16];

    StringMap m;
    for(int i; i < queue.Length; i++) {
        if(!(m = view_as<StringMap>(queue.Get(i))))
            continue;
        
        m.GetValue("sender", sender);
        m.GetValue("recipient", recipient);
        m.GetString("text", text, sizeof(text));
        m.GetString("channel", channel, sizeof(channel));

        delete m;

        create_usermessage_palette(channel, text, recipient, sender);
    }

    delete queue;
}

stock void create_usermessage_palette(const char[] n, const char[] text, int recipient, int s = -1) {
    static Handle um;
    if((um = StartMessageOne(n, recipient, USERMSG_RELIABLE)) != null) {

        if(!strcmp(n, "SayText2"))
            fill_usermessage_ST2_palette(um, s, text);

        else fill_usermessage_TM_palette(um, text);

        EndMessage();
    }
}

stock void fill_usermessage_ST2_palette(Handle u, int s, const char[] text) {
    static int j;
    const int MAX_PARAMS = 4;

    
    if(!GET_USERMESSAGE_TYPE()) {
        BfWriteByte(u, s);
        BfWriteByte(u, true);
        BfWriteString(u, text);
    } else {
        j = 0;

        PbSetInt(u, "ent_idx", s);
        PbSetBool(u, "chat", true);
        PbSetString(u, "msg_name", text);
        while(j < MAX_PARAMS)
            PbAddString(u, "params", "");
    }
}

stock void fill_usermessage_TM_palette(Handle u, const char[] text) {
    static int j;
    const int MAX_PARAMS = 5;

    
    if(!GET_USERMESSAGE_TYPE()) {
        BfWriteByte(u, 3);
        BfWriteString(u, text);
    } else {
        j = 1;

        PbSetInt(u, "msg_dst", 3);
        PbAddString(u, "params", text);
        while(j < MAX_PARAMS)
            PbAddString(u, "params", "");
    }
}