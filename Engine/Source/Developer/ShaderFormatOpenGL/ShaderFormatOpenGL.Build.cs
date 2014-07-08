// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ShaderFormatOpenGL : ModuleRules
{
	public ShaderFormatOpenGL(TargetInfo Target)
	{

		PrivateIncludePathModuleNames.Add("TargetPlatform");
		PrivateIncludePathModuleNames.Add("OpenGLDrv"); 

		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"ShaderCore",
				"ShaderCompilerCommon",
				"ShaderPreprocessor"
			}
			);

		AddThirdPartyPrivateStaticDependencies(Target, 
			"OpenGL",
			"HLSLCC"
			);

        if (Target.Platform == UnrealTargetPlatform.Linux)
        {
            AddThirdPartyPrivateStaticDependencies(Target, "SDL2");
        }
	}
}
