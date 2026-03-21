param(
    [string]$SourceDir = "C:\Users\선우\source\repos\NuNuEngine\NuNuEngine_SOURCE",
    [string]$FilterFile = "C:\Users\선우\source\repos\NuNuEngine\NuNuEngine_SOURCE\NuNuEngine_SOURCE.vcxitems.filters"
)

Write-Host "Starting refactoring NuNuEngine_SOURCE files to 'ya' prefix and moving to physical folders..."

[xml]$filtersXml = Get-Content $FilterFile
$ns = @{ default = "http://schemas.microsoft.com/developer/msbuild/2003" }

# 1. Collect all intended files and their target filters
$fileMapping = @{} # Dictionary of NewName -> @{ Filter=..., OldName=... }

foreach ($itemGroup in $filtersXml.Project.ItemGroup) {
    if ($null -ne $itemGroup.ClCompile) {
        foreach ($node in $itemGroup.ClCompile) {
            $includePath = $node.Include.Replace('$(MSBuildThisFileDirectory)', '')
            $filter = $node.Filter
            if ($includePath -match '^ya.*\.cpp$') {
                $oldName = $includePath -replace '^ya', 'N'
                $fileMapping[$includePath] = @{ Filter = $filter; OldName = $oldName; NewName = $includePath; Type = "ClCompile" }
            }
        }
    }
    if ($null -ne $itemGroup.ClInclude) {
        foreach ($node in $itemGroup.ClInclude) {
            $includePath = $node.Include.Replace('$(MSBuildThisFileDirectory)', '')
            $filter = $node.Filter
            if ($includePath -match '^ya.*\.h$') {
                $oldName = $includePath -replace '^ya', 'N'
                $fileMapping[$includePath] = @{ Filter = $filter; OldName = $oldName; NewName = $includePath; Type = "ClInclude" }
            } elseif ($includePath -eq 'CommonInclude.h') {
                $fileMapping[$includePath] = @{ Filter = $filter; OldName = $includePath; NewName = $includePath; Type = "ClInclude" }
            }
        }
    }
}

# 2. Find and Move Files
$headerPathMap = @{} # OldName -> "Filter/NewName" for #include updates

foreach ($key in $fileMapping.Keys) {
    $map = $fileMapping[$key]
    $oldName = $map.OldName
    $newName = $map.NewName
    $filter = $map.Filter
    
    $targetDirPath = Join-Path $SourceDir $filter
    if (-not (Test-Path $targetDirPath)) {
        New-Item -ItemType Directory -Path $targetDirPath | Out-Null
    }
    
    $targetFilePath = Join-Path $targetDirPath $newName
    
    # Search for the old file
    $foundFiles = Get-ChildItem -Path $SourceDir -Recurse -Filter $oldName -File
    if ($foundFiles.Count -eq 0) {
        # Check if it was already renamed to newName
        $foundFiles = Get-ChildItem -Path $SourceDir -Recurse -Filter $newName -File
    }
    
    if ($foundFiles.Count -gt 0) {
        $sourceFile = $foundFiles[0].FullName
        if ($sourceFile -ne $targetFilePath) {
            Write-Host "Moving & Renaming: $($foundFiles[0].Name) -> $filter\$newName"
            Move-Item -Path $sourceFile -Destination $targetFilePath -Force
        }
    } else {
        Write-Host "WARNING: File not found: $oldName or $newName"
    }

    if ($newName -match '\.h$') {
        $relativeHeaderPath = ($filter.Replace('\', '/') + '/' + $newName)
        if ($filter -eq '') {
           $relativeHeaderPath = $newName 
        }
        $headerPathMap[$oldName] = $relativeHeaderPath
    }
}

# 3. Rewrite actual Project Files (.vcxitems & .vcxitems.filters) to use physical paths
Write-Host "Rewriting XML files to include physical paths..."

# Create new .vcxitems XML
$vcxitemsPath = "$SourceDir\NuNuEngine_SOURCE.vcxitems"
[xml]$vcx = Get-Content $vcxitemsPath

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

# Create new .vcxitems.filters XML
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

# 4. Global string replacement in all .cpp and .h files for old includes and namespaces
Write-Host "Updating #include directives in all source files..."
$allSourceDirs = @("C:\Users\선우\source\repos\NuNuEngine\NuNuEngine_SOURCE", "C:\Users\선우\source\repos\NuNuEngine\Editor_Window", "C:\Users\선우\source\repos\NuNuEngine\NuNuEngine_Window")

foreach ($dir in $allSourceDirs) {
    $files = Get-ChildItem -Path $dir -Include *.cpp, *.h -Recurse -File
    foreach ($file in $files) {
        $content = Get-Content $file.FullName -Raw
        $originalContent = $content
        
        # Replace namespace NuNu -> ya (assuming they want to revert fully to tutorial naming, wait, user uses ya::math etc)
        # We will only replace NuNu:: with ya:: and namespace NuNu with namespace ya
        $content = $content -replace '\bNuNu\b', 'ya'
        
        # Replace class names globally (NApplication -> yaApplication)
        foreach ($oldName in $headerPathMap.Keys) {
            $newName = $headerPathMap[$oldName]
            $baseOld = $oldName.Replace('.h', '')
            $baseNew = $newName.Split('/')[-1].Replace('.h', '')
            
            # Replace class/struct usage
            $content = $content -replace "\b$baseOld\b", "$baseNew"
        }

        # Fix #includes
        # E.g. #include "NApplication.h" -> #include "High Level Interface/yaApplication.h"
        # Since we use regex, we'll match `#include ".*?N(\w+)\.h"` and replace according to map
        foreach ($oldName in $headerPathMap.Keys) {
            $newPath = $headerPathMap[$oldName]
            $escapedOldName = [regex]::Escape($oldName)
            $content = $content -replace "#include\s+`"[^`"]*$escapedOldName`"", "#include `"$newPath`""
        }
        
        # NGraphicDevice_DX11.cpp is special, user's snippets showed it was partially renamed or not renaming file?
        # User snippet: <ClCompile Include="$(MSBuildThisFileDirectory)yaGraphicDevice_DX11.cpp">
        
        if ($content -cne $originalContent) {
            Set-Content -Path $file.FullName -Value $content -Encoding UTF8
        }
    }
}

# Clean empty directories in NuNuEngine_SOURCE
Get-ChildItem -Path $SourceDir -Recurse -Directory | Where-Object { (Get-ChildItem -Path $_.FullName) -eq $null } | Remove-Item -Force

Write-Host "Refactoring Complete!"
