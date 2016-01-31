# dosengine
This is a small project to use as a lazy example piece for future 386-Pentium class demoscene demos. Uses Midas Sound System Library for sound(Supports Sound Blaster's and Gravis Ultrasound) so you'll have to include it to your Watcom project file. Also has PC Speaker support for fun. Hopefully this project will be finished until Assembly Summer 2016, since they MAY HAVE a Oldschool/Midschool demo compo that allows Pentium-class demos. This project will only support VGA. If you wish to support SVGA you'll have to do research for that yourself. Uses a modified version a public domain GIF loader for loading GIFs.

Personally I prefer 486+VGA+GUS so that is my target platform for this engine. Atleast 6 Mb of RAM is required, that seems to be the amount of RAM Future Crew's Unreal requires so I am personally OK with that amount.

## VGA
### Mode 13 - Regular Chunky Mode 
Straightforward mode to handle pixels but you have less freedom to change resolution, nor can you access more than 64 Kb of Video RAM meaning no page flipping is possible, this means you'll have to spend a bunch of CPU cycles moving double buffer data before VGA-card begins to write video data onto screen. Also to my understanding Mode 13 is slower than Mode X in read/write time. 
Due to 16:20 resolution on 4:3 screens pixels will be a bit more wide than tall, a ka fat pixels. Supports 8-bit images easily (a GIF loader and displaying those images functions exists within the demoengine).

### Mode X - Unchained Mode
Memory is split into four groups. For every close group of four pixels each is split into seperate planes. So one byte of display memory can represent more than a single pixel. By writing into all planes one byte write will fill up four pixels so this is ideal for quick fills like flat shaded polygons. 
Understanding Mode X will take time, mastering Mode X will take even more time and it's not always the best option but generally is the recommended one. Unlike bitplane graphics unchained byte still repesents one color. I suggest reading chapters 47-49 from Michael Abrash's Graphics Programming Black Book(PDF online) or use google.
Mode X will allow you to change resolution more freely and you have access to more Video RAM. By changing where VGA card starts reading video memory you essensually can have double buffer memory on video memory. That means you don't have to move data around and spend those valuable cpu cycles. All you have to do is tell VGA card where to start displaying video memory beforce vertical retrace starts.

### Fake hicolour 18-bit mode
This is entirely software baked visual illusion and not a real screen mode. Normally each row of pixels has different color component and on a old CRT monitor these colors will blend together. On this demoengine I use a bit different way of blending colors by dithering to make it look a bit more realistic on TFT monitors but not much can be done to make it look good on modern monitors. 

### EGA mode with VGA palette
This is basicly EGA 320x200 mode but most VGA cards support using VGA palette register to access 24-bit palette in EGA mode. You still only can use 16 colors / frame. Pixels are drawn to bitplane planars similar to Amiga. This mode can be used in some cases to have fast drawn sprites on screen.