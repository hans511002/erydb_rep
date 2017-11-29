/* Copyright (C) 2014 EryDB, Inc.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; version 2 of
   the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA. */

/******************************************************************************
 * $Id: logicalpartition.cpp 1823 2013-01-21 14:13:09Z rdempsey $
 *
 *****************************************************************************/

/** @file
 * Put LogicalPartition define here to avoid header file include confilct.
 * This struct will be used in connector, brm and writeengine
 */

#include <cstdio>
#include <sstream>
#include "bytestream.h"
#include "logicalpartition.h"
#include "oamcache.h"

namespace BRM
{
    /////////////////DBROOTS_struct///////////////////////////////
DBROOTS_struct::DBROOTS_struct()
{
    memset(dbRoots,0,sizeof(BRM::DBROOTS_struct));
    //for (int n=0; n < MAX_DATA_REPLICATESIZE ; n++)
    //{
    //    dbRoots[n]       = 0;
    //}
};
DBROOTS_struct::DBROOTS_struct(const DBROOTS_struct& e)
{
    memcpy(&dbRoots,&e.dbRoots,sizeof(DBROOTS_struct));
};
DBROOTS_struct& DBROOTS_struct::operator= (const DBROOTS_struct&e)
{
    memcpy(&dbRoots,&e.dbRoots,sizeof(DBROOTS_struct));
	return *this;
};


DBROOTS_struct& DBROOTS_struct::set(const DBROOTS_struct& e) {
    memcpy(&dbRoots, &e.dbRoots, sizeof(DBROOTS_struct));
    return *this;
};
uint16_t & DBROOTS_struct::operator [](int i){
    return dbRoots[i];
};
uint16_t DBROOTS_struct::get(int i)const{
    return dbRoots[i];
};
uint16_t DBROOTS_struct::getPmDbr(uint16_t pmid)const{
    oam::OamCache* oamcache = oam::OamCache::makeOamCache();
    int dbrIndex = 0;
    while (dbrIndex < 4)
    {
        uint16_t dbr = dbRoots[dbrIndex];
        dbrIndex++;
        if (dbr == 0)break; 
        if (!oamcache->existDbroot(dbr,pmid))continue;
        return dbr;
    }
    return 0;
};

void DBROOTS_struct::remove(uint16_t dbr){
    int n=-1,j=0;
    while (++n < MAX_DATA_REPLICATESIZE && dbRoots[n]!=0 ) {
        if( dbRoots[n] == dbr){
            for (j=n; j < MAX_DATA_REPLICATESIZE-1 ; j++){
                if(!dbRoots[j+1])
                    break;
                dbRoots[j]=dbRoots[j+1];
            }
            dbRoots[j]=0;
            break;
        }
    }
};

void DBROOTS_struct::serialize(messageqcpp::ByteStream &bs) const{
    for (int n=0; n<MAX_DATA_REPLICATESIZE; n++){
        bs<<dbRoots[n];
    }
};

void DBROOTS_struct::deserialize(messageqcpp::ByteStream &bs) {
    for (int n=0; n<MAX_DATA_REPLICATESIZE; n++){
        bs>>dbRoots[n];
    }
};
bool operator==( const BRM::DBROOTS_struct &a , const BRM::DBROOTS_struct &b){
    for (int n=0; n<MAX_DATA_REPLICATESIZE; n++)
    {
        if(a.dbRoots[n] != b.dbRoots[n]){
            return false;
        }
    }
    return true;
};
bool operator!=( const BRM::DBROOTS_struct &a , const BRM::DBROOTS_struct &b){
    for (int n=0; n<MAX_DATA_REPLICATESIZE; n++)
    {
        if(a.dbRoots[n] != b.dbRoots[n]){
            return true;
        }
    }
    return false;
};
bool operator>( const BRM::DBROOTS_struct& a, const BRM::DBROOTS_struct& b){
    return a.dbRoots[0] > b.dbRoots[0];
};
bool operator>=( const BRM::DBROOTS_struct& a, const BRM::DBROOTS_struct& b){
    return a.dbRoots[0] >= b.dbRoots[0];
};
bool operator<( const BRM::DBROOTS_struct& a, const BRM::DBROOTS_struct& b){
    return a.dbRoots[0] < b.dbRoots[0];
};
bool operator<=( const BRM::DBROOTS_struct& a, const BRM::DBROOTS_struct& b){
    return a.dbRoots[0] <= b.dbRoots[0];
};
std::ostream & operator<<(std::ostream &os, const DBROOTS_struct & a){
    for (int n=0; n<MAX_DATA_REPLICATESIZE; n++){
        if(n>0){
            os<<".";
        }
        os <<  a.dbRoots[n]  ;
    }
	return os;
};
std::istream & operator>>(std::istream & is, DBROOTS_struct &a){
    for (int n=0; n<MAX_DATA_REPLICATESIZE; n++){
        if(n>0){
            is.ignore();
        }
        is >>  a.dbRoots[n]  ;
    }
	return is;
};

////////////////////////////////////////////////
std::string LogicalPartition::toString() const
{
	char buf[256] = {0};
	//std::sprintf(buf, "%d.%d.%ulld", pp, seg, dbRoot.getUintVal());
	std::sprintf(buf, "%d.%d.%d.%d.%d.%d", pp, seg, dbRoot.dbRoots[0], dbRoot.dbRoots[1], dbRoot.dbRoots[2], dbRoot.dbRoots[3]);
	return std::string(buf);
}

/**
 * ostream operator
 */
std::ostream& operator<<(std::ostream& output, const LogicalPartition& rhs)
{
	output << rhs.pp << "." << rhs.seg << "." << rhs.dbRoot;
	return output;
}

std::istream& operator>>(std::istream& input, LogicalPartition& rhs)
{
	input >> rhs.pp;
	input.ignore();
	input >> rhs.seg;
	input.ignore();
	input >> rhs.dbRoot;
	return input;
}

/**
 * bytestream operator
 */
messageqcpp::ByteStream& operator<<(messageqcpp::ByteStream &bs, const LogicalPartition& rhs)
{
	rhs.serialize(bs);
	return bs;
}

messageqcpp::ByteStream& operator>>(messageqcpp::ByteStream &bs, LogicalPartition& rhs)
{
	rhs.unserialize(bs);
	return bs;
}

}
