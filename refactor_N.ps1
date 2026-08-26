param(
    [string]$SourceDir = "C:\Users\선우\source\repos\NuNuEngine\NuNuEngine_SOURCE",
    [string]$FilterFile = "C:\Users\선우\source\repos\NuNuEngine\NuNuEngine_SOURCE\NuNuEngine_SOURCE.vcxitems.filters"
)

Write-Host "Starting refactoring NuNuEngine_SOURCE files to 'N' prefix and moving to physical folders..."

# 1. Patch the filters XML to replace "ya" with "N" in filenames
$xmlText = Get-Content $FilterFile -Raw
$xmlText = $xmlText -replace '\)ya', ')N'
Set-Content -Path $FilterFile -Value $xmlText -Encoding UTF8

# 2. Parse the patched filters XML
[xml]$filtersXml = Get-Content $FilterFile
$fileMapping = @{}

foreach ($itemGroup in $filtersXml.Project.ItemGroup) {
    if ($null -ne $itemGroup.ClCompile) {
        foreach ($node in $itemGroup.ClCompile) {
            $includePath = $node.Include.Replace('$(MSBuildThisFileDirectory)', '')
            $filter = $node.Filter
            if ($includePath -match '^N.*\.cpp$') {
                $fileMapping[$includePath] = @{ Filter = $filter; Name = $includePath; Type = "ClCompile" }
            }
        }
    }
    if ($null -ne $itemGroup.ClInclude) {
        foreach ($node in $itemGroup.ClInclude) {
            $includePath = $node.Include.Replace('$(MSBuildThisFileDirectory)', '')
            $filter = $node.Filter
            if ($includePath -match '^N.*\.h$') {
                $fileMapping[$includePath] = @{ Filter = $filter; Name = $includePath; Type = "ClInclude" }
            } elseif ($includePath -eq 'CommonInclude.h') {
                $fileMapping[$includePath] = @{ Filter = $filter; Name = $includePath; Type = "ClInclude" }
            }
        }
    }
}

# 3. Find and Move Files
$headerPathMap = @{}

