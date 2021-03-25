# Bube - A Sauerbraten fork
Bube is a fork of Sauerbraten, To... well, I don't have a finalised goal.\
This is *not* Sauerbraten, You can find that project [here](http://sauerbraten.org/).

## Changes - thus far;
 * Restructured the repository.
 * Switched to GIT from SVN.
 * Switched to CMake :feelsgood:
 * Sounds have been converted from IDs to Names. Internally, the sound system still uses ids, but the rest of the engine uses names. Much better, IMO.
 * The error texture can now be generated at run-time, removing the need to have a `notexture.png`. Indeed, the base game here does not supply one.
 * Added some mock Lua 5.1 support, but no mappings. To test a lua file, run `/luadofile <fp>`.
 * Fonts are now loaded with FreeType! Gone are the days of awful bitmap fonts.

## Planned Changes (No Order)
- [ ] Increase size of maps.
- [ ] Increase hard-coded limits in general.
- [ ] Remove some hard-coded stuff.
- [ ] Specifically remove hard-coded sound names.
- [x] Add a scripting language.\
      *Lua 5.1!*
- [ ] Potentially replace GUI with better library.\
      *Although all the GUIs are 3D. This is an end-of-the-road type of deal.*

## Planned Features
- [] Backwards-compatability with Cube 2 mods/games\
      **Note:** I am currently unsure, due to changes in the sound system, if there is still compatability with ALL mods/games.

## License
Sauerbraten uses the zLib License.\
The notice included with the original source code is below.\
You can also find a copy in `src/readme_source.txt` - a file I'm not sure if I can legally remove.
```
Sauerbraten game engine source code, any release.

Copyright (C) 2001-2020 Wouter van Oortmerssen, Lee Salzman, Mike Dysart, Robert Pointon, and Quinton Reeves

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
```

Additionally, Sauerbraten (and by extension Bube) uses the ENet Network Library.\
ENet is licensed under the MIT License, and is compatible with the zLib License.

[Lua](https://www.lua.org/license.html) is licensed under the MIT license, but Bube does not provide sources for it. It is up to you to install and make available the Lua 5.1 sources.

[FreeType](https://www.freetype.org/) is licensed under the FLT license.