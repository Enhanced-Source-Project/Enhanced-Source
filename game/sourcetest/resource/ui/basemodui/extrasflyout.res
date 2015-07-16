"Resource/UI/ExtrasFlyout.res"
{
	"PnlBackground"
	{
		"ControlName"			"Panel"
		"fieldName"			"PnlBackground"
		"xpos"				"0"
		"ypos"				"0"
		"zpos"				"-1"
		"wide"				"156" [$ENGLISH]
		"wide"				"236" [!$ENGLISH]
		"tall"				"45" [$X360]
		"tall"				"45" [$WIN32]
		"visible"			"1"
		"enabled"			"1"
		"paintbackground"	"1"
		"paintborder"		"1"
	}

	"BtnCommentary"
	{
		"ControlName"				"BaseModHybridButton"
		"fieldName"				"BtnCommentary"
		"xpos"					"0"
		"ypos"					"0"
		"wide"					"150"
		"tall"					"20"
		"autoResize"				"1"
		"pinCorner"				"0"
		"visible"				"1"
		"enabled"				"1"
		"tabPosition"				"0"
		"navUp"					"BtnAddons"  [$WIN32]
		"navUp"					"BtnCredits" [$X360]
		"navDown"				"BtnCredits"
		"tooltiptext"				"#L4D360UI_Extras_Commentary_Tip"
		"labelText"				"#L4D360UI_Extras_Commentary"
		"style"					"FlyoutMenuButton"
		"command"				"DeveloperCommentary"
	}

	"BtnAddons"	[$WIN32]
	{
		"ControlName"			"BaseModHybridButton"
		"fieldName"					"BtnAddons"
		"xpos"					"0"
		"ypos"					"20"
		"wide"					"150"
		"tall"					"20"
		"autoResize"			"1"
		"pinCorner"				"0"
		"visible"				"0"
		"enabled"				"1"
		"tabPosition"			"0"
		"proportionalToParent"	"1"
		"navUp"					"BtnCommentary"
		"navDown"				"BtnCredits"
		"tooltiptext"			"#L4D360UI_Extras_Addons_Tip"
		"labelText"				"#L4D360UI_Extras_Addons"
		"style"					"FlyoutMenuButton"
		"command"				"Addons"
	}

	"BtnCredits"
	{
		"ControlName"			"BaseModHybridButton"
		"fieldName"				"BtnCredits"
		"xpos"					"0"
		"ypos"					"40"
		"wide"					"150"
		"tall"					"20"
		"autoResize"			"1"
		"pinCorner"				"0"
		"visible"				"0"
		"enabled"				"0"
		"tabPosition"			"0"
		"navUp"					"BtnAddons" [$WIN32]
		"navUp"					"BtnCommentary" [$X360]
		"navDown"				"BtnCommentary" 
		"tooltiptext"			"#L4D360UI_Extras_Credits_Tip"
		"labelText"				"#L4D360UI_Extras_Credits"
		"style"					"FlyoutMenuButton"
		"command"				"Credits"
	}


}