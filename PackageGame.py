#!/usr/bin/env python3
import os
import subprocess
import sys
import platform

#iterate through levels
def iterateLevels(LevelPath):
    lvllst = []

    for r, d, f in os.walk(LevelPath):
        for file in range (len(f)):
            if not f[file].lower().endswith((".uasset")):
                   lvllst.append(f[file].rsplit(".", 1)[0])
    return lvllst

def BuildAllLevels(Path : str, ProjectPath : str, LvlNames : list):
        #run UE Command to Build Lighting
    for f in range(0, len(LvlNames)):
        print('Rebuilding '+LvlNames[f]+' map')
        subprocess.run([ProjectPath + LvlNames[f], '-AutomatedMapBuild CLDesc="Rebuilding '+LvlNames[f]+' map" UseSCC=true'],-1,Path )
       
def PackageGame(Path : str, ProjectPath :str):
    PackageDir = None    
    SplitPath = ProjectPath.split("\\")
    ProjName = SplitPath[len(SplitPath) - 1]
    ProjectParent = ProjectPath.split(ProjName)
    ProjectParent = ProjectParent[0]
    ProjectParent = ProjectParent.replace('"', "")
    if not os.path.exists(ProjectParent+"\\PackagedGame"):
        os.makedirs('{}'.format(ProjectParent+"\\PackagedGame"))
    PackageDir = ProjectParent + "\\PackagedGame"
    ProjName = ProjName.replace('.uproject"', "")
    print("Packaging", ProjName)
    print(Path, "BuildCookRun", "-project="+ProjectPath, "-platform=Win64", "-clientconfig=Shipping")
    subprocess.run([Path, "BuildCookRun", "-project="+ProjectPath, "-Platform=Win64", "-clientconfig=Shipping", "-stage", "-pak", "-build=True", "-cook", "-nodebuginfo","-package", "-distribution", "-stagingdirectory="+PackageDir])

#main
def main():

    result = []
    AutomationToolExe = None
    DetectedPlatform = platform.system()
    if len(sys.argv) != 3:
        print('help: python PackageGame.py  UE/Project/Path Path/To/Maps\n')
        print(r'Ex: python PackageGame.py "D:\path\to\uproject.uproject" "D:\path\to\maps"')
        exit()

    print("Which version of Unreal Engine does this project use?")
    print("ex. 5.0")
    UEVersion = input("> ")
    UserInput = UEVersion.split(".")
    if(len(UserInput) > 2):
        UEVersion = str(UserInput[0]+ "."+UserInput[1])
        print(UEVersion)

    #Windows
    if(DetectedPlatform.__contains__("Windows")):
        if  UEVersion.__contains__("4."):
            exepath = "C:\\Program Files\\Epic Games\\UE_{}\\Engine\\Binaries\\Win64"
            print("Searching For UE4 Executable")
            for r,d,f in os.walk(exepath):
                if "UE4Editor.exe" in f:
                 print("UE4 Result Found")
                 result.append(os.path.join(r, "UE4Editor.exe"))
                 UEPath = result[0]
                 AutomationToolExe = "C:\\Program Files\\Epic Games\\UE_{}\\Engine\\Build\\BatchFiles\\RunUAT".format(UEVersion)
                 break
        elif  UEVersion.__contains__("5."):
            print("Searching For UE5 Executable")
            exepath = "C:\\Program Files\\Epic Games\\UE_{}\\Engine\\Binaries\\Win64".format(UEVersion)
            for r,d,f in os.walk(exepath):
                if "UnrealEditor.exe" in f:
                 print("UE5 Result Found")

                 result.append(os.path.join(r, "UnrealEditor.exe"))
                 UEPath = result[0]
                 AutomationToolExe = "C:\\Program Files\\Epic Games\\UE_{}\\Engine\\Build\\BatchFiles\\RunUAT".format(UEVersion)
                 break
        else:
            print("\nThis program only works with UE4 or UE5\n Now Exiting!")
            exit()
        AutomationToolExe = AutomationToolExe + ".bat"
    #MacOS
    elif (DetectedPlatform.__contains__("Darwin")): #Whatever MacOS is here
        if  UEVersion.__contains__("4."):
            exepath = "/Users/Shared/Epic Games/UE_{}/Engine/build/batchfiles/Mac/".format(UEVersion)
            print("Searching For UE4 Executable")
            for r,d,f in os.walk(exepath):
                if "UE4Editor.exe" in f:
                 print("UE4 Result Found")
                 result.append(os.path.join(r, "UE4Editor.exe"))
                 UEPath = result[0]
                 AutomationToolExe = "/Users/Shared/Epic Games/UE_{}/Engine/build/batchfiles/Mac/".format(UEVersion)
                 break
        elif  UEVersion.__contains__("5."):
            print("Searching For UE5 Executable")
            exepath = "/Users/Shared/Epic Games/UE_{}/Engine/build/batchfiles/Mac/".format(UEVersion)
            for r,d,f in os.walk(exepath):
                if "UnrealEditor.exe" in f:
                 print("UE5 Result Found")

                 result.append(os.path.join(r, "UnrealEditor.exe"))
                 UEPath = result[0]
                 AutomationToolExe = "/Users/Shared/Epic Games/UE_{}/Engine/build/batchfiles/Mac/".format(UEVersion)
                 break
        else:
            print("\nThis program only works with UE4 or UE5\n Now Exiting!")
            exit()
        AutomationToolExe = AutomationToolExe + ".sh"
    ##Some flavour of Linux
    #elif(DetectedPlatform.__contains__("Linux")): #Whatever Unix is here
    #    if  UEVersion.__contains__("4."):
    #        exepath = "~/Unreal Engine/UE_{}/Engine/Binaries/Win64"
    #        print("Searching For UE4 Executable")
    #        for r,d,f in os.walk(exepath):
    #            if "UE4Editor.exe" in f:
    #             print("UE4 Result Found")
    #             result.append(os.path.join(r, "UE4Editor.exe"))
    #             UEPath = result[0]
    #             AutomationToolExe = "C:\\Program Files\\Epic Games\\UE_{}\\Engine\\Build\\BatchFiles\\RunUAT"
    #             break
    #    elif  UEVersion.__contains__("5."):
    #        print("Searching For UE5 Executable")
    #        exepath = "C:\\Program Files\\Epic Games\\UE_{}\\Engine\\Binaries\\Win64".format(UEVersion)
    #        for r,d,f in os.walk(exepath):
    #            if "UnrealEditor.exe" in f:
    #             print("UE5 Result Found")
    #             result.append(os.path.join(r, "UnrealEditor.exe"))
    #             UEPath = result[0]
    #             AutomationToolExe = "C:\\Program Files\\Epic Games\\UE_{}\\Engine\\Build\\BatchFiles\\RunUAT".format(UEVersion)
    #             break
    #    else:
    #        print("Current Platform not supported by this build tool!")
    #        exit()
    #    AutomationToolExe = AutomationToolExe + ".sh"
    else:
        print("Current Platform not supported by this build tool!")
        exit()
   



    

    UEProjectPath = '"' + sys.argv[1] + '"'
    LevelsPath = sys.argv[2]
    LevelNames = iterateLevels(LevelsPath)
    
    BuildAllLevels(UEPath, UEProjectPath, LevelNames)
    PackageGame(AutomationToolExe, UEProjectPath)





#run main

main()