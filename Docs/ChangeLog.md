Shawn — 15/9/2024 5:49 pm
Minor Organisation (WEEK 2)
  - Base documentation + filter organisation
Shawn — 15/9/2024 8:25 pm
Math Library (WEEK 2)
  - Color (not math but whatev)
  - vector (pos)
  - timer
  - more to come
Bryan — 15/9/2024 8:38 pm
ECS Architecture (WEEK 2)
  - Ported over from prototype engine
  - Tested with some code in Main.cpp
Shawn — 16/9/2024 8:43 am
Math Library (WEEK 3)
  - Geometry
  - Trigonometry
Val — 16/9/2024 8:36 pm
Input system (WEEK 3)
  - created InputSystem class

Shawn — 16/9/2024 11:09 pm
Math Library (WEEK 3)
  - matrices (wip)

Shannon Rio — 18/9/2024 1:42 am
 Collisions (WEEK 3)
  - Added header and source file for collisions 
  - implemented Class
  - added simple basic Point-Rectangle Collision function (more to come)
 
Bryan — 18/9/2024 11:00 am
Serialization (WEEK 3)
- Implemented Serializer class using RapidJSON
- Able to read entities data(eg. tags, groups and components data) and create them from .json files
Hafawati — 18/9/2024 2:59 pm
Graphics (WEEK 3)
- Integrated Graphics with ECS
- Can render points, lines, rectangles, circles
- Have a background with texture
 
Xian Hui — 18/9/2024 9:29 pm
 unity prototype (WEEK 3)
- added layers of bg
- added bgm audio 
- added char
- added jump 
 
Xian Hui — 18/9/2024 9:37 pm
 - added main menu 
Shawn — 19/9/2024 8:29 pm
 FMOD (WEEK 3)
- integrated library
- added code to talk with library
Shawn — 20/9/2024 1:12 am
 FMOD (WEEK 3)
- now actually works (sound in game)

Shawn — 20/9/2024 2:22 am
 Libraries (WEEK 3)
- matrices update
- binary collision (port over)
Xian Hui — 22/9/2024 5:53 pm
 unity prototype (WEEK 4)
- added scrolling bg in parallex (can adjust spd for each layer)
- fixed jump
- added colored idle & jump sprite
- added camera controller
-added slide
- added layered music w 2 audio sources
 
Bryan — 22/9/2024 8:10 pm
Input system (WEEK 4)
- Modified it to emit input events
Event system (WEEK 4)
- Only input events as for now
- Uses the publisher-subscriber model
- A test system is created to handle the input events
Core Engine (WEEK 4)
- Implemented Game class that manages the main game loop and systems
- Added delta time calculation as well
Serializer (WEEK 4)
- Updated Serializer to read graphics components
 
Shannon Rio — 23/9/2024 12:44 am
Collision (WEEK 4)
 - Added Rect to Rect Collision
 - Added Circle to Circle Collision 
 (untested atm)
