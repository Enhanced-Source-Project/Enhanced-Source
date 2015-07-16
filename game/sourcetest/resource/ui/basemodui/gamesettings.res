"Resource/UI/GameSettings.res"
{
	"GameSettings"
	{
		"ControlName"				"Frame"
		"fieldName"					"GameSettings"
		"xpos"						"0"
		"ypos"						"0"
		"wide"						"f0"
		"tall"						"f0"
		"autoResize"				"0"
		"pinCorner"					"0"
		"visible"					"1"
		"enabled"					"1"
		"tabPosition"				"0"
	}
	
	"Title"
	{
		"fieldName"		"Title"
		"xpos"		"c-266"
		"ypos"		"123"
		"wide"		"150"
		"tall"		"19"
		"zpos"		"5"
		"font"		"DefaultExtraLarge"
		"textAlignment"		"west"
		"ControlName"		"Label"
		"labelText"		""
		"fgcolor_override"		"224 224 224 255"
	}
	
	"ImgBackground"
	{
		"ControlName"			"L4DMenuBackground"
		"fieldName"				"ImgBackground"
		"xpos"					"0"
		"ypos"					"189"
		"zpos"					"-1"
		"wide"					"f0"
		"tall"					"144"
		"autoResize"			"0"
		"pinCorner"				"0"
		"visible"				"1"				[$WIN32]
		"visible"				"0"				[$X360]
		"enabled"				"1"
		"tabPosition"			"0"
		"fillColor"				"0 0 0 0"
	} 
	
	"ImgLevelImage"
	{
		"ControlName"			"ImagePanel"
		"fieldName"				"ImgLevelImage"
		"xpos"					"c52"
		"ypos"					"173"
		"wide"					"133"
		"tall"					"100"
		"zpos"					"1"
		"scaleImage"			"1"
		"pinCorner"				"0"
		"visible"				"1"
		"enabled"				"1"
		"tabPosition"			"0"
		"image"					"maps/any"
		"scaleImage"			"1"
	}
	"ImgLevelImageFrame"
	{
		"ControlName"			"Panel"
		"fieldName"				"ImgLevelImageFrame"
		"xpos"					"c51"
		"ypos"					"172"
		"wide"					"135"
		"tall"					"102"
		"scaleImage"			"1"
		"pinCorner"				"0"
		"visible"				"1"
		"enabled"				"1"
		"tabPosition"			"0"
		"bgcolor_override"		"0 0 0 128"
		//"image"					"campaignFrame"
		//"scaleImage"			"1"
	}
	"MissionLabel"
	{
		"ControlName"			"Label"
		"fieldName"				"MissionLabel"
		"xpos"					"c45"
		"ypos"					"280"
		"wide"					"137"
		"tall"					"15"
		"visible"				"0"
		"enabled"				"1"
		"tabPosition"			"0"
		"textAlignment"		"west"
		"fgcolor_override"		"83 148 192 255"
	}
	// Campaign dropdown
	"DrpGameType"
	{
		"ControlName"			"DropDownMenu"
		"fieldName"				"DrpGameType"
		"xpos"					"c-250"	
		"ypos"					"185"	
		"zpos"					"1"
		"wide"					"280"	
		"tall"					"15"			[$WIN32]
		"tall"					"20"			[$X360]
		"visible"				"0"
		"enabled"				"1"
		"navUp"					"BtnCancel"		[$WIN32]
		"navUp"					"BtnSelectMission"	[$X360]
		"navDown"				"DrpChapter"
		
		//button and label
		"BtnDropButton"
		{
			"ControlName"					"BaseModHybridButton"
			"fieldName"						"BtnDropButton"
			"xpos"							"0"
			"ypos"							"0"
			"zpos"							"2"
			"wide"							"280"
			"wideatopen"					"160"
			"tall"							"15"			[$WIN32]
			"tall"							"20"			[$X360]
			"autoResize"					"1"
			"pinCorner"						"0"
			"visible"						"1"
			"enabled"						"1"
			"tabPosition"					"0"
			"labelText"						"#ASUI_GameType"
			"tooltiptext"					"#ASUI_GameType_tt"
			"disabled_tooltiptext"			"#ASUI_GameType_tt"
			"style"							"DropDownButton"
			"command"						"FlmGameType"
			"ActivationType"				"1" [$X360]
			"EnableCondition"				"Never" [$DEMO]
		}
	}
	
	// Campaign flyout	
	"FlmGameType"
	{
		"ControlName"			"FlyoutMenu"
		"fieldName"				"FlmGameType"
		"visible"				"0"
		"wide"					"0"
		"tall"					"0"
		"zpos"					"4"
		"InitialFocus"			"BtnCampaign1"
		"ResourceFile"			"resource/UI/basemodui/DropDownGameType.res"
	}

	"DrpSelectMission"
	{
		"ControlName"			"DropDownMenu"
		"fieldName"				"DrpSelectMission"
		"xpos"					"c-250"	
		"ypos"					"165"			[$WIN32]
		"ypos"					"165"			[$X360]
		"zpos"					"1"
		"wide"					"280"	
		"tall"					"15"			[$WIN32]
		"tall"					"20"			[$X360]
		"visible"				"1"
		"enabled"				"1"
		"navUp"					"DrpGameType"
		"navDown"				"DrpDifficulty"
		
		// button and label
		"BtnDropButton"
		{
			"ControlName"					"BaseModHybridButton"
			"fieldName"						"BtnDropButton"
			"xpos"							"0"
			"ypos"							"0"
			"zpos"							"2"
			"wide"							"280"
			"wideatopen"					"160"
			"tall"							"15"			[$WIN32]
			"tall"							"20"			[$X360]
			"autoResize"					"1"
			"pinCorner"						"0"
			"visible"						"1"
			"enabled"						"1"
			"tabPosition"					"0"
			"labelText"						"#ASUI_Select_Mission"
			"tooltiptext"					""
			"disabled_tooltiptext"			""
			"style"							"DropDownButton"
			"command"						"cmd_change_mission"
			"ActivationType"				"1" [$X360]
		}
	}
	
	// starting mission in a campaign game
	"DrpStartingMission"
	{
		"ControlName"			"DropDownMenu"
		"fieldName"				"DrpStartingMission"
		"xpos"					"c-250"	
		"ypos"					"185"			[$WIN32]
		"ypos"					"165"			[$X360]
		"zpos"					"1"
		"wide"					"280"	
		"tall"					"15"			[$WIN32]
		"tall"					"20"			[$X360]
		"visible"				"1"
		"enabled"				"1"
		"navUp"					"DrpSelectMission"
		"navDown"				"DrpDifficulty"
		
		// button and label
		"BtnDropButton"
		{
			"ControlName"					"BaseModHybridButton"
			"fieldName"						"BtnDropButton"
			"xpos"							"0"
			"ypos"							"0"
			"zpos"							"2"
			"wide"							"280"
			"wideatopen"					"160"
			"tall"							"15"			[$WIN32]
			"tall"							"20"			[$X360]
			"autoResize"					"1"
			"pinCorner"						"0"
			"visible"						"1"
			"enabled"						"1"
			"tabPosition"					"0"
			"labelText"						"#ASUI_Select_Starting_Mission"
			"tooltiptext"					""
			"disabled_tooltiptext"			""
			"style"							"DropDownButton"
			"command"						"cmd_change_starting_mission"
			"ActivationType"				"1" [$X360]
		}
	}

	// Difficulty dropdown
	"DrpDifficulty"
	{
		"ControlName"			"DropDownMenu"
		"fieldName"				"DrpDifficulty"
		"xpos"					"c-250"	
		"ypos"					"215"			[$WIN32]
		"ypos"					"165"			[$X360]
		"zpos"					"1"
		"wide"					"280"	
		"tall"					"15"			[$WIN32]
		"tall"					"20"			[$X360]
		"visible"				"1"
		"enabled"				"1"
		"navUp"					"BtnSelectMission"
		"navDown"				"BtnStartGame"
		
		// button and label
		"BtnDropButton"
		{
			"ControlName"					"BaseModHybridButton"
			"fieldName"						"BtnDropButton"
			"xpos"							"0"
			"ypos"							"0"
			"zpos"							"2"
			"wide"							"280"
			"wideatopen"					"160"
			"tall"							"15"			[$WIN32]
			"tall"							"20"			[$X360]
			"autoResize"					"1"
			"pinCorner"						"0"
			"visible"						"1"
			"enabled"						"1"
			"tabPosition"					"0"
			"labelText"						"#L4D360UI_GameSettings_Difficulty"
			"tooltiptext"					"#L4D360UI_GameSettings_Tooltip_Difficulty"
			"disabled_tooltiptext"			"#L4D360UI_GameSettings_Tooltip_Difficulty_Disabled"
			"style"							"DropDownButton"
			"command"						"FlmDifficulty"
			"ActivationType"				"1" [$X360]
		}
	}

	// Difficulty flyout		
	"FlmDifficulty"
	{
		"ControlName"			"FlyoutMenu"
		"fieldName"				"FlmDifficulty"
		"visible"				"0"
		"wide"					"0"
		"tall"					"0"
		"zpos"					"4"
		"InitialFocus"			"BtnNormal"
		"ResourceFile"			"resource/UI/basemodui/DropDownDifficulty.res"
	}
	
	"DrpOnslaught"
	{
		"ControlName"			"DropDownMenu"
		"fieldName"				"DrpOnslaught"
		"xpos"					"c-250"	
		"ypos"					"235"			[$WIN32]
		"ypos"					"165"			[$X360]
		"zpos"					"1"
		"wide"					"280"	
		"tall"					"15"			[$WIN32]
		"tall"					"20"			[$X360]
		"visible"				"1"
		"enabled"				"1"
		"navUp"					"BtnSelectMission"
		"navDown"				"BtnStartGame"
		
		// button and label
		"BtnDropButton"
		{
			"ControlName"					"BaseModHybridButton"
			"fieldName"						"BtnDropButton"
			"xpos"							"0"
			"ypos"							"0"
			"zpos"							"2"
			"wide"							"280"
			"wideatopen"					"160"
			"tall"							"15"			[$WIN32]
			"tall"							"20"			[$X360]
			"autoResize"					"1"
			"pinCorner"						"0"
			"visible"						"1"
			"enabled"						"1"
			"tabPosition"					"0"
			"labelText"						"#L4D360UI_Onslaught"
			"tooltiptext"					""
			"disabled_tooltiptext"			""
			"style"							"DropDownButton"
			"command"						"FlmOnslaught"
			"ActivationType"				"1" [$X360]
		}
	}
		
	"FlmOnslaught"
	{
		"ControlName"			"FlyoutMenu"
		"fieldName"				"FlmOnslaught"
		"visible"				"0"
		"wide"					"0"
		"tall"					"0"
		"zpos"					"4"
		"InitialFocus"			"BtnNormal"
		"ResourceFile"			"resource/UI/basemodui/DropDownOnslaught.res"
	}
	
	"DrpFriendlyFire"
	{
		"ControlName"			"DropDownMenu"
		"fieldName"				"DrpFriendlyFire"
		"xpos"					"c-250"	
		"ypos"					"255"			[$WIN32]
		"ypos"					"165"			[$X360]
		"zpos"					"1"
		"wide"					"280"	
		"tall"					"15"			[$WIN32]
		"tall"					"20"			[$X360]
		"visible"				"1"
		"enabled"				"1"
		"navUp"					"BtnSelectMission"
		"navDown"				"BtnStartGame"
		
		// button and label
		"BtnDropButton"
		{
			"ControlName"					"BaseModHybridButton"
			"fieldName"						"BtnDropButton"
			"xpos"							"0"
			"ypos"							"0"
			"zpos"							"2"
			"wide"							"280"
			"wideatopen"					"160"
			"tall"							"15"			[$WIN32]
			"tall"							"20"			[$X360]
			"autoResize"					"1"
			"pinCorner"						"0"
			"visible"						"1"
			"enabled"						"1"
			"tabPosition"					"0"
			"labelText"						"#L4D360UI_FriendlyFire"
			"tooltiptext"					""
			"disabled_tooltiptext"			""
			"style"							"DropDownButton"
			"command"						"FlmFriendlyFire"
			"ActivationType"				"1" [$X360]
		}
	}
	
	"FlmFriendlyFire"
	{
		"ControlName"			"FlyoutMenu"
		"fieldName"				"FlmFriendlyFire"
		"visible"				"0"
		"wide"					"0"
		"tall"					"0"
		"zpos"					"4"
		"InitialFocus"			"BtnNormal"
		"ResourceFile"			"resource/UI/basemodui/DropDownFriendlyFire.res"
	}
	
	"IconForwardArrow"
	{
		"ControlName"			"ImagePanel"
		"fieldName"				"IconForwardArrow"
		"xpos"					"c-245"		[$WIN32]
		"xpos"					"c-275"		[$X360]	
		"ypos"					"185"		[$WIN32]	
		"ypos"					"215"		[$X360]		
		"wide"					"15"		[$WIN32]	       
		"wide"					"20"		[$X360]	    
		"tall"					"15"		[$WIN32]    
		"tall"					"20"		[$X360]    
		"scaleImage"			"1"
		"pinCorner"				"0"
		"visible"				"0"
		"enabled"				"1"
		"tabPosition"			"0"
		"image"					"icon_button_arrow_right"
		"scaleImage"			"1"
	}
	
	"BtnStart"
	{
		"ControlName"			"CNB_Button"
		"fieldName"				"BtnStart"
		"xpos"		"c147"
		"ypos"		"r23"
		"wide"		"117"
		"tall"		"27"
		"zpos"		"1"
		"visible"				"1"
		"enabled"				"1"
		"tabPosition"			"0"
		"labelText"				"#L4D360UI_StartGame"
		"command"				"StartGame"
		"textAlignment"		"center"
		"font"		"DefaultMedium"
		"fgcolor_override"		"113 142 181 255"
	}
	
	"DrpServerType"
	{
		"ControlName"			"DropDownMenu"
		"fieldName"				"DrpServerType"
		"xpos"					"c-250"
		"ypos"					"215"			[$WIN32]
		"ypos"					"210"			[$X360]
		"zpos"					"1"
		"wide"					"280"
		"tall"					"15"			[$WIN32]
		"tall"					"20"			[$X360]
		"visible"				"0"				[$WIN32]
		"visible"				"0"				[$X360]
		"enabled"				"1"
		"navUp"					"DrpGameAccess"
		"navDown"				"BtnStartLobby"
		
		//button and label
		"BtnDropButton"
		{
			"ControlName"			"BaseModHybridButton"
			"fieldName"				"BtnDropButton"
			"xpos"					"0"
			"ypos"					"0"
			"zpos"					"2"
			"wide"					"280"
			"wideatopen"			"160"
			"tall"					"15"			[$WIN32]
			"tall"					"20"			[$X360]
			"autoResize"			"1"
			"pinCorner"				"0"
			"visible"				"1"
			"enabled"				"1"
			"tabPosition"			"0"
			"labelText"				"#L4D360UI_Lobby_Change_ServerType"
			"tooltiptext"			"#L4D360UI_Lobby_Change_ServerType_Tip"
			"style"					"DropDownButton"
			"command"				"FlmServerType"
		}
	}
	
	"DrpGameAccess"
	{
		"ControlName"			"DropDownMenu"
		"fieldName"				"DrpGameAccess"
		"xpos"					"c-250"
		"ypos"					"285"			[$WIN32]
		"ypos"					"185"			[$X360]
		"zpos"					"1"
		"wide"					"280"
		"tall"					"15"			[$WIN32]
		"tall"					"20"			[$X360]
		"visible"				"1"
		"enabled"				"1"
		"navUp"					"DrpDifficulty"
		"navDown"				"DrpServerType"
		
		//button and label
		"BtnDropButton"
		{
			"ControlName"			"BaseModHybridButton"
			"fieldName"				"BtnDropButton"
			"xpos"					"0"
			"ypos"					"0"
			"zpos"					"2"
			"wide"					"280"
			"wideatopen"			"160"
			"tall"					"15"		[$WIN32]
			"tall"					"20"		[$X360]
			"autoResize"			"1"
			"pinCorner"				"0"
			"visible"				"1"
			"enabled"				"1"
			"tabPosition"			"0"
			"labelText"				"#L4D360UI_Lobby_Change_GameAccess"
			"tooltiptext"			"#L4D360UI_Lobby_Change_GameAccess_Tip"
			"disabled_tooltiptext"	"#L4D360UI_GameSettings_Tooltip_Access_Disabled"
			"style"					"DropDownButton"
			"command"				"FlmGameAccess"
			"ActivationType"		"1" [$X360]
		}
	}
		
	"FlmGameAccess"
	{
		"ControlName"			"FlyoutMenu"
		"fieldName"				"FlmGameAccess"
		"visible"				"0"
		"wide"					"0"
		"tall"					"0"
		"zpos"					"3"
		"InitialFocus"			"BtnFriends"
		"ResourceFile"			"resource/UI/basemodui/DropDownGameAccess.res"
	}
		
	"FlmServerType"
	{
		"ControlName"		"FlyoutMenu"
		"fieldName"			"FlmServerType"
		"visible"			"0"
		"wide"				"0"
		"tall"				"0"
		"zpos"				"3"
		"InitialFocus"		"BtnOfficial"
		"ResourceFile"		"resource/UI/basemodui/DropDownServerType.res"
	}
	
	"IconBackArrow" [$WIN32]
	{
		"ControlName"			"ImagePanel"
		"fieldName"				"IconBackArrow"
		"xpos"					"c-265"		
		"ypos"					"205"
		"wide"					"15"
		"tall"					"15"
		"scaleImage"			"1"
		"pinCorner"				"0"
		"visible"				"0"
		"enabled"				"1"
		"tabPosition"			"0"
		"image"					"icon_button_arrow_left"
		"scaleImage"			"1"
	}
	"BtnCancel" [$WIN32]
	{
		"ControlName"			"CNB_Button"
		"fieldName"				"BtnCancel"
		"xpos"		"c-264"
		"ypos"		"r23"
		"wide"		"117"
		"tall"		"27"
		"zpos"		"1"
		"visible"				"1"
		"enabled"				"1"
		"tabPosition"			"0"
		"labelText"				"#nb_back"
		"command"				"Back"
		"textAlignment"		"center"
		"font"		"DefaultMedium"
		"fgcolor_override"		"113 142 181 255"
	}
}
