//----------------------------------------------------------------------------
//
// Copyright(C) 2010 James Haley
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//----------------------------------------------------------------------------
//
// EDF Sprites Module
//
// By James Haley
//
//----------------------------------------------------------------------------

#include "z_zone.h"
#include "info.h"
#include "m_dllist.h"

#define NEED_EDF_DEFINITIONS

#include "Confuse/confuse.h"
#include "e_edf.h"
#include "e_lib.h"
#include "e_hash.h"
#include "e_sprite.h"

// Data

static int     numspritesalloc; // number of sprites allocated
static ehash_t spritehash;      // sprite hash table

// Sprite hashing

typedef struct esprite_s
{
   struct mdllistitem_s link; // hash links
   int num;                   // sprite number
   char name[5];              // sprite name
   char *nameptr;             // pointer to name
} esprite_t;

E_KEYFUNC(esprite_t, nameptr)

//
// E_SpriteNumForName
//
// Sprite hashing function. Returns the index of "name" in the sprnames array,
// if found. If not, returns -1.
//
int E_SpriteNumForName(const char *name)
{
   void *obj;
   int spritenum = -1;

   if((obj = E_HashObjectForKey(&spritehash, &name)))
      spritenum = ((esprite_t *)obj)->num;

   return spritenum;
}

//
// E_AddSprite
//
// haleyjd 03/23/10: Add a sprite name to sprnames, if such is not already 
// present. Returns true if successful and false otherwise.
//
static boolean E_AddSprite(const char *name, esprite_t *sprite)
{
   // initialize the esprite object   
   strncpy(sprite->name, name, 4);
   sprite->num = NUMSPRITES;
   sprite->nameptr = sprite->name;

   // initialize the sprite hash if it has not been initialized
   if(!spritehash.isinit)
      E_NCStrHashInit(&spritehash, 257, E_KEYFUNCNAME(esprite_t, nameptr), NULL);
   else if(E_HashObjectForKey(&spritehash, &name))
      return false; // don't add the same sprite name twice
   
   E_EDFLogPrintf("\t\tAdding spritename %s\n", name);
   
   // add esprite to hash
   E_HashAddObject(&spritehash, sprite);

   // reallocate sprnames if necessary
   if(NUMSPRITES + 1 >= numspritesalloc)
   {
      numspritesalloc = numspritesalloc ? numspritesalloc + 128 : 256;
      sprnames = (char **)(realloc(sprnames, numspritesalloc * sizeof(char *)));
   }

   // set the new sprnames entry, and make the next one NULL
   sprnames[NUMSPRITES]     = sprite->name;
   sprnames[NUMSPRITES + 1] = NULL;

   ++NUMSPRITES;

   return true;
}

//
// E_ProcessSprites
//
// haleyjd 03/23/10: Processes the EDF spritenames array, which is now largely
// only provided for the purpose of giving DOOM's sprites their proper DeHackEd
// numbers. Sprites defined through DECORATE-format state definitions will be
// added separately.
//
// When loading sprites from wad lumps at runtime, they will be added to any that
// already exist, rather than overwriting them. This shouldn't make any practical
// difference.
//
void E_ProcessSprites(cfg_t *cfg)
{
   esprite_t *sprites;
   int numarraysprites;
   int i;

   E_EDFLogPuts("\t* Processing spritenames array\n");

   // get number of sprites in the spritenames array
   numarraysprites = cfg_size(cfg, SEC_SPRITE);
   
   E_EDFLogPrintf("\t\t%d sprite name(s) defined\n", numarraysprites);
   
   // At least one sprite is required to be defined through the 
   // spritenames array, but only when no sprites have been defined
   // already.
   if(!numarraysprites)
   {
      if(!NUMSPRITES)
         E_EDFLoggedErr(2, "E_ProcessSprites: no sprite names defined.\n");
      return;
   }

   // 10/17/03: allocate a single array of sprite objects to save a lot of
   // memory and some time.
   sprites = (esprite_t *)(calloc(numarraysprites, sizeof(*sprites)));

   // process each spritename
   for(i = 0; i < numarraysprites; ++i)
   {
      const char *sprname = cfg_getnstr(cfg, SEC_SPRITE, i);

      // spritenames must be exactly 4 characters long
      if(strlen(sprname) != 4)
      {
         E_EDFLoggedErr(2, 
            "E_ProcessSprites: invalid sprite name '%s'\n", sprname);
      }

      // add the sprite
      E_AddSprite(sprname, &sprites[i]);
   }

   E_EDFLogPuts("\t\tFinished spritenames\n");
}

//
// E_ProcessSingleSprite
//
// haleyjd 03/24/10: Adds a single sprite, such as one being implicitly defined
// by an EDF frame or DECORATE state. Returns true if successful and false if
// there is any kind of problem during the process.
//
boolean E_ProcessSingleSprite(const char *sprname)
{
   esprite_t *spr;

   // must be exactly 4 characters
   if(strlen(sprname) != 4)
      return false;

   // allocate separate storage for implicit sprites
   spr = (esprite_t *)(calloc(1, sizeof(*spr)));

   // try adding it; if this fails, we need to free spr
   if(!E_AddSprite(sprname, spr))
   {
      free(spr);
      return false;
   }
   
   return true;
}

// EOF
