cmake_minimum_required(VERSION 3.16)

include_directories(../include ../enet/include ../shared ../engine)
set(CMAKE_CXX_FLAGS "-O3 -fomit-frame-pointer -ffast-math -Wall -fsigned-char -fno-exceptions -fno-rtti")

set(rpggameSources
	../shared/crypto.cpp
	../shared/geom.cpp
	../shared/glemu.cpp
	../shared/stream.cpp
	../shared/tools.cpp
	../shared/zip.cpp
	../engine/3dgui.cpp
	../engine/bih.cpp
	../engine/blend.cpp
	../engine/blob.cpp
	../engine/client.cpp
	../engine/command.cpp
	../engine/console.cpp
	../engine/cubeloader.cpp
	../engine/decal.cpp
	../engine/dynlight.cpp
	../engine/glare.cpp
	../engine/grass.cpp
	../engine/lightmap.cpp
	../engine/main.cpp
	../engine/material.cpp
	../engine/menus.cpp
	../engine/movie.cpp
	../engine/normal.cpp
	../engine/octa.cpp
	../engine/octaedit.cpp
	../engine/octarender.cpp
	../engine/physics.cpp
	../engine/pvs.cpp
	../engine/rendergl.cpp
	../engine/rendermodel.cpp
	../engine/renderparticles.cpp
	../engine/rendersky.cpp
	../engine/rendertext.cpp
	../engine/renderva.cpp
	../engine/server.cpp
	../engine/serverbrowser.cpp
	../engine/shader.cpp
	../engine/shadowmap.cpp
	../engine/sound.cpp
	../engine/texture.cpp
	../engine/water.cpp
	../engine/world.cpp
	../engine/worldio.cpp
	../rpggame/ent.cpp
	../rpggame/entities.cpp
	../rpggame/objset.cpp
	../rpggame/rpg.cpp
)

add_library(rpggame ${rpggameSources})