Overview ===================================

Module	Name					Primary Role			Secondary Role			Championing Roles
-----------------------------------------------------------------------------------------------------------------------------------------------------------
RTIS	ANG Bryan Wei Ze			Technical Lead								Engine Champion
RTIS	BINTE MOHD ALI Hafawati 		Programmer								Graphics Champion
RTIS	GOH Pearl Jia Tong			Programmer									Level Editor Champion
IMGD	NG Jun Heng Shawn			Programmer									Gameplay Champion
IMGD	RIO Shannon Yvon Leonardo		Programmer								Physics/Collision Champion
IMGD	TAY Val Yun Ying			Product Manager			Programmer 			Production/Input Champion
UXGD	TANG Xian Hui				Design Lead									Design/Story Champion
BFA	HO Chew Yuan					Art Lead									Art Champion


Demo Input/Usage ===================================

// Graphics //
- Press 'Q' to enable/disable collider drawing
- Mouse scroll to zoom in and out
- Hold mouse right button to pan the camera
- Use mouse left button select entity in scene viewport using cursor
- Use ImGuizmo to scale/rotate/translate selected entity

// Prototype //
- Press "R,F,V" keys to move character position


User Interface ===================================

File
-----
- Located at the top left, able to load, add and save scenes


Hierarchy Section
-----------------

- Add entity by clicking the "Add Entity" button
- Click on any current entities to show components of the entity in the inspector tab

Asset Store
------------
- Click on the respective asset type to display the files in the asset folder
- Add Asset by clicking the "Add asset" button while inside the directory of the respective asset (ONLY audio and textures)
- Delete Asset by clicking the "Delete asset" button while a file is selected (file name will turn pink instead of white when selected)
- Reset ALL meta files by clicking the "Total Wipe" button

Layer
------
- Click on "Add Layer" to add a layer
- Able to change the order of rendering; order '0' is rendered first 
- Able to delete layer under "Actions"

Layer Assignment
----------------
- Assign entity to it's respective layer

Scene
-----
- Mouse scroll to zoom in and out
- Hold mouse middle/right button to pan the camera
- Use left mouse button select an entity in the scene viewport
- Use the toolbar to select the ImGuizmo to be used on selected entity

Game
-----
- Able to play, pause and stop the gameplay

Inspector
----------
- Able to add component(text, script or audio)
- Able to edit component's data
- Audio components can be paused, stopped and played via buttons. Different sound tracks can be played.
- Animation component can be played and stopped, animation state can be changed.

Prefab Editor
-------------
- Able to add component(text, script or audio)
- Able to edit component's data
- Changes to a prefab's components data (except for transform) will reflect to instances across multiple scenes

Console
-------
- Shows the debug and info statements

Performance Viewer
-------------------
- Shows performance for engine and different system

Performance Testing
--------------------
- Add and clear entity 


Project =============================================

Team_Popplio Project Settings (Debug, Release and Release-Game)
-------------------------------------------------------------------------
Output Directory: $(SolutionDir)\bin\$(Configuration)-$(Platform)\
Intermediate Directory: $(SolutionDir)\.tmp\$(Configuration)-$(Platform)\
Working Directory: $(SolutionDir)\bin\$(Configuration)-$(Platform)\
C++ Language Standard: ISO C++20 Standard

Team_Popplio_PreDLL Project Settings (Debug, Release and Release-Game)
-------------------------------------------------------------------------
Output Directory: $(SolutionDir)\bin\$(Configuration)-$(Platform)\
Intermediate Directory: $(SolutionDir)\.tmp\$(Configuration)-$(Platform)\
Working Directory: $(ProjectDir)
C++ Language Standard: ISO C++20 Standard

Level Editor Toggling
-------------------------------------------------------------------------
Press 'F11' to disable level editor
Press 'Spacebar' to toggle level editor (Turn off / on simulation)

Window Mode Toggling
-------------------------------------------------------------------------
Press 'F1' to toggle between windowed and fullscreen mode

TroubleShooting (!!!!!!!!!READ ME!!!!!!!)
-------------------------------------------------------------------------
The bin inside "Team_Popplio_Script" contains dlls required by the compiler!!!
It should not be missing!


However, there may be a chance that the Team_Popplio_Script.dll is missing
from either of the filepaths:
"Team_Popplio_Script\bin\x64\Debug\net6.0"
"Team_Popplio_Script\bin\x64\Release\net6.0"
"Team_Popplio_Script\bin\x64\Release-Game\net6.0"

To regenerate this dll,
1) Right-click "Team_Popplio_Script" in the Solution Explorer
2) Click "Build"

Once build is finished, the project can be ran as per usual.


