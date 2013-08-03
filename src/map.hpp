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

#ifndef _MAP_H_
#define _MAP_H_

struct Tile
{
    bool explored_; // has the player already seen this tile ?
    Tile() : explored_(false) {}
};
 
class Map : public Persistent
{
public:
	int32_t width_;
	int32_t height_;
 
    Map(int32_t width, int32_t height);
    virtual ~Map();
    bool IsWall(int32_t x, int32_t y) const;
	bool IsInFov(int32_t x, int32_t y) const;
    bool IsExplored(int32_t x, int32_t y) const;
    bool CanWalk(int32_t x, int32_t y) const;
    void ComputeFov();
    void Render() const;

	void Load(TCODZip &zip);
	void Save(TCODZip &zip);
 	void Init(bool withActors);

protected:
    Tile *tiles_;
    TCODMap *map_;
    long seed_;
    TCODRandom *rng_;
    friend class BspListener;
 
    void Dig(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
    void CreateRoom(bool first, int32_t x1, int32_t y1, int32_t x2, int32_t y2,bool withActors);
    void AddMonster(int32_t x, int32_t y);
    void AddItem(int32_t x, int32_t y);
};

#endif // _MAP_H_
