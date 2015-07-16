"Resource/UI/StatsReport.res"
{
	"Background1"
	{
		"ControlName"		"Panel"
		"fieldName"		"Background1"
		"xpos"			"c-280"
		"ypos"			"75"
		"zpos"			"-1"
		"wide"			"560"
		"tall"			"100"

		"PaintBackgroundType"	"0"
		"bgcolor_override"	"35 41 57 192"
	}
	
	"Background2"
	{
		"ControlName"		"Panel"
		"fieldName"		"Background1"
		"xpos"			"c-280"
		"ypos"			"175"
		"zpos"			"-1"
		"wide"			"260"
		"tall"			"250"

		"PaintBackgroundType"	"0"
		"bgcolor_override"	"35 41 57 192"
	}
	
	"ObjectiveMap"      // local player's experience bar
	{
		"ControlName"		"ObjectiveMap"
		"fieldName"		"ObjectiveMap"
		"xpos"			"c-275"
		"ypos"			"170"
		"zpos"			"0"
		"wide"			"250"
		"tall"			"250"

		//"PaintBackgroundType"	"0"
		//"bgcolor_override"	"35 41 57 128"
	}
	
	"CategoryButton0"   // Friendly fire
	{
		"ControlName"		"Button"
		"fieldName"		"CategoryButton0"
		"xpos"			"c-280"
		"ypos"			"60"
		"zpos"			"0"
		"wide"			"260"
		"tall"			"15"

		"font"			"Default"
		"labelText"		"#asw_stats_ff"
	}
	"CategoryButton1"    // Kills
	{
		"ControlName"		"Button"
		"fieldName"		"CategoryButton1"
		"zpos"			"0"

		"font"			"Default"
		"labelText"		"#asw_stats_kills"
	}
	"CategoryButton2"    // Health
	{
		"ControlName"		"Button"
		"fieldName"		"CategoryButton2"
		"zpos"			"0"

		"font"			"Default"
		"labelText"		"#asw_stats_health"
	}
	"CategoryButton3"    // Ammo
	{
		"ControlName"		"Button"
		"fieldName"		"CategoryButton3"
		"zpos"			"0"

		"font"			"Default"
		"labelText"		"#asw_stats_ammo"
	}
	
	"PlayerNamesPosition"
	{
		"ControlName"		"Panel"
		"fieldName"		"PlayerNamesPosition"
		"xpos"			"c130"
		"ypos"			"75"
		"zpos"			"0"
		"wide"			"140"
		"tall"			"20"

		"textAlignment"   "west"
		"font"      "DefaultLarge"
		"fgcolor_override"	"255 255 255 0"
	}
	
	"PlayerName0"
	{
		"ControlName"		"Label"
		"fieldName"		"PlayerName0"
		"zpos"			"0"

		"textAlignment"   "west"
		"font"      "DefaultLarge"
		"fgcolor_override"	"255 255 255 255"
	}
	"PlayerName1"
	{
		"ControlName"		"Label"
		"fieldName"		"PlayerName1"
		"zpos"			"0"

		"textAlignment"   "west"
		"font"      "DefaultLarge"
		"fgcolor_override"	"255 255 255 255"
	}
	"PlayerName2"
	{
		"ControlName"		"Label"
		"fieldName"		"PlayerName2"
		"zpos"			"0"

		"textAlignment"   "west"
		"font"      "DefaultLarge"
		"fgcolor_override"	"255 255 255 255"
	}
	"PlayerName3"
	{
		"ControlName"		"Label"
		"fieldName"		"PlayerName3"
		"zpos"			"0"

		"textAlignment"   "west"
		"font"      "DefaultLarge"
		"fgcolor_override"	"255 255 255 255"
	}
	
	"ReadyCheckImage0"
	{
		"fieldName"		"ReadyCheckImage0"
		"ControlName"		"ImagePanel"
		"zpos"		"1"
		"scaleImage"		"1"
		"wide"		"12"
		"tall"		"12"
	}
	"ReadyCheckImage1"
	{
		"fieldName"		"ReadyCheckImage1"
		"ControlName"		"ImagePanel"
		"zpos"		"1"
		"scaleImage"		"1"
	}
	"ReadyCheckImage2"
	{
		"fieldName"		"ReadyCheckImage2"
		"ControlName"		"ImagePanel"
		"zpos"		"1"
		"scaleImage"		"1"
	}
	"ReadyCheckImage3"
	{
		"fieldName"		"ReadyCheckImage3"
		"ControlName"		"ImagePanel"
		"zpos"		"1"
		"scaleImage"		"1"
	}
	
	"AvatarImage0"
	{
		"fieldName"		"AvatarImage0"
		"zpos"		"0"
		"wide"		"16"
		"tall"		"16"
		"ControlName"		"CAvatarImagePanel"
		"scaleImage"		"1"
	}
	"AvatarImage1"
	{
		"fieldName"		"AvatarImage1"
		"zpos"		"0"
		"ControlName"		"CAvatarImagePanel"
		"scaleImage"		"1"
	}
	"AvatarImage2"
	{
		"fieldName"		"AvatarImage2"
		"zpos"		"0"
		"ControlName"		"CAvatarImagePanel"
		"scaleImage"		"1"
	}
	"AvatarImage3"
	{
		"fieldName"		"AvatarImage3"
		"zpos"		"0"
		"ControlName"		"CAvatarImagePanel"
		"scaleImage"		"1"
	}
	
	"StatGraphPlayer"
	{
		"ControlName"		"StatGraphPlayer"
		"fieldName"		"StatGraphPlayer"
		"xpos"			"c-275"
		"ypos"			"80"
		"zpos"			"0"
		"wide"			"400"
		"tall"			"90"

		//"PaintBackgroundType"	"0"
		//"bgcolor_override"	"35 41 57 192"
	}
	
	"Debrief"
	{
		"ControlName"		"DebriefTextPage"
		"fieldName"		"Debrief"
		"xpos"			"c-15"
		"ypos"			"180"
		"zpos"			"0"
		"wide"			"295"
		"tall"			"245"

		"PaintBackgroundType"	"0"
		"bgcolor_override"	"35 41 57 192"
	}
}