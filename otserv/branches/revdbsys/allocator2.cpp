//////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
//////////////////////////////////////////////////////////////////////
// memory allocator2
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#include "otsystem.h"
#include "allocator.h"

//normal new/delete
void* operator new(size_t bytes)
{
	return PoolManager::getInstance().allocate(bytes);
}

void* operator new[](size_t bytes)
{
	return PoolManager::getInstance().allocate(bytes);
}

void operator delete(void *p)
{
	PoolManager::getInstance().deallocate(p);
}

void operator delete[](void *p){
	PoolManager::getInstance().deallocate(p);
}

//dummy new/delete operators
void* operator new(size_t bytes, int dummy)
{
	return malloc(bytes);
}