foreach ($key in $fileMapping.Keys) {
    $map = $fileMapping[$key]
    $name = $map.Name
    $filter = $map.Filter
    
    $targetDirPath = Join-Path $SourceDir $filter
    if (-not (Test-Path $targetDirPath)) {
        New-Item -ItemType Directory -Path $targetDirPath -Force | Out-Null
    }
    
    $targetFilePath = Join-Path $targetDirPath $name
    
    # Search for the file anywhere in SourceDir
    $foundFiles = Get-ChildItem -Path $SourceDir -Recurse -Filter $name -File
    
    if ($foundFiles.Count -gt 0) {
        $sourceFile = $foundFiles[0].FullName
        if ($sourceFile -ne $targetFilePath) {
            Write-Host "Moving: $($foundFiles[0].Name) -> $filter\$name"
            Move-Item -Path $sourceFile -Destination $targetFilePath -Force
        }
    } else {
        Write-Host "WARNING: File not found: $name"
    }

    if ($name -match '\.h$') {
        $relativeHeaderPath = ($filter.Replace('\', '/') + '/' + $name)
        if ($filter -eq '') {
           $relativeHeaderPath = $name 
        }
        $headerPathMap[$name] = $relativeHeaderPath
    }
}

# 4. Rewrite XML files to include physical paths
$vcxitemsPath = "$SourceDir\NuNuEngine_SOURCE.vcxitems"
[xml]$vcx = Get-Content $vcxitemsPath

# We also need to rename 'ya' -> 'N' in .vcxitems before parsing
$vcxText = Get-Content $vcxitemsPath -Raw
$vcxText = $vcxText -replace '\)ya', ')N'
Set-Content -Path $vcxitemsPath -Value $vcxText -Encoding UTF8
[xml]$vcx = Get-Content $vcxitemsPath

# Process vcxitems
foreach ($itemGroup in $vcx.Project.ItemGroup) {
    if ($null -ne $itemGroup.ClCompile) {
        foreach ($node in $itemGroup.ClCompile) {
            $name = $node.Include.Replace('$(MSBuildThisFileDirectory)', '').Split('\')[-1]
            if ($fileMapping.ContainsKey($name)) {
                $filter = $fileMapping[$name].Filter
                if ($filter) {
                    $node.Include = "`$(MSBuildThisFileDirectory)$filter\$name"
                } else {
                    $node.Include = "`$(MSBuildThisFileDirectory)$name"
                }
            }
        }
    }
    if ($null -ne $itemGroup.ClInclude) {
        foreach ($node in $itemGroup.ClInclude) {
            $name = $node.Include.Replace('$(MSBuildThisFileDirectory)', '').Split('\')[-1]
            if ($fileMapping.ContainsKey($name)) {
                $filter = $fileMapping[$name].Filter
                if ($filter) {
                    $node.Include = "`$(MSBuildThisFileDirectory)$filter\$name"
                } else {
                    $node.Include = "`$(MSBuildThisFileDirectory)$name"
                }
            }
        }
    }
}
$vcx.Save($vcxitemsPath)

# Process filters XML
foreach ($itemGroup in $filtersXml.Project.ItemGroup) {
    if ($null -ne $itemGroup.ClCompile) {
        foreach ($node in $itemGroup.ClCompile) {
            $name = $node.Include.Replace('$(MSBuildThisFileDirectory)', '').Split('\')[-1]
            if ($fileMapping.ContainsKey($name)) {
                $filter = $fileMapping[$name].Filter
                if ($filter) {
                    $node.Include = "`$(MSBuildThisFileDirectory)$filter\$name"
                } else {
                    $node.Include = "`$(MSBuildThisFileDirectory)$name"
                }
            }
        }
    }
    if ($null -ne $itemGroup.ClInclude) {
        foreach ($node in $itemGroup.ClInclude) {
            $name = $node.Include.Replace('$(MSBuildThisFileDirectory)', '').Split('\')[-1]
            if ($fileMapping.ContainsKey($name)) {
                $filter = $fileMapping[$name].Filter
                if ($filter) {
                    $node.Include = "`$(MSBuildThisFileDirectory)$filter\$name"
                } else {
                    $node.Include = "`$(MSBuildThisFileDirectory)$name"
                }
            }
        }
    }
}
$filtersXml.Save($FilterFile)

# 5. Fix #include directives in all source files
$allSourceDirs = @("C:\Users\선우\source\repos\NuNuEngine\NuNuEngine_SOURCE", "C:\Users\선우\source\repos\NuNuEngine\Editor_Window", "C:\Users\선우\source\repos\NuNuEngine\NuNuEngine_Window")

foreach ($dir in $allSourceDirs) {
    $files = Get-ChildItem -Path $dir -Include *.cpp, *.h -Recurse -File
    foreach ($file in $files) {
        $content = Get-Content $file.FullName -Raw
        $originalContent = $content
        
        # We only need to fix includes of N*.h to use the new folder paths.
        foreach ($name in $headerPathMap.Keys) {
            $newPath = $headerPathMap[$name]
            $escapedName = [regex]::Escape($name)
            # Find `#include ".../NName.h"` or `#include "NName.h"` and replace with `#include "Filter/NName.h"`
            $content = $content -replace "#include\s+`"[^`"]*$escapedName`"", "#include `"$newPath`""
        }
        
        if ($content -cne $originalContent) {
            Set-Content -Path $file.FullName -Value $content -Encoding UTF8
        }
    }
}

# Clean empty directories
Get-ChildItem -Path $SourceDir -Recurse -Directory | Where-Object { (Get-ChildItem -Path $_.FullName) -eq $null } | Remove-Item -Force

Write-Host "Refactoring Complete!"
