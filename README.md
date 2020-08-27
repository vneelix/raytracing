# Raytracing C/OpenCL

This project is meant to generate images according to Raytracing protocol.
Project based on SDL2 library, check whether the library is available on your system.
This version uses the **GPU** available on your system **(OpenCL)**.

## Features:
- ### Direct light
- ### Limited objects: cone, cylinder, plane, paraboloid
- ### Bump mapping and color disruption
- ### External files for scene description
- ### Reflection
- ### Transparency
- ### Soft shadows
- ### Composed elements: cubes, pyramids
- ### Textures

You can change the scene while the program is running.\
You can use the left mouse button to focus on an object then use the right mouse button to rotate around it.\
If focus is missing, right mouse button to rotate camera on space.\
Arrows allow you to move around the scene or move objects.\
You can change object color. Just focus them and press 'C' - open color palette.\

## There are two modes full render and fast render:
Fast render activate automatically when you you make any movement.\
Full render activate when you press 'T'. In full mode you can configure some params:\
- 'Q' On/Off ambient
- 'W' On/Off antialiasing
- 'E' On/Off recursion (reflect, refract)
- 'R' On/Off soft shadows

<figure>
    <img src="pic/showcase.png" />
    <img src="pic/spheres2.png" />
</figure>

#### Ambient
<table>
  <tr>
    <td>Not active</td>
     <td>Active</td>
  </tr>
  <tr>
    <td><img src="pic/all_figures_soft.png"></td>
    <td><img src="pic/ambient.png"></td>
  </tr>
 </table>
 
#### Ambient
<table>
  <tr>
    <td>Not active</td>
     <td>Active</td>
  </tr>
  <tr>
    <td><img src="pic/soft_shadows.png"></td>
    <td><img src="pic/soft_shadows_ambient.png"></td>
  </tr>
 </table>
