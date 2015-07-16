"Resource/UI/NB_Mission_Panel.res"
{
	"MissionPanel"
	{
		"fieldName"		"MissionPanel"
		"ControlName"		"EditablePanel"
		"xpos"		"0"
		"ypos"		"0"
		"wide"		"f0"
		"tall"		"f0"
		"zpos"		"50"
	}
	"BackButton"
	{
		"fieldName"		"BackButton"
		"xpos"		"c-264"
		"ypos"		"r23"
		"wide"		"117"
		"tall"		"27"
		"ControlName"		"CNB_Button"
		"labelText"		"#nb_back"
		"textAlignment"		"center"
		"font"		"DefaultMedium"
		"fgcolor_override"		"113 142 181 255"
		"zpos"		"3"
	}
	"RetriesLabel"
	{
		"fieldName"		"RetriesLabel"
		"xpos"		"c120"
		"ypos"		"7"
		"wide"		"150"
		"tall"		"14"
		"font"		"Default"
		"labelText"		"RetriesLabel"
		"textAlignment"		"north-east"
		"ControlName"		"Label"
		"fgcolor_override"	"47 79 111 255"
		"allcaps"		"1"
	}
	"objectivemap"
	{
		"fieldName"		"objectivemap"
		"ControlName"	"Panel"
		"xpos"			"c-126"
		"ypos"			"70"
		"wide"			"240"
		"tall"			"240"
	}
	"objectivelistbox"
	{
		"fieldName"		"objectivelistbox"
		"ControlName"	"Panel"
		"xpos"			"c-297"
		"ypos"			"70"
		"wide"			"166"
		"tall"			"240"
	}
	"objectivedetails"
	{
		"fieldName"		"objectivedetails"
		"ControlName"	"Panel"
		"xpos"			"c120"
		"ypos"			"70"
	}	
	"DifficultyIsland"
	{
		"fieldName"		"DifficultyIsland"
		"ControlName"	"Panel"
		"xpos"			"c-279"
		"ypos"			"320"
		"wide"			"558"
		"tall"			"120"
		"zpos"			"1"
	}	
	"DifficultyDescription"
	{
		"fieldName"		"DifficultyDescription"
		"ControlName"	"Label"
		"xpos"			"c60"
		"ypos"			"355"
		"wide"			"200"
		"tall"			"80"
		"wrap"			"1"
		"textAlignment"		"north-west"
		"fgcolor_override"		"83 148 192 255"
		"zpos"		"3"
	}
	// Difficulty
	"DrpDifficulty"
	{
		"ControlName"			"DropDownMenu"
		"fieldName"				"DrpDifficulty"
		"xpos"					"c-259"	
		"ypos"					"355"	
		"zpos"					"2"
		"wide"					"200"	
		"tall"					"15"
		"visible"				"1"
		"enabled"				"1"
		
		//button and label
		"BtnDropButton"
		{
			"ControlName"					"BaseModHybridButton"
			"fieldName"						"BtnDropButton"
			"xpos"							"0"
			"ypos"							"0"
			"zpos"							"3"
			"wide"							"200"
			"wideatopen"					"160"
			"tall"							"18"			[$WIN32]
			"tall"							"20"			[$X360]
			"autoResize"					"1"
			"pinCorner"						"0"
			"visible"						"1"
			"enabled"						"1"
			"tabPosition"					"0"
			"labelText"						"#nb_difficulty_title"
			"tooltiptext"					""
			"disabled_tooltiptext"			""
			"style"							"DropDownButton"
			"command"						"FlmDifficulty"
			"ActivationType"				"1" [$X360]
			"EnableCondition"				"Never" [$DEMO]
		}
	}
	
	// Campaign flyout	
	"FlmDifficulty"
	{
		"ControlName"			"FlyoutMenu"
		"fieldName"				"FlmDifficulty"
		"visible"				"0"
		"wide"					"0"
		"tall"					"0"
		"zpos"					"4"
		"InitialFocus"			"BtnCampaign1"
		"ResourceFile"			"resource/UI/basemodui/DropDownDifficulty.res"
	}
	
	// Friendly Fire
	"DrpFriendlyFire"
	{
		"ControlName"			"DropDownMenu"
		"fieldName"				"DrpFriendlyFire"
		"xpos"					"c-259"	
		"ypos"					"395"	
		"zpos"					"2"
		"wide"					"200"	
		"tall"					"15"
		"visible"				"1"
		"enabled"				"1"
		
		//button and label
		"BtnDropButton"
		{
			"ControlName"					"BaseModHybridButton"
			"fieldName"						"BtnDropButton"
			"xpos"							"0"
			"ypos"							"0"
			"zpos"							"3"
			"wide"							"200"
			"wideatopen"					"160"
			"tall"							"18"			[$WIN32]
			"tall"							"20"			[$X360]
			"autoResize"					"1"
			"pinCorner"						"0"
			"visible"						"1"
			"enabled"						"1"
			"tabPosition"					"0"
			"labelText"						"#nb_friendly_fire_title"
			"tooltiptext"					""
			"disabled_tooltiptext"			""
			"style"							"DropDownButton"
			"command"						"FlmFriendlyFire"
			"ActivationType"				"1" [$X360]
			"EnableCondition"				"Never" [$DEMO]
		}
	}
	
	// Campaign flyout	
	"FlmFriendlyFire"
	{
		"ControlName"			"FlyoutMenu"
		"fieldName"				"FlmFriendlyFire"
		"visible"				"0"
		"wide"					"0"
		"tall"					"0"
		"zpos"					"4"
		"InitialFocus"			"BtnCampaign1"
		"ResourceFile"			"resource/UI/basemodui/DropDownFriendlyFire.res"
	}
	
	// Onslaught
	"DrpOnslaught"
	{
		"ControlName"			"DropDownMenu"
		"fieldName"				"DrpOnslaught"
		"xpos"					"c-259"	
		"ypos"					"375"	
		"zpos"					"2"
		"wide"					"200"	
		"tall"					"15"
		"visible"				"1"
		"enabled"				"1"
		
		//button and label
		"BtnDropButton"
		{
			"ControlName"					"BaseModHybridButton"
			"fieldName"						"BtnDropButton"
			"xpos"							"0"
			"ypos"							"0"
			"zpos"							"3"
			"wide"							"200"
			"wideatopen"					"160"
			"tall"							"18"			[$WIN32]
			"tall"							"20"			[$X360]
			"autoResize"					"1"
			"pinCorner"						"0"
			"visible"						"1"
			"enabled"						"1"
			"tabPosition"					"0"
			"labelText"						"#nb_onslaught_title"
			"tooltiptext"					""
			"disabled_tooltiptext"			""
			"style"							"DropDownButton"
			"command"						"FlmOnslaught"
			"ActivationType"				"1" [$X360]
			"EnableCondition"				"Never" [$DEMO]
		}
	}
	
	// Campaign flyout	
	"FlmOnslaught"
	{
		"ControlName"			"FlyoutMenu"
		"fieldName"				"FlmOnslaught"
		"visible"				"0"
		"wide"					"0"
		"tall"					"0"
		"zpos"					"4"
		"InitialFocus"			"BtnCampaign1"
		"ResourceFile"			"resource/UI/basemodui/DropDownOnslaught.res"
	}
	
	// Fixed skill points
	"DrpFixedSkillPoints"
	{
		"ControlName"			"DropDownMenu"
		"fieldName"				"DrpFixedSkillPoints"
		"xpos"					"c-259"	
		"ypos"					"415"	
		"zpos"					"2"
		"wide"					"200"	
		"tall"					"18"
		"visible"				"1"
		"enabled"				"1"
		
		//button and label
		"BtnDropButton"
		{
			"ControlName"					"BaseModHybridButton"
			"fieldName"						"BtnDropButton"
			"xpos"							"0"
			"ypos"							"0"
			"zpos"							"2"
			"wide"							"200"
			"wideatopen"					"160"
			"tall"							"15"			[$WIN32]
			"tall"							"20"			[$X360]
			"autoResize"					"1"
			"pinCorner"						"0"
			"visible"						"1"
			"enabled"						"1"
			"tabPosition"					"0"
			"labelText"						"#nb_skill_points"
			"tooltiptext"					""
			"disabled_tooltiptext"			""
			"style"							"DropDownButton"
			"command"						"FlmFixedSkillPoints"
			"ActivationType"				"1" [$X360]
			"EnableCondition"				"Never" [$DEMO]
		}
	}
	
	// Campaign flyout	
	"FlmFixedSkillPoints"
	{
		"ControlName"			"FlyoutMenu"
		"fieldName"				"FlmFixedSkillPoints"
		"visible"				"0"
		"wide"					"0"
		"tall"					"0"
		"zpos"					"4"
		"InitialFocus"			"BtnCampaign1"
		"ResourceFile"			"resource/UI/DropDownFixedSkillPoints.res"
	}
}
