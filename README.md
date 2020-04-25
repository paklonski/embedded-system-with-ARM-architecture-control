# Embedded system with ARM architecture. The Control of Two Reflectors.

The program for MZAPO board that controls two RGB reflectors.

A pair of RGB LEDs, named as LED1 and LED2 on PCB, are shown in the bottom quarter and represent both reflectors. Enter input parameters using the three rotary knobs marked ENCODER1, ENCODER2, and ENCODER3 on the PCB.

The **User Manual** you will find in the package 'user_manual'.   

**NECESSARY FUNCTIONS**

The color of the reflector light color is always selected in the HSV / HSB color model, see http://colorizer.org/ . Be aware that the RGB-LED cannot distinguish the full range of colors as displays, but depending on its brightness, it displays from 256 (extreme) to 1024 (medium brightness). Select the change steps accordingly. On the web, you will find a number of C programs to convert from HSV to RGB.

**Control clarity**

1. The LCD display shows the selected mode and instantaneous adjustment of both reflectors. The option is changed by the knobs.
2. The LCD display shows not only the mode and settings but also lists a clear text menu where you can select the settings.
3. The same as in the previous point, but the display can be set not the only normal font, but also double magnified font, so that even people with poor eyesight can read it well.

**Light effects**

Effects are added to reflector lights and their points are added together. The unit must remember effects, i.e. after setting another effect, you can return to the previous one without having to enter its values again. If the effect is running, its values can be changed without having to interrupt it, so the result is immediately visible.

1. Base points - static light whose color is selected in three modes. Each reflector can be set:        
  * individually - only the selected reflector is set and the other is not changed;        
  * together - they change simultaneously;        
  * copy - the reflector takes the setting from the other.           
2. Continuous color change - the reflector continuously changes between a pair of colors along a track in the HSV model. Again, this effect can only be selected “individually” for a single spotlight, or “common” for both, or for both in anti-phase. The change time can be set the same as the end colors.
3. Flashing - lighting on with set color can be interrupted with three independently selected times: the duration of continuous illumination, the extinction time, and the phase shift of one reflector to the other.
  * individually - the flashing is only set for the selected spotlight and the other is not changed.
  * together - set at the same time for both reflectors.
  * phase offset - With one spotlight, the phase of the set flashing can be shifted.

-------------------       

**THE TEMPLATE FILES**               

**Makefile** - rules for building and remote running of an application         
**Change_me.c** - source code with main function main - template to be renamed         
**Font_prop14x16.c** - raster proportional width font 14×16 (width x height points)         
**Font_rom8x16.c** - raster fixed width font 8×16         
**Font_types.h** - definition of type used for font description         
**Mzapo_parlcd.h** - declaration of functions for low level access to the LCD display         
**Mzapo_parlcd.c** - implementation of low-level access to the LCD display         
**Mzapo_phys.h** - declaration of function for mapping the physical range of addresses into the application process         
**Mzapo_phys.c** - implementation of a physical address mapping function into an application process         
**Mzapo_regs.h** - definition of base addresses and MZ_APO tutorials         
A more detailed description of the design registers can be found in documentation of MZ_APO.

**ADDED FILES TO COMPLETE THE TASK**

**Main.c** - include the control of the whole program        
**Externals.c** - include global variables used by project functions menu_creator.c - include functions that draw menu sections depending on the selected scale        
**Set_continuous_change.c** - include functions created when starting up the glow of lamps. Presented as threads that stop if the user chooses STOP ALL on the menu screen.          
**Set_flashing.c** - include functions that turn off and turn on the necessary lamps for a user-defined period of time. Presented in the form of threads, which the user independently stops by clicking on STOP ALL in the selection menu.               
**Set_static_zoom1x.c** - include functions that perform static glow adjustment with text menu adjustment for 1x scale        
**Set_static_zoom2x.c** - includes functions that perform static glow adjustment with text menu adjustment for 2x scale        
**Set_time.c** - include functions that adjust the time intervals required during the program, as well as adjust the display value on the screen depending on the selected scale        
**Utils.c** - include functions necessary to translate the HSV value into the RGB values, as well as the initial setup of the program, and so on        

**Comment:**      
The user on the screen selects the necessary colors, then their values are stored in the structures in the file ‘externals.h’. Starting and lighting of lamps, as well as their flashing is carried out in the form of threads. They are stopped if the user changes the flag that controls the execution of the thread. Each menu item is adjusted depending on the scale for clarity and ease of use. If a substantial rearrangement of menu items is necessary, the functions were carried out separately with the prefix _zoom1x or _zoom2x.    


