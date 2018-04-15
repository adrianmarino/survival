using UnrealBuildTool;
using System.Collections.Generic;

public class SurvivalTarget : TargetRules
{
	public SurvivalTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.Add("Survival");
	}
}
