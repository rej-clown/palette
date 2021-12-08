#pragma newdecls required

#include <palette>

public void OnMapStart() {
    PARSE_PALETTE();
}

public void OnClientPutInServer(int iClient) {
    if(!IsFakeClient(iClient))
        PrintToChat_palette(iClient, "{G}Welcume!");
}