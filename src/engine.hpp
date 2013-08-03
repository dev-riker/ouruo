///////////////////////////////////////////////////////////////////////////////////////
//                                                                                   //
//	This is OurUO Tech Demo, for more info on this project, please refer to:         //
//	                                                                                 //
// 	https://github.com/OurUO/ouruo-server                                            //
// 		                                                                             //
// 	Copyright (C) 2013  Developer Riker (https://github.com/dev-riker)               //
// 	Copyright (C) 2013  Developer Sebastien (https://github.com/devsebastien)        //
//                                                                                   //
// 	This program is free software; you can redistribute it and/or                    //
// 	modify it under the terms of the GNU General Public License                      //
// 	as published by the Free Software Foundation; either version 2                   //
// 	of the License, or (at your option) any later version.                           //
//                                                                                   //
// 	This program is distributed in the hope that it will be useful,                  //
// 	but WITHOUT ANY WARRANTY; without even the implied warranty of                   //
// 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                    //
// 	GNU General Public License for more details.                                     //
//                                                                                   //
// 	You should have received a copy of the GNU General Public License                //
// 	along with this program; if not, write to the Free Software                      //
// 	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.  //
//                                                                                   //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _ENGINE_H_
#define _ENGINE_H_

class Engine
{
public:
	enum GameStatus
	{
		STARTUP,
		IDLE,
		NEW_TURN,
		VICTORY,
		DEFEAT
	} gameStatus;
	
	TCOD_key_t lastKey_;
	TCOD_mouse_t mouse_;
    TCODList<Actor *> actors_;
    Actor *player_;
    Map *map_;
    int32_t fovRadius_;
    int32_t screenWidth_;
    int32_t screenHeight_;
    Gui *gui_;

    Engine(int32_t screenWidth, int32_t screenHeight);
    virtual ~Engine();
    void Update();
    void Render();
    void SendToBack(Actor *actor);
    Actor *GetActor(int32_t x, int32_t y) const;
    Actor *GetClosestMonster(int32_t x, int32_t y, float range) const;
    bool PickATile(int32_t *x, int32_t *y, float maxRange = 0.0f);
   	void Load();
	void Save();
    void Init();
    void Term();
};
 
extern Engine engine;

#endif // _ENGINE_H_
