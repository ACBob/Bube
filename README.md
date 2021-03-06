# Bube - A Sauerbraten fork
Bube is a fork of Sauerbraten, To... well, I don't have a finalised goal.\
This is *not* Sauerbraten, You can find that project [here](http://sauerbraten.org/).

## Changes - thus far;
 * Restructured the repository.
 * Switched to GIT from SVN.
 * Switched to CMake :feelsgood:
 * Sounds have been converted from IDs to Names. Internally, the sound system still uses ids, but the rest of the engine uses names. Much better, IMO.
 * The error texture can now be generated at run-time, removing the need to have a `notexture.png`. Indeed, the base game here does not supply one.
 * Fonts are now loaded with FreeType! Gone are the days of awful bitmap fonts.
 * AngelScript scripting. To run a file, run `/script <fp>`.

## Planned Changes (No Order)
- [ ] Improve certain hard-coded limitations.
- [x] Add a scripting language.\
- [ ] Extend / Replace GUI System.\
      *Not going to happen soon!*
- [ ] Source:tm: Engine Style Entity I/O.\
      *Connected to scripts? See [#3](https://github.com/ACBob/Bube/issues/3)*

**Note:** Backwards-compatability with Cube 2 engine games is NOT garunteed, or a goal. If something works, It's a minor miracle.

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

[AngelScript](http://www.angelcode.com/angelscript/) is licensed under the zLib license.

[FreeType](https://www.freetype.org/) is licensed under the FLT license.