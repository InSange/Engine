param(
    [string]$SourceDir = "c:\Users\선우\source\repos\NuNuEngine\NuNuEngine_SOURCE",
    [string]$WindowDir = "c:\Users\선우\source\repos\NuNuEngine\NuNuEngine_Window"
)

$globalIncludeMap = @{}

function Process-Filters($filtersPath, $baseDir, $isShared) {
    [xml]$xml = Get-Content -Raw $filtersPath
    $ns = new-object Xml.XmlNamespaceManager $xml.NameTable
    $ns.AddNamespace("msb", "http://schemas.microsoft.com/developer/msbuild/2003")

    $items = $xml.SelectNodes("//msb:ClInclude | //msb:ClCompile", $ns)
    $localMap = @{}

    foreach ($item in $items) {
        $includePath = $item.Include
        if (-not $includePath) { continue }
        $cleanPath = $includePath -replace "^\`$\(MSBuildThisFileDirectory\)", ""
        $filename = [System.IO.Path]::GetFileName($cleanPath)
        $filterNode = $item.SelectSingleNode("msb:Filter", $ns)
        
        if ($filterNode) {
            $filterPath = $filterNode.InnerText.Trim()
            $localMap[$filename] = $filterPath
            
            $unixFilterPath = $filterPath.Replace('\', '/')
            $globalIncludeMap[$filename] = "$unixFilterPath/$filename"
        }
    }
    return $localMap
}

function Move-FilesAndUpdateXML($baseDir, $localMap, $xmlPaths, $isShared) {
    foreach ($filename in $localMap.Keys) {
        $filterPath = $localMap[$filename]
        $sourcePath = Join-Path $baseDir $filename
        $targetDir = Join-Path $baseDir $filterPath
        $targetPath = Join-Path $targetDir $filename
        
        if (Test-Path $sourcePath) {
            if (-not (Test-Path $targetDir)) {
                New-Item -ItemType Directory -Path $targetDir -Force | Out-Null
            }
            Move-Item -Path $sourcePath -Destination $targetPath -Force
        }
    }

    foreach ($xmlPath in $xmlPaths) {
        $content = Get-Content -Raw $xmlPath
        foreach ($filename in $localMap.Keys) {
            $filterPath = $localMap[$filename]
            if ($isShared) {
                $escapedFilename = [regex]::Escape($filename)
                $replacement = '$$' + "(MSBuildThisFileDirectory)$filterPath\$filename"
                $content = [regex]::Replace($content, "\`$\(MSBuildThisFileDirectory\)$escapedFilename\b", $replacement)
            } else {
                $escapedFilename = [regex]::Escape($filename)
                $replacement = "Include=`"$filterPath\$filename`""
                $content = [regex]::Replace($content, "Include=`"$escapedFilename\`"", $replacement)
            }
        }
        Set-Content -Path $xmlPath -Value $content -Encoding UTF8
    }
}

$sourceMap = Process-Filters "$SourceDir\NuNuEngine_SOURCE.vcxitems.filters" $SourceDir $true
$windowMap = Process-Filters "$WindowDir\NuNuEngine_Window.vcxproj.filters" $WindowDir $false

Move-FilesAndUpdateXML $SourceDir $sourceMap @("$SourceDir\NuNuEngine_SOURCE.vcxitems", "$SourceDir\NuNuEngine_SOURCE.vcxitems.filters") $true
Move-FilesAndUpdateXML $WindowDir $windowMap @("$WindowDir\NuNuEngine_Window.vcxproj", "$WindowDir\NuNuEngine_Window.vcxproj.filters") $false

$windowProj = "$WindowDir\NuNuEngine_Window.vcxproj"
$projContent = Get-Content -Raw $windowProj
if ($projContent -notmatch "<AdditionalIncludeDirectories>\`$\(ProjectDir\)") {
    $projContent = $projContent -replace "<AdditionalIncludeDirectories>\.\.\\NuNuEngine_SOURCE", "<AdditionalIncludeDirectories>`$(ProjectDir);..\NuNuEngine_SOURCE"
    Set-Content -Path $windowProj -Value $projContent -Encoding UTF8
}

$allSrcFiles = Get-ChildItem -Path $SourceDir, $WindowDir -Include *.h, *.cpp -Recurse -File
foreach ($file in $allSrcFiles) {
    if ($file.Extension -match "\.(h|cpp)$") {
        $content = Get-Content -Raw $file.FullName
        $modified = $false
        
        $pattern = '#include\s+"([^"]+)"'
        $matches = [regex]::Matches($content, $pattern)
        
        foreach ($m in $matches) {
            $includeStr = $m.Groups[1].Value
            $incFilename = [System.IO.Path]::GetFileName($includeStr)
            
            if ($globalIncludeMap.ContainsKey($incFilename)) {
                $newInclude = $globalIncludeMap[$incFilename]
                if ($includeStr -ne $newInclude) {
                    $escapedIncludeStr = [regex]::Escape($includeStr)
                    $content = [regex]::Replace($content, "#include\s+`"$escapedIncludeStr`"", "#include `"$newInclude`"")
                    $modified = $true
                }
            }
        }
        if ($modified) {
            Set-Content -Path $file.FullName -Value $content -Encoding UTF8
        }
    }
}
Write-Output "Refactoring complete."
