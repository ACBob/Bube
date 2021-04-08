// Loosely inspired from GoldSRC' implementation in the public Ricochet SDK; https://github.com/ValveSoftware/halflife/blob/master/ricochet/dlls/cbase.h
// NOTE: Using ricochet was an accident, it was the first github displayed in the search results.
// TODO: Is using it as reference legal? Although I'm not sure valve care in the first place....

#include "engine.h"

class BaseEntity {
	public:

		// Init Functions
		virtual void Spawn(void) = 0; // Called when the entity enters the world
		virtual void Precache(void) = 0; // Called to precache resources

		// Callbacks
		void (BaseEntity ::*Think)(void);

		// Virtual Functions
		// TODO: vec APPEARS to be the 3d vector class, but I am yet to verify this.
		virtual vec GetPosition() { return position; };


		// Ported from physent
			vec o, vel, falling;  // origin, velocity
			vec deltapos, newpos; // movement interpolation
			float yaw, pitch, roll;
			float maxspeed;					   // cubes per second, 100 for player
			float radius, eyeheight, aboveeye; // bounding box size
			float xradius, yradius, zmargin;
			vec floor; // the normal of floor the dynent is on

			ushort timeinair;
			uchar inwater;
			bool jumping;
			schar move, strafe;

			uchar physstate;		// one of PHYS_* above
			uchar state, editstate; // one of CS_* above
			uchar type;				// one of ENT_* above
			uchar collidetype;		// one of COLLIDE_* above

			bool blocked; // used by physics to signal ai

			// TODO: what did this do?
			// physent()
			// 	: o(0, 0, 0), deltapos(0, 0, 0), newpos(0, 0, 0), yaw(0), pitch(0), roll(0), maxspeed(100), radius(4.1f),
			// 	eyeheight(14), aboveeye(1), xradius(4.1f), yradius(4.1f), zmargin(0), state(CS_ALIVE), editstate(CS_ALIVE),
			// 	type(ENT_PLAYER), collidetype(COLLIDE_ELLIPSE), blocked(false)
			// {
			// 	reset();
			// }

			void resetinterp()
			{
				newpos = o;
				deltapos = vec(0, 0, 0);
			}

			void reset()
			{
				inwater = 0;
				timeinair = 0;
				jumping = false;
				strafe = move = 0;
				physstate = PHYS_FALL;
				vel = falling = vec(0, 0, 0);
				floor = vec(0, 0, 1);
			}

			vec feetpos(float offset = 0) const
			{
				return vec(o).add(vec(0, 0, offset - eyeheight));
			}
			vec headpos(float offset = 0) const
			{
				return vec(o).add(vec(0, 0, offset));
			}

			bool maymove() const
			{
				return timeinair || physstate < PHYS_FLOOR || vel.squaredlen() > 1e-4f || deltapos.squaredlen() > 1e-4f;
			}
		// End ported from physent
	
	protected:
		vec position; // Position in 3d space
};

// TODO: I have no idea what this does, I copied it purely from the goldsrc sdk
#define SetThink( ent ) Think = static_cast<void (BaseEntity::*)(void)> (a)


class BasePlayer : public BaseEntity {
	public:
	// Ported from dynent
		bool k_left, k_right, k_up, k_down; // see input code

		entitylight light;
		animinterpinfo animinterp[MAXANIMPARTS];
		ragdolldata *ragdoll;
		occludequery *query;
		int lastrendered;
		uchar occluded;

		void stopmoving()
		{
			k_left = k_right = k_up = k_down = jumping = false;
			move = strafe = 0;
		}

		void reset()
		{
			BaseEntity::reset();
			stopmoving();
			loopi(MAXANIMPARTS) animinterp[i].reset();
		}

		vec abovehead()
		{
			return vec(o).add(vec(0, 0, aboveeye + 4));
		}
};