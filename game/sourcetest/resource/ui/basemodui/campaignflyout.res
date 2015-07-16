"Resource/UI/CampaignFlyout.res"
{
	"PnlBackground"
	{
		"ControlName"			"Panel"
		"fieldName"				"PnlBackground"
		"xpos"					"0"
		"ypos"					"0"
		"zpos"					"-1"
		"wide"					"180" [$ENGLISH]
		"wide"					"270" [!$ENGLISH]
		"tall"					"65"  [$X360]
		"tall"					"85"  [$WIN32]
		"visible"				"1"
		"enabled"				"1"
		"paintbackground"		"1"
		"paintborder"			"1"
	}

	"BtnCreateGame"
	{
		"ControlName"			"BaseModHybridButton"
		"fieldName"				"BtnCreateGame"
		"xpos"					"0"
		"ypos"					"0"
		"wide"					"150"
		"tall"					"20"
		"autoResize"			"1"
		"pinCorner"				"0"
		"visible"				"1"
		"enabled"				"1"
		"tabPosition"			"0"
		"wrap"					"1"
		"navUp"					"BtnPlayCoopWithFriends"	[$X360]
		"navUp"					"BtnPlayOnGroupServer"		[$WIN32]
		"navDown"				"BtnPlayCoopWithAnyone"
		"labelText"				"#L4D360UI_FoudGames_CreateNewFlyout"
		"tooltiptext"			"#L4D360UI_FoudGames_CreateNewFlyout"
		"disabled_tooltiptext"	"#L4D360UI_FoudGames_CreateNewFlyout"
		"style"					"FlyoutMenuButton"
		"command"				"CreateGame"
	}

	"BtnPlayCoopWithFriends"
	{
		"ControlName"			"BaseModHybridButton"
		"fieldName"				"BtnPlayCoopWithFriends"
		"xpos"					"0"
		"ypos"					"20"
		"wide"					"150"
		"tall"					"20"
		"autoResize"			"1"
		"pinCorner"				"0"
		"visible"				"1"
		"enabled"				"1"
		"tabPosition"			"0"
		"wrap"					"1"
		"navUp"					"BtnPlayCoopWithAnyone"
		"navDown"				"BtnQuickMatch"			[$X360]
		"navDown"				"BtnPlayOnGroupServer"	[$WIN32]
		"labelText"				"#L4D360UI_MainMenu_PlayCoopWithFriends"
		"tooltiptext"			"#L4D360UI_MainMenu_PlayCoopWithFriends_Tip"
		"disabled_tooltiptext"	"#L4D360UI_MainMenu_PlayCoopWithFriends_Tip_Disabled"
		"style"					"FlyoutMenuButton"
		"command"				"FriendsMatch_campaign"
	}

	"BtnPlayCoopWithAnyone"
	{
		"ControlName"			"BaseModHybridButton"
		"fieldName"				"BtnPlayCoopWithAnyone"
		"xpos"					"0"
		"ypos"					"40"
		"wide"					"150"
		"tall"					"20"
		"autoResize"			"1"
		"pinCorner"				"0"
		"visible"				"1"
		"enabled"				"1"
		"tabPosition"			"0"
		"wrap"					"1"
		"navUp"					"BtnQuickMatch"
		"navDown"				"BtnPlayCoopWithFriends"
		"labelText"				"#L4D360UI_CustomMatch"	[$X360]
		"labelText"				"#L4D360UI_MainMenu_PlayOnline" [$WIN32]
		"tooltiptext"			"#L4D360UI_MainMenu_PlayCoopWithAnyone_Tip"
		"disabled_tooltiptext"	"#L4D360UI_MainMenu_PlayCoopWithAnyone_Tip_Disabled"
		"style"					"FlyoutMenuButton"
		"command"				"CustomMatch_"
	}	
	
	"BtnPlayOnGroupServer" [$WIN32]
	{
		"ControlName"			"BaseModHybridButton"
		"fieldName"				"BtnPlayOnGroupServer"
		"xpos"					"0"
		"ypos"					"60"
		"wide"					"150"
		"tall"					"20"
		"autoResize"			"1"
		"pinCorner"				"0"
		"visible"				"1"
		"enabled"				"1"
		"tabPosition"			"0"
		"wrap"					"1"
		"navUp"					"BtnPlayCoopWithFriends"
		"navDown"				"DrpCreateGame"
		"labelText"				"#L4D360UI_MainMenu_DedicatedServerBrowser"
		"tooltiptext"			"#L4D360UI_MainMenu_DedicatedServerBrowser"
		"disabled_tooltiptext"	"#L4D360UI_MainMenu_DedicatedServerBrowser"
		"style"					"FlyoutMenuButton"
		"command"				"OpenServerBrowser"
		//"command"				"GroupServer_"
	}
	
	"DrpCreateGame"
	{
		"ControlName"			"BaseModHybridButton"
		"fieldName"				"DrpCreateGame"
		"xpos"					"0"
		"ypos"					"80"
		"wide"					"150"
		"tall"					"20"
		"autoResize"			"1"
		"pinCorner"				"0"
		"visible"				"0"
		"enabled"				"1"
		"tabPosition"			"0"
		"wrap"					"1"
		"navUp"					"BtnPlayOnGroupServer"
		"navDown"				"BtnQuickMatch"
		//button and label
		"labelText"				"#L4D360UI_GameSettings_Create_Lobby"
		"style"					"FlyoutMenuButton"
		"command"				"CreateGame"
	}
}