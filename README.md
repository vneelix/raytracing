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
