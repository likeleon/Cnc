if ($args.Length -eq 0)
{
	echo "Command list:"
	echo ""
	echo "  all    Builds the game and its development tools."
	echo ""
	$command = (Read-Host "Enter command").Split(' ', 2)
}
else
{
	$command = $args
}

if ($command -eq "dependencies")
{
	$msBuild = FindMSBuild
	$msBuildArguments = "/t:Rebuild /nr:false"
	if ($msBuild -eq $null)
	{
		echo "Unable to locate an appropriate version of MSBuild."
	}
	else
	{
		$targetSolution = "src/third_party/lpng162/projects/vstudio/vstudio.sln"
		$proc = Start-Process  $msbuild $msBuildArguments
	}
	cd src/third_party/lpng162/projects/vstudio
	cd ../../../../../
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