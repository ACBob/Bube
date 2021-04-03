Sauerbraten RPG
===============

Running
-------

To launch the game, run the \"rpg.bat\" file which is included in the
main Sauerbraten install directory.

Building
--------

A Visual Studio solution and a Code::Blocks project file are included
for building in the \"src\\rpggame\\\" directory within the Sauerbraten
install directory.

Gameplay Scripting {#gameplay_scripting}
------------------

The RPG is set up in a very open way such that all gameplay objects and
behaviour can be modified in a script file, currently in
data/game\_rpg.cfg (this will change later to be on a per-map basis).
The idea is that every map comes with its own \"mod\", and that gameplay
can be influenced by level designer, allowing gameplay types spanning
from a stat/story influenced FPS game, an action RPG, all the way to
more complex quest/story/stat based RPGs.

This file simply lists all gameplay objects, which can then be placed in
the level editor. For example, by writing:

``` {#spawn_apple}
spawn_apple = [ r_worth 20 ]
```

you define a script that gets executed when an apple is spawned. You
place this apple either directly in the world or as part of another
objects inventory. To place it in the world, you create an entity of
type \"spawn\" with a name attached. This is best done by typing (in the
console):

``` {#newrpgspawn}
newrpgspawn apple
```

which will show you your new entity in the editor. Running the map then
will create a new apple object at that location, with all stats set to
your liking (in this case just its worth).

What is important to realize is that most r\_ commands work with what is
called the RPG stack. This makes it possible to apply properties and
actions to things without having to specify all the time which object
you are talking about. For example, when the spawn\_apple script gets
executed, the apple object will be on the top of the RPG stack, so any
commands such as r\_worth that apply to an object, will automatically
apply to it. It is a stack, because during the initialization of one
object you may spawn another, which means all r\_ commands will apply to
the new object, until it is done, when the RPG stack returns to the
previous object.

Following is a list of all commands\... unless otherwise noted, they
apply to the current object (top of the RPG stack).