Pearl — 23/9/2024 12:55 pm
Graphics (WEEK 4)
- implemented TRS (currently includes bg & sprites tgt, rather than separately
Shawn — 23/9/2024 4:15 pm
Math (WEEK 4)
  - lerp
  - rand
Shawn — 23/9/2024 6:37 pm
FMOD (WEEK 4)
  - better fleshed out interface
  - connected audiosystem to ecs
Shawn — 23/9/2024 9:52 pm
Audio (WEEK 4)
  - calculated beat intervals and related info
  - got metronome to play
Hafawati — 24/9/2024 4:48 pm
Graphics: Text/Font (WEEK 4)
- Implemented text rendering
Shannon Rio — 24/9/2024 11:15 pm
Physics: Gravity (WEEK 4)
 - added a basic Gravity implementation for now
  (untested, hope to try testing tmr to see if any adjustments needed)
 
Bryan — 25/9/2024 6:15 pm
Serializer (WEEK 4)
- Updated Serializer to read general application information (eg. window width, window height and vSync on/off)
Pearl — 25/9/2024 6:40 pm
Graphics (WEEK 4)
- finished implementing TRS
Pearl — 26/9/2024 7:41 am
Debugging (WEEK 4)
-debugging msg of fps & number of objs
Xian Hui — 26/9/2024 5:32 pm
 
- added hold for jump, crouch jump & slide (w bugs) 
 
Bryan — 28/9/2024 8:05 pm
Debugging Capabilities (WEEK 4)
- Implemented a logger to output debugging messages to console
- 5 levels of messages (Debug, Info, Warning, Error and Critical)
- A log includes date-time stamp, message level and message
- To use the logger, include "Logger.h" in your source/header files
Bryan — 29/9/2024 1:51 am
Entity Cloning (WEEK 4)
- Implemented a clone system that create clones of existing entities
- For now, press 'D' to clone entity with id 1
Shawn — 29/9/2024 9:55 pm
Prototype (WEEK 4)
- Character movement between the 3 lanes
Shawn — 30/9/2024 3:17 am
Prototype (WEEK 5)
- Track: moving according to music
- Judgement: judge according to time of input vs note pos (havent tested)
- Note: note pos, type is tap for now, delete upon missing (no feedback yet)
Shawn — 30/9/2024 11:37 am
Prototype (WEEK 5)
- Note: added feedback
- Judgement: miss
Val — 30/9/2024 3:29 pm
Asset Manager (WEEK 5)
  - Integrated to store Shaders
  - store Textures
  - store audio data
Shawn — 30/9/2024 7:35 pm
Prototype (WEEK 5)
- Judgement: input (errors but prototype so)
- Track: First 5 to 10 secs mapped
Shannon Rio — 1/10/2024 9:35 pm
Collisions & Gravity Test (WEEK 5)
- Collisions (for rect to rect and AABB) & gravity successfully tested and showing promising results
 
Bryan — 2/10/2024 10:21 am
ECS (WEEK 5)
- Added some error handling
Post Build Events (WEEK 5)
- Fixed "freetype.dll" missing by adding a post build event for it
 
Bryan — 2/10/2024 6:26 pm
Event System (WEEK 5)
- It can now handle callbacks outside a system
Pearl — 2/10/2024 7:57 pm
Graphics (Week 5)
- Finished for debug mesh side
- added buttons for T,R & S 
- started on camera but commented out as its still WIP
Shawn — 2/10/2024 8:43 pm
Prototype (WEEK 5)
- WIP on hold note
Audio (WEEK 5)
- Tweaked beat detection
- change to pascal case
Input (WEEK 5)
- Input subscription to event (WIP)
- Input vs Audio lag test
Math (WEEK 5)
- change to pascal case
etc.
Shawn — 3/10/2024 4:04 am
Prototype (WEEK 5)
- Prepared for build (Fixed judgement bugs)
- Added reset button (TAB) and quit button (ESC)
Hafawati — 3/10/2024 7:05 am
Graphics(WEEK 5)
- Implemented instance rendering of 2500 tri obj
- Fixed merging stuff on graphics system
- Changed to input system for some key presses on graphics system
Pearl — 11/10/2024 12:11 pm
Graphics (WEEK 6)
- Implemented some more Matrix functions into Matrices.cpp & .h ((to change TRS from glm to matrices))
- Able to do TRS now (NOT YET CHANGED INTO MATRICES & DEBUG TRS NOW CMI)
Bryan — 11/10/2024 8:17 pm
Project Settings (WEEK 6)
- Changed from C++14 to C++17 to access std::optional
- Updated document.h (RapidJSON main header) to work with C++17
- Pushed to working
- Apologies for not realising that the default is C++14
 
Shawn — 13/10/2024 6:08 pm
imgui (WEEK 6)
- added library
- added wiki & docs for ref
Shawn — 13/10/2024 6:40 pm
imgui (WEEK 6)
- rendered demo gui
Bryan — 14/10/2024 1:56 am
 Prefabs (WEEK 7)
- Added Prefabs Manager
- Tested with code in Test.cpp
 File Organization (WEEK 7)
- Reorganized Systems, Components and Events Folder into Graphic, Physics, Input and etc
 Logging (WEEK 7)
- Redirected all log messages to ImGui window
Shawn — 14/10/2024 8:24 pm
Logic System + File Reorg (WEEK 7)
- Base logic system for scripting (Untested due to inability to get entity from component currently)
- Reorganised files (libraries -> math + utilities)
Bryan — 14/10/2024 9:04 pm
 Serialization/Deserialization (WEEK 7)
- Refactored component deserializing to use a function template
- For future components, add the component deserialization logic into DeserializeComponent template in Serialization.h
 
Pearl — 16/10/2024 5:46 pm
Graphics (WEEK 7)
- Camera zoom in & out done (button Z & X)
- Transformations in matrix done
Hafawati — 18/10/2024 2:13 pm
Graphics (WEEK 7)
- Implemented animation using sprite sheet
Bryan — 20/10/2024 7:57 pm
Refactoring (WEEK 7)
- Refactored the rendering system to integrate more closely with ECS
- Refactored the clone system to be more scalable for future components
- Refactored the asset store to not use raw pointer (except for FMOD::Sounds)
- Refactored the collision system to integrate more closely with ECS
Project settings (WEEK 7)
- Changed the output and intermediate directories
- Updated post build events to copy Assets folder to output directory
- Changed the project working directory to output
- Something like what we did for GAM100 and GAM150
Transformations (WEEK 7)
- Separated transformations from render system to another system
Shannon Rio — 20/10/2024 8:18 pm
Physics (Week 7)
- Added forces to physics
- Tested (Kind of?) and shows promising results
- Gravity can still work with addition of physics forces
Val — 23/10/2024 2:31 pm
Performance Debugger (WEEK 7)
  - implemented performance viewer with functions start() & end() which stores the code execution time in ms
AssetStore (WEEK 7)  
  - removed Meshes from asset store and returned it to graphics system
 
Bryan — 24/10/2024 7:44 pm
Logger (WEEK 8)
- Updated Logger to support logging to both Windows console and ImGui.
- Logger methods now accept a LogDestination parameter.
Shannon Rio — 26/10/2024 11:06 pm
Physics (Week 8)
- Added drag force to ensure velocity stops to a constant when gravity is applied (note: only gravity has drag force)
 
Hafawati — 29/10/2024 7:58 pm
Animation (Week 9)
- Implemented Animation System for managing entity animations.
- Added functions to pause, resume, loop, play once.
- Press Q,W,E to toggle between different animations
- Press 1 to pause animations
- Press 2 to resume animations
 
Bryan — 30/10/2024 8:50 pm
 Prefabs (WEEK 9)
- Serialization and Deserialization
- Components editing
- Loading of prefabs in editor
- Saving of prefabs in editor
- Deletion of prefabs and its files
- Reflection of changes to a prefab's components data to all of its instances
Val — 30/10/2024 9:39 pm
Performance Viewer (WEEK 9)
- Calculated time for engine
- ImGui window that displays Performance graph for the last 100 polls
 
Shawn — 1/11/2024 11:04 pm
Prototype (WEEK 8)
- Updated assets
- Added char animation
- Mapped track
- Added hold notes
- Added track mute system
- wip scoring
 
Shawn — 2/11/2024 11:18 pm
Math (WEEK 9)
- Finalised Vector2D & Matrix2D
- Not tested
Compile Optimization (WEEK 9)
- Multi Processor Compile
- Pre-Compiled Header for MathLib.h
- Linker (Incremental Build)
 
Shawn — 3/11/2024 10:17 pm
Components (WEEK 9)
- PCH
- Logic, Active, Audio Components (WIP)
Game Viewport (WEEK 9)
- Play, Stop, Pause (WIP)
- Logic System (Scripts) (WIP)
Shawn — 3/11/2024 11:00 pm
Game State (WEEK 9)
- Stop now resets scene (kill all entities + re-deserialize)
Bryan — 4/11/2024 2:24 pm
Input system (WEEK 9)
- Refactored completely to resemble c-processing and alpha engine input systems
- No longer emit events
- Not including this in my IMR as it belongs to val
Compile time optimization (WEEK 9)
- Integrated shawn's pre-compiled math headers with external and std lib headers into Precompiled/pch.h
Creating/Loading/Saving of multiple scenes (WEEK 10)
- Added creating, loading and saving of multiple scenes in editor
- Click on "File" in main menu bar and select the scene action you want to perform
Shawn — 4/11/2024 8:02 pm
Logic System (WEEK 10)
- Scripts now work (WIP, assetstore has to be further developed to read files in folders)
- Scripts loaded by asset store, and runs within game loop
Matrix2D (WEEK 10)
- Data now encapsulated in union
Val — 4/11/2024 9:46 pm
Performance Viewer (WEEK 10)
  - Specified the time data types
  - Converted graph reading of performance from graph to table format
Bryan — 4/11/2024 11:14 pm
Editor Camera (WEEK 10)
- Moved from render system to Editor
- Changed controls from ImGui window to keyboard/mouse
- WASD or hold right mouse button to move
- Scroll to zoom in/out
Shawn — 5/11/2024 1:33 am
Logic (WEEK 10)
- Added to json manually to test
Math (WEEK 10)
- Added Polynomials (for future features e.g. FFT)
Audio (WEEK 10)
- Able to play / pause / stop audio in component GUI
- Added to json manually to test
 
Shawn — 5/11/2024 6:04 am
Logic (WEEK 10)
- Integrated events into ILogicScript (Input events)
Audio (WEEK 10)
- Using DSP (FFT) - WIP
Hafawati — 5/11/2024 9:08 pm
Asset Pipeline (Week 10)
- Updated asset pipeline to load assets without needing to code in c++
- Asset paths are to be added in the ResourcePath.json
- As of now, assets added in components should follow the naming convention of just the file name 
Bryan — 7/11/2024 4:13 pm
Precompiled Headers (WEEK 10)
- Enabled again
- Fixed precompile headers error encountered when hafa was merging
Bryan — 8/11/2024 1:20 pm
Level Editor (WEEK 10)
- Added toggling on/off
- Press 'F11' to toggle
- When level editor is switched off, scene will start automatically
- When level editor is switched on, scene will end automatically
Shawn — 13/11/2024 11:54 am
Level Editor (WEEK 10)
- Converted toggle to toggle for editor run and fullscreen mode
Prototype (WEEK 10)
- Results screen
- UI (Combo, Track, etc.)
- Level Map
Editor (WEEK 10)
- Reset serialized entities upon stopping editor
- Fixed instanced entities and performance test issues when serializing
Collision Events (WEEK 10)
- Collision Events emitted
- Integrated event callback for scripts for Collisions (Currently can only acll back for EVERY collision)
Misc (WEEK 10)
- Fixed pch related errors on merge
Bryan — 13/11/2024 10:37 pm
Entity Selection (WEEK 11)
- Finished entity selection and dragging with cursor in viewport
- Can now press del key to delete selected entity)
- Highlighting selected entity with rgb box
File Dialog (WEEK 11)
- Refactored prefabs and scenes loading/saving to use windows file dialog instead
 
