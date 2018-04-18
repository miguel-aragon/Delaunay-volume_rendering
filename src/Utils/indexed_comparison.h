/*

Fast pseudo volume rendering with Delaunay Tessellations.
Copyright (C) 2009-2018 Miguel Angel Aragon-Calvo

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

If you wish to contact the author write to:
		miguel.angel.aragon.calvo@gmail.com

*/
#ifndef INDEXED_COMPARISON_H
#define INDEXED_COMPARISON_H

class compare  {
public:
	std::vector<float> * objects;

	compare(std::vector<float>* obj) : objects(obj) {}

	bool operator ()(const int p1,const int p2)
	{
		return((*objects)[p1] < (*objects)[p2]);
	}
};

#endif