RPG object commands {#rpg_object_commands}
-------------------

``` {#r_pop}
r_pop
```

Removes the top object from the RPG stack

``` {#r_spawn}
r_spawn N
```

creates a new object, places it on the top of the RPG stack, then
executes spawn\_N to initialize it.

``` {#r_contain}
r_contain T
```

Takes the object on the top of the stack, and makes it part of the
inventory of the object on the stack below it. T is the type of
inventory object, see the convenient scripts r\_inventory, r\_loot,
r\_fortrade instead.

``` {#r_model}
r_model M
```

Specify model M to represent this object visually when placed in the
world

``` {#r_action}
r_action N A
```

``` {#Pre3}
r_quest N A
```

defines a custom action the player can take on this object. N is the tag
word that describes the action, visible in the objects menu in the game
world, and A is the script that gets executed when the player selects
it.

When A gets executed, the top of the stack will be object on which the
action is defined, below that target of the action, and below that the
user of the action. For example, the player (user) casts a fireball
(object) onto a monster (target). Or, the player (user/target) interacts
with an npc (object).

A special tag is \"use\", this is the default action for items to be
executed when the user clicks on them in the inventory, or, it is the
action that gets executed when the player casts the currently selected
spell (also see usetype below).

r\_quest is the same as r\_action, except that it also marks the start
of a quest, with the action being the resolution of the quest. The quest
description is whatever the last r\_say command was. Any r\_take inside
the action with a positive outcome concludes the quest. Quests can be
seen in the quest log.

``` {#r_say}
r_say T
```

Make text T the last thing this object said to the player

``` {#r_take}
r_take N A NA
```

Attempt to take object with name N from the player inventory, and place
it in the objects inventory. If the item was succesfully grabbed (it was
present), action A is executed. If the item was not available, NA is
executed. r\_take is the preferred way to structure resolution of
quests, i.e. everything the player ever needs to do should go via token
items.

``` {#r_give}
r_give N
```

give the player an item with name N

``` {#Pre1}
r_applydamage D
```

does D damage to the object on top of the stack. object one below the
top of the stack must be the one doing the damage.

``` {#Pre2}
r_use
```

select or use the object for the player (see usetype below).

RPG stats & attributes {#H2_1}
----------------------

The following is a list of all stats tracked by each object. A stat is
defined as a property of an object that determines its effectiveness
when performing a certain action, i.e. the \"melee\" stat affects your
melee combat effectiveness. Stats are things that you can build up
thruout the game, hence \"health\" is not a stat, but it is dependent on
a stat, maxhp.

  --------------------- ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  melee                 amount of damage when using a melee weapon (fists, swords, axes\...)
  ranged                amount of damage when using a ranged weapon (crossbow, shotgun, \...)
  magic                 amount of damage when using an attack spell (fireball\...), level of utility when using healing/creation spells
  regen                 amount of hp regenerated per second. regen is the only defense stat, it replaces the traditional armour or resistence stats. regen is more flexible in the sense that it is time based, so your effectiveness depends on being able to give or take sustained damage, and deals better with the effects of running away mid-fight etc.
  focus                 amount of mana regenerated per second
  attackspeed           reduces time delay between two attacks for any attack (melee/ranged/magic)
  movespeed             
  jumpheight            
  maxhp                 maximum health you will regenerate to
  maxmana               maximum mana you can store
  tradeskill            will give your better deals when trading
  feared                the higher this stat, the more likely people are likely to comply with your wishes
  stealth               reduces npc fov & distance when stealing stuff
  hostility             
  stata, statb, statc   generic stats, i.e. their meaning is map-specific. they can be set and accumulated like any other stat, then scripts can use the results in any way they wish.
  --------------------- ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

a stat can be set on any rpg object using the r\_X command, where X is
any of the above stat names, e.g. *r\_melee 50* gives the current object
50 melee points. a script can read the current points using r\_get\_X,
i.e. *echo \"your melee points are: \" (r\_get\_melee)* outputs the
current objects melee points.

A special case are objects that have no stats set directly (typically
the player, and sometimes NPCs): their stats are computed as cumulative
from the stats of all their inventory items. So if the player carries a
gem that has melee 50, and gloves that have melee 20, his own melee
points are 70.

It\'s important to realize that stat points do not directly translate to
damage and such. Points are an indication of efficiency of a particular
stat over the base 100%, along a logarithmic curve. So 0 melee points
translates into 100% melee efficiency, and what 70 melee points
translates into\... well, something above 100%, depending on the log
curve. Every map can define their own log curve for each stat, depending
on how fast you want points to result in increased melee damage. This is
a game balancing thing, in the sense that if the game world makes obtain
items with melee points too easy and the log curve is too gentle, then
players will be overpowered. The formula for melee is (other stats work
similarly):

*meleedamage = weapondamage \* ( log ( points / pointsscale + 1 ) \*
percentscale + 100 ) / 100*

(log = natural logarithm). so with a pointscale of 10, and a
percentscale of 25, our 70 points amount to a 152% melee efficiency,
which, when combined with a weapon that does 10 damage (at 100%), now
does 15 melee damage. For movespeed you\'d have a similar formula, where
instead of weapondamage you\'d have the default running speed.

You can get the efficiency of any stat using r\_eff\_X, e.g.
r\_eff\_melee.

That may seem like a really convoluted way of doing things, but the log
scale is the only way to give a relatively constant challenge level in
the game. Without it, the game would start hard and become too easy real
soon. The UI will show both points and efficiency for stats so you can
easy see the effect without understanding the math.

You can define the log scale by doing: *r\_def\_X pointscale
percentscale*, but this should only really ever be done by the game
designer, or experienced rpg level designers on their own levels, as
tweaking these values requires deep insight into how players will
collect points thruout a game.

It is worth repeating what should be obvious from the above: the player
and npcs don\'t have \"levels\" as in other RPGs. Increased ability is
purely denoted by means of points and efficiency above whatever is the
100% value of the related ability. The end result is similar to
traditional RPGs, but not quite. The above system is more flexible in
the sense that you can have completely independent \"levels\" for all
your abilities. But best of all, since your points are determined by
what you have in your inventory, any way you can get items can increase
your ability, which includes killing, stealing, finding, trading etc.
You can change how you assign your priorities mid-game, by trading melee
items for magic items, for example. You won\'t hit a \"item plateau\" as
in so many RPG games 20% into the game because most items you get are
useless, here you can put everything to *some* use, even if sometimes
minimal.

The following are the attributes of any rpg object. Attrs are simpler in
the sense that they are just variables kept track of per object, and can
be set/changed by either the rpg system or the script code. They are all
default 0 for a newly initialized object.

  --------------------- ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  health                set to maxhp when an AI object spawns in the world, and regens using regen

  mana                  set to maxmana upon spawn, and regens using focus

  gold                  how much money is owned by the object (mostly for npcs/player only)

  worth                 base value when this object is sold by an npc (not necessarily when sold TO an npc)

  ai                    0: no AI (a lifeless object, default), 1: a friendly AI (npc), 2: hostile AI (animals/monsters) 

  useamount             amount of uses this object has. items such as melee weapons have a certain amount of uses before they break, and potion bottles may have a certain content.

  usetype               how this object is used: 0: not a weapon, 1: melee weapon, 2: ranged weapon, 3: magic (projectile).\
                        When you \"use\" an object, an it is not a weapon, its associated action will be executed immediately. When it is a weapon, it is merely selected, and it is then available for repeated execution through the fire button (see attackrate).

  damage                if not 0, this object can be used a weapon

  maxrange              max range within which objects can be targeted when this object is used

  maxangle              max angle from \"forward\" that objects can be targeted

  attackrate            minimum time between two uses of this object

  manacost              amount of mana used up for one use of this object

  effect                the effect associated with this item when it is used\... for spells it is the particle number

  attra, attrb, attrc   generic attributes, can be set by the script code to implement gameplay features not foreseen by the system

  usesound              sound played when this item is used. value must be from a call to \"registersound\"
  --------------------- ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

These can be used similarly using r\_X and r\_get\_X.