Shawn — 18/11/2024 7:40 pm
C# Scripting (WEEK 11)
- Base API done
Val — 18/11/2024 8:00 pm
Performance Viewer (WEEK 11)
  - Integrated ImPlot into the project
Pearl — 18/11/2024 8:05 pm
 ImGuizmo (WEEK 11)
  - Wrote code for ImGuizmo but still trying to render it out
Bryan — 18/11/2024 8:06 pm
Release-Game Build Configuration (WEEK 11)
- Completely separated ImGui Code (all of Editor class) from this Build using IMGUI_DISABLE macro
- Disabled windows console by setting the linker SubSystem to WINDOWS
- Exe will no longer generate imgui.ini
In-Game Camera and Camera Manager (WEEK 11)
- When separating ImGui code for the release-game build, I realised that the in-game camera seems to be missing
- Implemented a Camera Manager that owns the in-game and editor cameras
Selected Entity bounding box (WEEk 11)
- Refactored the drawing of the bounding box to use ImDrawList instead of RenderSystem
To-do by WEEK 13:
- Implement a scene viewport which will display the in-game camera frustum and grid view (similar to unity's scene view)
 
Hafawati — 18/11/2024 8:24 pm
Level Editor (WEEK 11)
- Font size can be adjusted 
- Animation can be edited and deleted. 
- New animation can be added to an existing entity and new entity
 
Shannon Rio — 19/11/2024 11:50 am
 Level Editor (WEEK 12)
- When player adds a Box Collider component to an entity, it takes the value of the scale of the entity
- this is still in a custom function 
 
Pearl — 22/11/2024 9:31 am
ImGuizmo (WEEK 12)
  - Fixed implementation of ImGuizmo, added in a Imgui window to change between transformations & added a bool ImGuizmoActive to switch between ImGuizmo stuff & mouse moving entity in viewport
Bryan — 24/11/2024 9:07 pm
Text/Font system (WEEK 12)
- Successfully integrated transform component with text component: changing position in transform component will now move the text, but changing rotation and scale doesn't affect the text as it will clash with the font sizes
- Removed position from text component as it's no longer necessary
- Text entities are now selectable in editor
- As for now, it only renders in-world text, will add on-screen text after M3
- It can now support variable font sizes of the same font type
- Font size can be changed through the font size property of text component
Background (WEEK 12)
- Background entities will now move when the editor camera moves
- Are now scalable
Prefabs (WEEK 12)
- Fixed changes to RigidbodyComponent mass and useGravity not reflecting to instances
- Changes to prefabs data(except for transform component) will now reflect to instances across multiple scenes
Shawn — 24/11/2024 10:00 pm
Mono C# (WEEK12)
- Switched to mono
- Able to invoke void and return value methods with or without params
- Able to get and set variables and properties
- All is one way currently (can only call from C++, since the tutorial didn't say how to do the other way)
- No hot-reload (tutorial dont have)
- Has mem leaks (tutorial didnt say how to exit and mono documentation on this is very little)
- TODO research more on calling from c#, hot-reload and exit
 
Val — 25/11/2024 12:53 am
Asset Loader (WEEK 12)
  - Assets are now read from directory
  - Gave Audio files meta files to store data
Shawn — 25/11/2024 5:04 pm
Mono C# (WEEK 12)
- C# scripts now in game loop (but not attached to any entities currently)
- Tried to remove mem leaks but can't
Shawn — 25/11/2024 8:05 pm
Mono C# (WEEK 12)
- C# Scripts can be attached to entities (but the ids they hold are wrong + cant ref the entities yet because requires CppSharp / SWIG to bridge between C++ and C#)
- I forgor, but how u can use the API can be seen in "tests/TestMono.cpp"
Shawn — 26/11/2024 3:41 am
Mono C# (WEEK 13)
- Now can call C++ code from C# scripts using CppSharp + mono
Use the Team_Popplio_CppSharp project to build generator for the binders for the c# script and c++ code to bridge
To do so, right click on the project in VS and click build
After building, run "dotnet run" in the project directory after building to generate the binders
Copy the generated .cpp files from "Generated" folder into Team_Popplio_EngineDLL
Copy the generated .cs files from "Generated" into Team_Popplio_Script/cppsharp
Build the solution
Ref: https://github.com/mono/CppSharp/blob/main/docs/GeneratingBindings.md
NOTE: You only have to do the above if you want to add more c++ code to be read by C# scripts OR
The C++ Header used for the binder has changed since the last time
Otherwise, you can now call C++ codes using "using Team_Popplio"
- Currently wip (crashes on play if using Test.cs)
- working on interface
 
Pearl — 26/11/2024 9:51 pm
ImGuizmo (WEEK 13)
  - Fixed rotation for ImGuizmo & commented out imgui's window to change T R S & removed bool ImGuizmoActive
Shawn — 29/11/2024 5:18 am
Mono C# (WEEK 13)
- Deprecated CppSharp and EngineDLL projects (Keep in solution in case need in the future)
- Restructured solution AGAIN (Now has a simple exe, with a main engine DLL and the C# project DLL
The above is necessary to faciliate 2 way comms (at least with how I implemented it)
- Implemented Internal Calls to call C++ code from C#
- Confirmed test for 2 way communication between the languages
- Currently barebones for the Native API (Extremely unscalable due to Mono and how I implemented it)
Logic (WEEK 13)
- Fixed audio not pausing when pausing runtime
- Refactored logic loop
GUI (WEEK 13)
- Added gui for log filters (now can toggle log filters)
- Currently unsaveable
 
Shannon Rio — 29/11/2024 9:36 pm
Prototype (WEEK 13)
- Player can ‘jump’ to different positions when pressing the keys “R, V and F”
- Audio plays in the background (track.wav)
- Backgrounds can move 
Bryan — 30/11/2024 7:29 pm
Scene and Game Viewports (WEEK 13)
- Scaling the viewports will now maintain the aspect ratios
Inspector and Prefab Editor (WEEk 13)
- Components (except for Active and Transform) can now be removed from entity/prefab
 
Val — 30/11/2024 7:31 pm
ASSET STORE (WEEK 13)
  - Asset store can now select a .wav from somewhere and save it into assets folder
  - can do the same for .png
  - Can reload assets
  - Total wipe button to reset all meta files to default settings
  - made browsing assetstore look nicer
Hafawati — 30/11/2024 10:50 pm
Fullscreen & Interruption Handler (WEEK 13)
- Toggle fullscreen mode using F1
-  alt+tab, ctlr+alt+tab can minimise and pause game & audio on fullscreen and windowed mode
- Inputs are not confused and work properly when returned to the game
 
Shawn — 6/12/2024 3:19 pm
Logic System C# (WEEK 13)
- C# code can call logger
- Hot reload WIP
- Fixed mem leaks
Some reorg
Game loop fixes
ImGui - Added collider component options
Documentation
FFT visualizer for audio
fixed some warnings
Undo (WEEK 14)
WIP
Shawn — 6/12/2024 5:41 pm
Undo (Holidays)
- Change detected achieved for gizmo transform
Shawn — 7/12/2024 2:49 am
Undo / Redo (HOLIDAYS)
- undo achieved for gizmo transform (ctrl z)
- redo achieved for gizmo transform (ctrl shift z)
Shawn — 7/12/2024 3:23 am
GUI (HOLIDAYS)
- Top bar now shows current scene open
Shawn — 7/12/2024 4:05 am
GUI (HOLIDAYS)
- Fixed moving hierarchy list when window moves (commented out code in case)
Undo / Redo (HOLIDAYS)
- undo / redo achieved for edit component transform
Shawn — 7/12/2024 5:05 am
ECS (HOLIDAYS)
- Now all components' systems detect if the entity is active before processing (ActiveComponent)
Shawn — 7/12/2024 9:44 pm
UNDO / REDO (HOLIDAYS)
- All components, and entity changes detected (except entity info)
- Can undo / redo component modifications, addition and removal
- Can undo / redo entity addition and removal
EDITOR (HOLIDAYS)
- Saved to temp file instead when running to prevent unintended save
MISC (HOLIDAYS)
Fixed release-game
Shawn — 7/12/2024 10:08 pm
EDITOR (HOLIDAYS)
- Added undo / redo buttons to top bar
- Removed console from release editor
Shawn — 8/12/2024 9:03 pm
PARTICLE SYSTEM (HOLIDAYS)
- WIP (Base level implementation)
COLOR (HOLIDAYS)
- Fleshed out color class to be used by particle system
- Converters, operators, etc.
Bryan — 9/12/2024 1:12 am
INSTALLER (HOLIDAYS)
- Fixed desktop shortcut not working (missing a '\' somewhere in installer script)
 
Shawn — 9/12/2024 3:53 pm
MISCELLANEOUS (HOLIDAYS)
- Added icons to engine and game
- Added script directory window
- Added toggles on top bar to toggle windows
Bryan — 11/12/2024 12:25 am
Drag and Drop Assets (HOLIDAYS)
- Added drag/drop textures(.png) and audio(.wav) files functionality into editor
- Any other types of files are unsupported
- To do: Display pop-up error message if user tries to drag/drop unsupported file types
Directory Browser (HOLIDAYS)
- Replaced selectables with icon buttons (using Font Awesome for most files and dirs)
- For textures, they are replaced with image buttons
- To fix: Buttons and text alignment
- Deprecated old AssetStoreGUI.cpp in case something went wrong with new one
Misc (HOLIDAYS)
- Refactored Vector2D libs to public data members for efficiency
 
Shawn — 13/12/2024 12:34 am
Mono Hot Reload (HOLIDAYS)
Hot Reload (EXPLICIT) added
added log filter for engine / user logs
switched .NET framework as previous one couldnt even support creating new C# classes in mono
Hot Reload:
Can rebuild only with engine exe (not while using Engine.sln), otherwise use explicit reload with F8 or the menu button or by running the editor
Use the Scripts.sln to rebuild (dont use the green button)
This will automatically put a new dll in the working dir
You can then explicitly reload it
Currently I could not find a way to automatic reload
This will do for now
Shawn — 14/12/2024 8:02 am
Components (HOLIDAYS)
Added isActive to all components for use by scripts
Audio component is more fleshed out + actually uses instances now instead of asset audio instance
Mono (HOLIDAYS)
Resolved issues regarding ECS in C# (Kill now works in C#)
Logic (HOLIDAYS)
Resolved issues regarding deleted entities
Misc (HOLIDAYS)
Fixed some stuff
Shawn — 14/12/2024 8:10 am
Editor (HOLIDAYS)
Save / Save as shortcuts added (Ctrl + S = Save | Ctrl + Shift + S = Save As)
Logic (HOLIDAYS)
Scripts now dont crash the engine when they error out (Error handling)

Bryan — 16/12/2024 2:38 am
 Asset Browser (HOLIDAYS)
- No longer displays meta files
Layer Matrix (HOLIDAYS)
- Can be used to toggle collision checks between entities of different layers
- Layer interactions(collision) data is now serialized/deserialized
Shawn — 19/12/2024 9:23 pm
(HOLIDAYS)
Documentation
- Header file docs for new files
- Added doxygen file for doc generation
- Added Change logs
- Added README
Error Handling
- Better error handling for C# scripts
Change
- Auto save feature
- Unsaved change detection
Misc
- C# code organisation
- Reorganised script load
- Added default assets (scene, image)
- Minor fixes (warnings, release game, etc)
Events
- Collision events now can be used for C#
- Have to check on script side if entity is involved if it is a C++ script
Logic
- Changed to use script instances per entity
- Now differentiates between different script languages
Audio
- Changed to use audio instances
Configuration
- Added safe mode
- Added start scene build
- Added last scene opened
Serialization
- Now does not outright invalidate whole entities for missing members, so that when engine changes components it doesn't ruin the whole scene, instead updates the missing members to default values, which user can then save fixed serialization nuking old scenes that have old component data
C#
- Added missing assemblies
- Added entity objects in C# synced to its C++ counterpart
C# Scripting
- Internal calls mostly done
- components can now be called via C#:
- Active, Transform, Render, RigidBody, BoxCollider, Text, Logic, Animation
- Added color and Vector2 interfaces
- Inter-C#-Script interactions are possible now
- Test scripts can be referred to under the "Test" folder
 
Shawn — 21/12/2024 2:43 am
(HOLIDAYS)
C# Scripting
- Audio Internal calls done
- Added some Audio Library Internal calls
- Destroy entity Internal calls done
- Instantiate entity Internal call WIP (Requires Instantiate entity interface, now only has prefab one)
Fixes
- Fixed crash when selected entity is destroyed by script in editor during runtime
- Fixed crash when adding audio to entity in editor
Bryan — 22/12/2024 2:02 am
Hierarchy/Parent-Child relationship (HOLIDAYS)
- Can be seen in Hierarchy view
- Only works for Active and Transform components
- To use it, add the parent component to an entity and add children using that component
accordingly
- Currently not serialized yet (will do it next day)
Shawn — 22/12/2024 4:39 am
(HOLIDAYS)
C#
- Implemented input library to internal calls for C#
- Implemented instantiation and deletion (causes crashes currently due to how instantiation is done, and deletion might not be done correctly either)
- Added tests for input and instantiation and deletion
Scene
- Added empty scene code files for future scene switching at runtime
Fixes
- Fixed crash when running editor second time due to subscription for previous cpp script instantiations in the eariler run
 
Bryan — 23/12/2024 3:28 am
Hierarchy/Parent-Child relationship (HOLIDAYS)
- Finished adding serialization/deserialzation of parent component
- A child entity can only have one parent
- A parent entity can have multiple children
- Deleting a parent entity will also delete all its children entities
- To fix: Rotating a parent entity will make the children entities rotate 
about their own positions instead of about the parent entity's position (done)
 
Bryan — 23/12/2024 6:34 pm
Hierarchy/Parent-Child relationship (HOLIDAYS)
- Rotating a parent entity will now preserve the relation position and orientation of
the children entities
Shawn — 24/12/2024 3:19 am
(HOLIDAYS)
Inspector
- logic component wip for C# scripts (currently achieved read of C# script variable in inspector)
Hierarchy
- Fixed child not updating to -1 value for parent when parent removes child
- Fixed undo/redo of parent child component actions (was not added + required some more complex code due to nature of action)
 
Shawn — 24/12/2024 3:30 am
(HOLIDAYS)
Miscellaneous
- Added shortcuts (W, E, R, T) for gizmos
Shawn — 24/12/2024 9:57 am
(HOLIDAYS)
Hierarchy / Undo/Redo
- Fixed unable to undo/redo remove parent
- Fixed unable to undo/redo remove child
Bryan — 25/12/2024 3:40 am
Logging of crashes (HOLIDAYS)
- Added logging of crashes to text files
- Details include Exception Code, Exception Address, Exception Parameters and Stack Trace
- Parameter[0] indicates the type of violation (0 for read, 1 for write)
- Parameter[1] contains the memory address that was accessed
Shawn — 29/12/2024 6:38 am
(HOLIDAYS)
C#
- Inspector displays script variables, can edit variables to be used at start
- Script variable serialization
- Script variable initialization (init component values add start of runtime according to component values)
Misc
- Fixed crash on parent remove
- Entity change detection (add/remove) for C# dictionary to use at runtime
- Undo/Redo safety (only can undo/redo while in editing state)
(NEXT)
- Port M2 prototype over
 
Shawn — 29/12/2024 5:38 pm
(HOLIDAYS)
- Added art assets
- Fixed assetloader not being able to load intermediate files in non-terminating dirs
Shawn — 3/1/2025 4:07 am
(HOLIDAYS)
Prototype
- added refs from unity
- added art assets
Scene
- Runtime scene change works somewhat via C# (still got some bugs)
ECS / Logic
- Added OnDestroy / OnEnable / OnDisable events for C# scripts
QueueSystem
- Added queue utility for runtime and editor queues
- Used to update things after the main loop to avoid crashes via things like iterator invalidation
Fixes
- Fixed crash on parent destroy (via C#)
- Fixed crash on C++ script entity destroy (via C#)
Audio
- GetSpectrum works (requires more testing)
Hierarchy
- Internal calls for C# works (still got bugs)
Mono
- Array conversions (Need to better differentiate between jagged, true rectangular, and generic containers in the future)
Script
- Boolean array in inspector achieved
Performance Viewer
- Fixed display not showing much due to milliseconds (changed to microseconds)
- Added refresh limit to better view data + to optimize
- Removes non-updating subsystems after a timeout duration
 
Shawn — 8/1/2025 12:16 am
(HOLIDAYS)
Fixes
- Fixed display for performance viewer not showing properly due to casting
- Fixed recurring entity creation via inspector
- Changed edit anim speed to FPS display instead
- Fixed mono reload failing on load scene
- Fixed crash on run due to C# script null entity
Performance Viewer
- Added refresh rate limit + changed milli to micro
- Removed subsystem from list after timeout time
Inspector
- Container vars from C# script now can be edited (currently only jagged arrays)
Serialization
- Container vars from C# script now serialized
Game
- Porting WIP
- Added audio files
Events
- Added OnTrigger (enter/stay/exit) + OnCollision (enter/stay/exit)

Hafawati
 — 11/1/2025 9:42 pm
WEEK 1
Fixes
- Fixed the upside down and misaligned text
Serialization/Graphics
- Added alpha control
 
Bryan
 — 11/1/2025 9:50 pm
Hierarchy system (WEEK 1)
- Added local transforms to parent component
- local position (Position relative to parent)
- local scale (Multiplies up through the hierarchy. If a parent is scaled 2x and the child is scaled 3x, the final scale is 6x)
- local rotation (If an object is already rotated 45°, rotating it 90° locally will rotate relative to its current orientation)
 
Shawn
 — 11/1/2025 9:58 pm
WEEK 1
Undo / Redo
- Entity modify change detection
- Entity modify undo / redo
Fixes
- Fixed path changing when load scene
C#
- Small audio component additions
- Time library
- Copy components via script
Game
- Port wip
Hafawati
 — 12/1/2025 2:10 am
WEEK 1
Text
- Added alpha
Animation
- Added proper stop function
Fixes(?)
- Pause animation when pause button is pressed
Bryan
 — 16/1/2025 11:44 pm
In-Game Camera (WEEK 2)
- Added window to move and abjust the zoom of in-game camera
- Added rendering of in-game camera view frustum
Shawn
 — 19/1/2025 1:20 am
WEEK 1 + WEEK 2
Components
- Persist on load functionality (untested)
- Persist on load for children (untested)
C#
- AddComponent interface
- Persist on load interface (untested)
- IgnoreInspector attribute
- public vars in gui inspector
- prefab interface
- GetAnimations
- Global usings
- GetComponent, HasComponent, GetScript, HasScript functions -> changed T to their respective types
- Json interface
- Setting absolute / local transform fix
Misc
- OnAppQuit event (unimplemented)
Fixes
- C# entity property null
- Child local transform not preserving
- absolute / local transform bug
- NativeID not setting correctly for invalid entity (-1)
- Player movement
- C# elapsed time
Game Port (WIP)
- Scripts ported
- Level deserialization wip
- Level background
 
Bryan
 — 19/1/2025 1:34 am
In-Game Camera (WEEK 2)
- Added serialization/deserialization of camera settings per scene
Shawn
 — 19/1/2025 2:20 am
WEEK 2
Added temporary fix for ontrigger inputs in C#
Rio kairevious.
 — 22/1/2025 7:31 pm• ≽ܫ≼
WEEK 2 + 3
- Forces Manager Added (WIP)
- Integrating into current Physics System
currently
 
Bryan
 — 22/1/2025 10:20 pm
Clone System (WEEK 3)
- Updated to account for hierarchical relationships
- Every child entity will have a different id
Nested Prefabs (WIP)(WEEK 3)
- Following the way unity does it:
    - Parent and children prefabs will be saved as separate files
    - Parent prefab will contain references to children prefabs
    - Instantiation of parent prefab will create the hierarchical relationship automatically
Shawn
 — 25/1/2025 10:45 pm
WEEK 2
Game
- Port
- background
Misc
- Tweaked animation sheet dimensions
- Active icon in hierarchy list
- Clear on run for console log
- Changed hierarchy update order
Transform / Hierarchy
- absolute vs local transform
- Reflect local transform on absolute transform
Fixes
- Player movement
- elapsed time
- Setting world to local not taking into account scale, like local to world
Serialization
- Font size
WEEK 3
Cloning
- Clone nested entities
- clone ties child entities referenced in script for an entity within the tree
- clone entity info
Fixes
- Clone not preserving highest order original entity's parent
- Json reader
- runtime instantiate not setting up entity properly
- deserialization after deleting entities (fixed by setting entity ids accordingly if there are deleted entities in between in the serialized file)
- ECS free id not correctly done
- Public variables not showing in inspector
- (Game) miss not triggering at the correct time (bug due to using absolute instead of local transform)
- Logic system not being able to update scripts that turned active at runtime
- (Game) beat metronome + speed
- (Game) corrupted scene
- Errors on deserialization with free ids
- Free ids not getting removed correctly when set to
- Clone not preserving parent & local transforms
- Runtime instantiation not instantiating scripts attached properly
- Crash due to GC collection after destroying notes in game
- Delta time in C#
- release-game build
Misc
- Header attribute for inspector in C# (attempted)
- Added corruption detection for scenes due to duplicate ids
- Added time check for asset load and scene load
Game
- Note feedback
- Player animations
- Port finished-ish
C#
- Json reader
Game loop
- Finished fixed update
Temporary Fixes
- Note feedback and related scripts are called explictly instead of on a loop due to not being able to be added to the loop
Shawn
 — 26/1/2025 7:20 pm
WEEK 3
Game
- Fixed timing for track to sync with audio
- Tweaked local transforms
- Fixed fever code (rendering for 'square' and 'missingtexture' textures still bugged)
- Added moving border UI
- Audio fade in
Mono
- Fixed script deletion randomly within game (?)
Audio
- GetSpectrum Logarithmic
Shawn
 — 26/1/2025 10:50 pm
WEEK 3
Audio
- Reorg audio folder
- Reorg scene folder
- Added Prof Gavin's sfx
Game
- Added good / bad sfx
Settings
- Audio volume
Fixes
- Most warnings
Shawn
 — 27/1/2025 6:40 am
WEEK 3
Hierarchy / ECS / Logic
- Clone preserve child entity ref in logic component (w/o entity lists for now)
Game
- Trail on player
- Hold Note
- Target UI feedback
- Fixed hold judge
- Settings
- Bar markers
Fixes
- Default textures (?) (Bugs out after a certain while for some reason)
- Local vs Absolute transform
Misc
- Art assets
Shawn
 — 31/1/2025 8:52 am
WEEK 4
Game
- Hold vfx
- Judgement vfx (animated)
- Audio spectrum vfx
- General scene updates
- BG moving entities
Fixes
- Pause then stop runtime issue
- Note spawn drift
- C# runtime issues (instantiation, etc.), still crashes in debug mode
- Cloning doesnt clone logic component vars properly
Misc
- Art assets added
C#
- OnCreate
Level
- Added sprite id to note class in chart editor (doesn't do anything now)
Animation
- Added dimension calculation for sheet
- Now can animate multiple rows
Bryan
 — 31/1/2025 1:10 pm
Nested Prefabs (WEEK 4)
- Finished serialization and deserialization of nested prefabs
- Finished instantiation of nested prefabs
- Update prefab list to display hierarchical relationships
Clone system (WEEK 4)
- Added additional validations
- Still trying to fix hierarchical cloning crash on debug mode
Merged to working
Shawn
 — 3/2/2025 12:32 am
WEEK 4
Game
- Speed tweaks for bg moving entities
- Level parsing tweaks
- Note spawn overhaul (switched to cycling instead of instantiate-destroy) (still ahve some visual bugs)
Misc
- Config files copied to working dir in project configuration
- Unsaved change status shown
- Added issues.md file
Mono
- Added entities objects in C# to managed GC
Fixes
- release-game (still have some instability issues (sometimes script / i dunno what not correct, undefined behaviour))
- Crash | crashes when a variable is initialized with new twice after garbage collection on stopping runtime
 
Bryan
 — 3/2/2025 9:49 am
Fixes (WEEK 4)
- Fixed crash when playing/stopping the scene on debug mode
- Replaced std::set with std::vector for children in Parent Component
Shawn
 — 3/2/2025 10:14 am
WEEK 5
Game
- UI additions
- Changed border
- Added player skid and jump VFX
- Added goal bar
- Cloud movement
- (Fix) recover notes to pool after now
- (Fix) metronome doesn't play after end now
- Level parse tweaks
- Judgement tweaks
- (Fix) judgement feedback fixed
- Added spotlights
Misc
- Added Art Assets
Systems
- Tweaked collision system (optimization)
- Finished OnTriggerExit / OnCollisionExit
Fixes
- Fixed crash on stopping scene due to kill entity
- Fixed scene load at runtime
- Fixed persistent entity (currently does not support children though)
 
Shawn
 — 6/2/2025 11:05 pm
WEEK 5
Game
- (Feedback) Judgement particles
- Results screen
- static score
- (Fix) note drift
- FPS counter
- Pause (animations still running)
- Load scene
- Various tweaks (audio spectrum, positions, vfx, etc)
- Connected scenes together
- (Fix) miss counted twice
- (Fix) player fall off temporary fix
- Added stationary speedboat
- (Fix) finicky pause button
Persistent
- Attempted to fix (not using for now)
C#
- Camera interface
- Quit interface
Timer
- timer for SystemFixedDelta to seperate from FixedDelta
Fixes
- Clone crash due to no check for parent component in handle logic copy
- Some load issues (logic system, etc)
- Load scene issues
- release-game build
- Audio instance removal
- Replay game scene breaking
Misc
- Art asset rename
- Updated dates on file level comments
- Cleared old audio
Shawn
 — 25/2/2025 2:45 pm
WEEK 6 + 7
Systems
- Particle system
- Particle system interface
- Particle system serialization
- Attempted to render particles
Game
- Fever (WIP)
- Rainbow (WIP)
- Directional notes
Misc
- Assets
- Multi note test
- Folder org
Fixes
- Game not switching to result screen after second play onwards
- Logic update order
- Background layering + BG speed
- Note sync issues
 
Hafawati
 — 27/2/2025 12:00 am
WEEK 7
- Rendered particles in instanced and non instanced rendering (texture and animation) 
- Added alpha and animation to instanced rendering
 
Bryan
 — 28/2/2025 1:33 am
Minor optimization of asset loading (WEEK 8)
- DeserializeMetaFile()
        - Reduced I/O, memory allocations and redundant operations
- ListFilesRecursively()
        - Used std::unordered_set for fast meta file lookup
- ReloadAssets()
    - Eliminated redundant string() calls
    - Replaced expensive fs::exists() calls with std::unordered_set 
    - Used structured bindings to keep the code clean and 
- Multithreading is killing me :(
Bryan
 — 4/3/2025 10:40 am
Entity List (WEEK 9)
- Hierarchical relationships can now be created by drag and drop entities into target parent entity
- Entities can now be reordered by (Does not work with root entities yet)
    1. Drag the entity first
    2. Hold shift
    3. Blue line will indicate where entity will end up
    4. Drop it (need to be precise)
- To do:
    - Make reordering work for root entities
    - Drag/Drop children entities into main entity list to make them root again
    - Edit Component: Add searching for textures
Bryan
 — 5/3/2025 6:16 pm
Fixes (WEEK 9)
- Fixed instantiation of nested prefabs referencing same entities as original
- IsKeyTriggered and IsMouseButtonTriggered
 
Bryan
 — 7/3/2025 1:38 am
Editor QOL (WEEK 9)
- RenderComponent editor: Added search bar at the top of textures drop down list
- Entities Hierarchy: Added reordering of root entities
Shawn
 — 10/3/2025 7:29 pm
WEEK 8 + 9
Particles
- Deserialization
- Interface
- C# (WIP)
VFX
- Hold first hit vfx
- Fever animation
Audio
- Hold first hit sfx
UI
- darken
- directional UI
- hold note shorten when holding
Misc
- versioning
- Art assets update
WEEK 10
- C# serialization json interface
- Settings / Game / Debug Json Files
Val
 — 11/3/2025 6:51 am• they/them
Chart editor (WEEK 9 + 10)
  - updated wrong timing calcs
  - added serialization for rainbow notes
  - added directional notes 
  (see editor window on how to use)
  - remapped lvl 1 with rainbow notes
Bryan
 — 11/3/2025 2:33 pm
Editor QOL (WEEK 10)
- Entities can now be made root again by drag/drop into box at the top of entity list
Xian Hui
 — 12/3/2025 4:44 pm
Game engine prototype (Week 10)
- Updated mainMenu
- Updated optionsMenu (need Shannon’s autosave)
Shawn
 — 14/3/2025 2:52 pm
WEEK 10
System
- Gamesystem script to all scenes
Settings
- Calibration
- Keybinds
- debug hotkeys
- Options menu tweak
Gameplay
- Rainbow note vfx
- Rainbow / Fever scoring
- Calculated results
- Level 3 (WIP)
Aesthetics
- audio spectrum in fever
- full combo display
- background for track 2 and 3
- Note sprites according to type
- Menu UI
- Results UI
Engine
- Fixed text color not saving
Misc
- Assets
- Warnings fix
Bryan
 — 21/3/2025 6:14 pm
Loading Screen (WEEK 11)
- Added loading screen so that there is no annoying white screen when launching the engine/game
- Only consists of a loading bar as for now
Bryan
 — 31/3/2025 11:04 am
Loading screen (WEEK 13)
- Added digipen logo splash screen with fade in/out
- Added team logo splash screen
- Need FMOD logo splash screen?
Fixes (WEEK 13)
- Fixed changes to prefabs with UIComponent not syncing with instances
 
Shawn
 — 1/4/2025 4:24 am
WEEK 11
Gameplay
- Fixed directional note VFX
- Reposition directional note UI
- Offset tweak
- Visual calibration
- Change script field to component field WIP (on hold)
- Calibration menu
- Fixed beatlengthtime
- Tweak judgement hold
Engine
- Clone entity field ref to entities within root cloned
- Scene load cooldown
WEEK 12
Gameplay
- Fix Track1 bugged entities
- Fix confirm button in calibration
- Adjust visual note in calibration
- Fix Options menu back button loop
- Attempt to fix end level replay bug
- Fix custom speed not loading
- Custom per chart speed
- Fix notes appear suddenly
- Fix pre-track metronome not lining up with audible audio
Mono
- GC Improvements
- Might have issue with heap and used memory not unloading
- Track memory
- Debug
- Diagnose GC Leak WIP
- Added glib (not working)
- Edit risky RegisterUICallback mono code
- Restructure logic system to use handles
- Unpin MonoObjects
- Free strings properly
- Remove GC when loading scene explicitly
Engine
- Particle System C# Interface
- Particle System Directional Accel WIP
- Autosave tweak
- Disable mono log calls in game-release
Misc
- License logos
Shawn
 — 1/4/2025 4:32 am
WEEK 13
Gameplay
- Fix level replay bug
Mono
- Set static object vars to null at end of scene
- Attempt ForceGC tweak
- Try unsubscribing from events for components at end of scene
Shawn
 — 1/4/2025 6:47 am
WEEK 13
Gameplay
- fps counter tweak
- tutorial
- option button in main menu
Mono
- clear gc tweak
Misc
- Added assets
Bryan
 — 5/4/2025 1:31 am
Loading screen (WEEK 13)
- Made loading screen more responsive by adding the option of loading assets in batches
- Splash screens are now skippable by pressing space
Shawn
 — Yesterday at 1:39 am
WEEK 13
Game
- UI tweaks
- Disabled calibration on first play
- Calibration offset tweak
- Options
- Score tweaks
- Fade in time tweak for music
- sfx vol tweak
- Option menu FPS counter
- Tutorial if noob
- BGM for main menu, options, level select, results screen
- Results text color
- Hold feedback tweak
- game bottom ui
- animations for codeb in results
- Options save tweak
- Goal marker animation
Engine
- Help with Ctrl Alt Del, Alt Tab interruption code
- Reload scene option in editor "File" button
- Check for every key trigger / down / released
- Particle direction acceleration interface / serialization / C#
- Reload mono option for loading scene (works in preventing crash if used for loading Track 1 - 3, currently not used)
Misc
- Assets
- Audio assets
Fixes
- Pause button bug for editor
- Audio not looping
- Rainbow vfx appearing with normal vfx
- Track scene bugs
Shawn
 — Yesterday at 8:51 pm
WEEK 13
- Reload mono when loading Track 1 - 3
- Fix settings for audio
- Pause / Resume for Animation / Audio
- Pause screen Quit Button actually quits
- Fix unfocus window still entering results screen(?)
- Fix warnings