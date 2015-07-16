"Resource/UI/Achievements.res"
{
	"Achievements"
	{
		"ControlName"	"Frame"
		"fieldName"		"Achievements"
		"xpos"			"0"
		"ypos"			"0"
		"wide"			"f0"
		"tall"			"480"	[$WIN32]
		"tall"			"380"	[$X360]
		//"autoResize"	"0"
		//"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"	"0"
		//"usetitlesafe"	"1"
	}
	
	"Title"
	{
		"fieldName"		"Title"
		"xpos"		"c-266"
		"ypos"		"56"
		"wide"		"150"
		"tall"		"19"
		"zpos"		"5"
		"font"		"DefaultExtraLarge"
		"textAlignment"		"west"
		"ControlName"		"Label"
		"labelText"		"#L4D360UI_My_Achievements"
		"fgcolor_override"		"224 224 224 255"
	}
	
	"TotalProgressBG" 
	{
		"ControlName"			"Panel"
		"fieldName"				"TotalProgressBG"
		"ypos"					"365"
		//"xpos"					"0"
		//"wide"					"f0"
		//"xpos"					"c-226"
		//"wide"					"450"	
		"xpos"					"c-90"
		"wide"					"180"
		"tall"					"30"
		"zpos"					"0"
		"bgcolor_override"		"0 0 0 192"
		"visible"				"0"
	}
	
	"ProTotalProgress" 
	{
		"ControlName"			"ContinuousProgressBar"
		"fieldName"				"ProTotalProgress"
		"xpos"					"c-212"	[$X360]
		"ypos"					"110"	[$X360]
		"wide"					"420"	[$X360]
		"xpos"					"c-85"	[$WIN32]
		"ypos"					"368"	[$WIN32]
		"wide"					"170"	[$WIN32]
		"zpos"					"1"
		"tall"					"14"
		"autoResize"			"1"
		"pinCorner"				"0"
		"visible"				"1"		[$WIN32]
		"visible"				"0"		[$X360]
		"enabled"				"1"
		"tabPosition"			"0"
		"proportionalToParent"	"1"
	}
	
	"LblComplete" 
	{
		"ControlName"			"Label"
		"fieldName"				"LblComplete"
		"xpos"					"c-212"	[$X360]
		"ypos"					"110"	[$X360]
		"xpos"					"c-100"	[$WIN32]
		"ypos"					"374"	[$WIN32]
		"wide"					"200"
		"zpos"					"1"
		"tall"					"24" [$WIN32]
		"autoResize"			"1"
		"pinCorner"				"0"
		"visible"				"0"		[$X360]
		"visible"				"1"		[$WIN32]
		"enabled"				"1"
		"tabPosition"			"0"
		"proportionalToParent"	"1"
		"textAlignment"			"center"
		"fgcolor_override"		"83 148 192 255"
	}

	"LblGamerscore" 
	{
		"ControlName"			"Label"
		"fieldName"				"LblGamerscore"
		"xpos"					"c-0"
		"ypos"					"80"	[$WIN32]
		"ypos"					"48"	[$X360]
		"wide"					"172"
		"zpos"					"1"
		"autoResize"			"1"
		"pinCorner"				"0"
		"visible"				"1" [$X360]
		"visible"				"0" [$WIN32]
		"enabled"				"1"
		"tabPosition"			"0"
		"proportionalToParent"	"1"
		"textAlignment"			"east"
		"Font"					"DefaultLarge"
	}
	
	"Divider1" [$WIN32]
	{
		"ControlName"			"ImagePanel"
		"fieldName"				"Divider1"
		"xpos"					"c-238"	
		"ypos"					"120"	
		"zpos"					"2"
		"wide"					"450"
		"tall"					"2"
		"autoResize"			"0"
		"pinCorner"				"0"
		"visible"				"0"
		"enabled"				"1"
		"tabPosition"			"0"
		"image"					"divider_gradient"
		"scaleImage"			"1"
	}

	"GplAwards"	[$X360]
	{
		"ControlName"			"GenericPanelList"
		"fieldName"				"GplAwards"
		"xpos"					"c-226"
		"ypos"					"90"
		"wide"					"450"	
		"tall"					"255"
		"zpos"					"1"
		"autoResize"			"1"
		"pinCorner"				"0"
		"visible"				"0"
		"enabled"				"1"
		"tabPosition"			"1"
		"proportionalToParent"	"1"
	}
	
	"GplAchievements"
	{
		"ControlName"			"GenericPanelList"
		"fieldName"				"GplAchievements"
		"xpos"					"c-226"
		"ypos"					"90"	[$X360]
		"ypos"					"90"	[$WIN32]
		"wide"					"450"	
		"tall"					"270"	[$WIN32]	
		"tall"					"255"	[$X360]	
		"zpos"					"1"
		"autoResize"			"1"
		"pinCorner"				"0"
		"visible"				"1"
		"enabled"				"1"
		"tabPosition"			"1"
		"proportionalToParent"	"1"
		
		"bgcolor_override" "0 0 0 192"
	}
	
	"Divider2" [$WIN32]
	{
		"ControlName"			"ImagePanel"
		"fieldName"				"Divider2"
		"xpos"					"c-238"	
		"ypos"					"388"	
		"zpos"					"2"
		"wide"					"450"
		"tall"					"2"
		"autoResize"			"0"
		"pinCorner"				"0"
		"visible"				"0"
		"enabled"				"1"
		"tabPosition"			"0"
		"image"					"divider_gradient"
		"scaleImage"			"1"
	}

	"PnlLowerGarnish" [$WIN32]
	{
		"ControlName"		"Panel"
		"fieldName"		"PnlLowerGarnish"
		"xpos"			"0"
		"ypos"			"r45"
		"zpos"			"-1"
		"wide"			"f0"
		"tall"			"45"
		"autoResize"		"1"
		"pinCorner"		"0"
		"visible"		"0"
		"enabled"		"1"
		"tabPosition"		"0"
		"proportionalToParent"	"1"
	}
    "IconBackArrow" [$WIN32]
	{
		"ControlName"			"ImagePanel"
		"fieldName"				"IconBackArrow"
		"xpos"					"c-195"
		"ypos"					"420"
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
