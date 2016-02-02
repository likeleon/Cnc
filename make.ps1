function FindMSBuild
{
	$msBuildVersions = @("14.0")
	foreach ($msBuildVersion in $msBuildVersions)
	{
		$key = "HKLM:\SOFTWARE\Microsoft\MSBuild\ToolsVersions\{0}" -f $msBuildVersion
		$property = Get-ItemProperty $key -ErrorAction SilentlyContinue
		if ($property -eq $null -or $property.MSBuildToolsPath -eq $null)
		{
			continue
		}
		$path = Join-Path $property.MSBuildToolsPath -ChildPath "MSBuild.exe"
		if (Test-Path $path)
		{
			return $path
		}
	}
	return $null
}

if ($args.Length -eq 0)
{
	echo "Command list:"
	echo ""
	echo "  all             Builds the game and its development tools."
	echo "  dependencies    Thid third-party libraries."
	echo ""
	$command = (Read-Host "Enter command").Split(' ', 2)
}
else
{
	$command = $args
}

if ($command -eq "all")
{
	$msBuild = FindMSBuild
	if ($msBuild -eq $null)
	{
		echo "Unable to locate an appropriate version of MSBuild."
	}
	else
	{
		$msBuildArguments = "Cnc.sln /t:Build /p:Configuration=Release /nr:false"
		$proc = Start-Process $msbuild $msBuildArguments -NoNewWindow -PassThru -Wait
		if ($proc.ExitCode -ne 0)
		{
			echo "Cnc build failed."
		}
		else
		{
			echo "Build succeeded."
		}
	}
}
elseif ($command -eq "dependencies")
{
	$msBuild = FindMSBuild
	if ($msBuild -eq $null)
	{
		echo "Unable to locate an appropriate version of MSBuild."
	}
	else
	{
		$lpngDir = "src/third_party/lpng162/projects/vstudio"
		$msBuildArguments = $lpngDir + "/vstudio.sln /t:zlib;libpng /p:Configuration=Release /nr:false"
		$proc = Start-Process $msbuild $msBuildArguments -NoNewWindow -PassThru -Wait
		if ($proc.ExitCode -ne 0)
		{
			echo "lpng162 build failed."
		}
		else
		{
			$configurations = @("Debug", "Release")
			foreach ($configuration in $configurations)
			{
				$sourceFiles = $lpngDir + "/Release/*.dll"
				$destinationFolder = "bin/" + $configuration
				if ((Test-Path $destinationFolder) -eq 0)
				{
					mkdir $destinationFolder
				}
				cp $sourceFiles $destinationFolder
			}
			echo "Dependencies build done and copied."
		}
	}
}
else
{
	echo ("Invalid command '{0}'" -f $command)
}

if ($args.Length -eq 0)
{
	echo "Press enter to continue."
	while ($true)
	{
		if ([System.Console]::KeyAvailable)
		{
			break;
		}
		Start-Sleep -Milliseconds 50
	}
}