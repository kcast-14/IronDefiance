# IronDefiance

## Developed with Unreal Engine 5

### How to build the project (Fresh Repo Clone)

- Step 1: Right click IronDefiance.uproject
- Step 2: In the dropdown menu, navigate to _Show more options_ at the bottom of the dropdown menu and click it
- Step 3: A new dropdown menu will appear and you should then navigate to _Generate Visual Studio project files_ and click that
This will cause Unreal Engine to generate the necessary Visual Studio solution as well as compile the project once this is complete you can move onto step 4.

- Step 4: If you are not a programmer who is writing C++ code then follow step _4a_ otherwise follow step _4b_
    > 4a: Finally, double-click IronDefiance.uproject to open the project in the editor and you're free to begin your work!

    > 4b: Double-click the IronDefiance.sln file to open the visual studio solution, you can hit F5 on your keyboard to build the project and launch the Editor.

### How to pull changes made to the repo without overwriting your changes
In order to keep things consistent it is required that you fetch the most recent version of the repo before you start any work that involves Unreal Engine/Blueprints/C++ etc 

The following steps are broken into subcategories. Category a are the steps for those using Github Desktop, category b are for those using git in the terminal or command prompt. Category b follows the same rules as category a therefore category b will only have the commands that you need to use.

- Step 1:

    > 1a: First you'll want to ensure that you're working in the _Development_ branch. Towards the top of the window you should see a bar containing the current branch you're in, if this doesn't say _Development_ you'll want to click the dropdown arrow and switch to the correct branch.

    > 1b: git branch Development

- Step 2:

    > 2a: Next you'll want to commit your changes to the repo by navigating to the bottom left corner and filling out a summary of the changes that you've made.

    > 2b: git add . 
    ***Note: If you've added or created new files you may have to add those files manually using the command git add /path/to/new/file***
    > 2b: git commit -m "MESSAGE GOES HERE"


- Step 3: 

    > 3a: Third, in the same bar where you found the current branch, next to it should be a button with a dropdown arrow on it as well, you'll want to click that arrow and click the option that says _fetch_

    > 3b: git pull

- Step 4:

    > 4a: Now you're ready to push your changes to the repo.

    > 4b: git push


### How to package the game for playtesting

If you're using windows then you'll want to double click _BuildPackagedGame-Windows.bat_ and allow that script to run.

If you're on MacOS you'll want to open your terminal and navigate to the directory that houses the Iron Defiance project and enter the command ```./BuildPackagedGame-MacOS.sh```

The packaged game will output to a directory called _PackagedGame_ within the same directory that the Iron Defiance project is in.


