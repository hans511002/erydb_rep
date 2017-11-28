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

   //  $Id: systemcatalog.cpp 2101 2013-01-21 14:12:52Z rdempsey $

#include <iostream>
#include <assert.h>
#include <sys/time.h>
using namespace std;

#include "systemcatalog.h"
#include "configcpp.h"
using namespace execplan;

#include "we_colopcompress.h"
#include "resourcemanager.h"
using namespace joblist;

#include "dbbuilder.h"
#include "ddlpkg.h"
using namespace ddlpackage;
#include "we_messages.h"
using namespace WriteEngine;  

#include "oamcache.h"
using namespace oam;

#include "bytestream.h"
using namespace messageqcpp;

#include "erydbsystemcatalog.h"
using namespace execplan;
#include "createtableprocessor.h"
using namespace ddlpackageprocessor;


typedef boost::shared_ptr<std::map<string, int> > StringIntMap;

void SystemCatalog::buildRep()
{
    BRM::DBRM* dbrm = new BRM::DBRM();
    //DDLPackageProcessor ddlProc(dbrm);
    WriteEngine::WEClients  fWEClient(WriteEngine::WEClients::DDLPROC);
    remove();
    cout << "Creating System Catalog..." << endl;
    cout << endl;
    // SYSTABLE
    timeval startTime;
    gettimeofday(&startTime, 0);
    ostringstream msg;
    WErrorCodes ec;
    ByteStream bytestream;
    uint64_t uniqueId = 0;
    uniqueId = dbrm->getUnique64();
    fWEClient.addQueue(uniqueId);
    BRM::TxnID txnID;
    txnID.id = 0;
    txnID.valid = 0;
    int rc = 0;
    DBROOTS_struct dbRoot;
    BRMWrapper::getInstance()->getSysDataDBRoots(&dbRoot);
    cout << "SysDataDBRoots=" << dbRoot << endl;
    StringIntMap colMap;
    colMap.reset(new StringIntMap::element_type());
    int compressionType = 0;
    string errorMsg;
    uint32_t partition = 0;
    uint16_t segment = 0;

    ResourceManager rm;
    std::map<uint32_t, uint32_t> oids;
    if (rm.useHdfs())
    {
        compressionType = 2;
        oids[OID_SYSTABLE_TABLENAME] = OID_SYSTABLE_TABLENAME;
        oids[DICTOID_SYSTABLE_TABLENAME] = DICTOID_SYSTABLE_TABLENAME;
        oids[OID_SYSTABLE_SCHEMA] = OID_SYSTABLE_SCHEMA;
        oids[DICTOID_SYSTABLE_SCHEMA] = DICTOID_SYSTABLE_SCHEMA;
        oids[OID_SYSTABLE_OBJECTID] = OID_SYSTABLE_OBJECTID;
        oids[OID_SYSTABLE_CREATEDATE] = OID_SYSTABLE_CREATEDATE;
        oids[OID_SYSTABLE_LASTUPDATE] = OID_SYSTABLE_LASTUPDATE;
        oids[OID_SYSTABLE_INIT] = OID_SYSTABLE_INIT;
        oids[OID_SYSTABLE_NEXT] = OID_SYSTABLE_NEXT;
        oids[OID_SYSTABLE_NUMOFROWS] = OID_SYSTABLE_NUMOFROWS;
        oids[OID_SYSTABLE_AVGROWLEN] = OID_SYSTABLE_AVGROWLEN;
        oids[OID_SYSTABLE_NUMOFBLOCKS] = OID_SYSTABLE_NUMOFBLOCKS;
        oids[OID_SYSTABLE_AUTOINCREMENT] = OID_SYSTABLE_AUTOINCREMENT;
        //SYSCOLUMN
        oids[OID_SYSCOLUMN_SCHEMA] = OID_SYSCOLUMN_SCHEMA;
        oids[DICTOID_SYSCOLUMN_SCHEMA] = DICTOID_SYSCOLUMN_SCHEMA;
        oids[OID_SYSCOLUMN_TABLENAME] = OID_SYSCOLUMN_TABLENAME;
        oids[DICTOID_SYSCOLUMN_TABLENAME] = DICTOID_SYSCOLUMN_TABLENAME;
        oids[OID_SYSCOLUMN_COLNAME] = OID_SYSCOLUMN_COLNAME;
        oids[DICTOID_SYSCOLUMN_COLNAME] = DICTOID_SYSCOLUMN_COLNAME;
        oids[OID_SYSCOLUMN_OBJECTID] = OID_SYSCOLUMN_OBJECTID;
        oids[OID_SYSCOLUMN_DICTOID] = OID_SYSCOLUMN_DICTOID;
        oids[OID_SYSCOLUMN_LISTOBJID] = OID_SYSCOLUMN_LISTOBJID;
        oids[OID_SYSCOLUMN_TREEOBJID] = OID_SYSCOLUMN_TREEOBJID;
        oids[OID_SYSCOLUMN_DATATYPE] = OID_SYSCOLUMN_DATATYPE;
        oids[OID_SYSCOLUMN_COLUMNLEN] = OID_SYSCOLUMN_COLUMNLEN;
        oids[OID_SYSCOLUMN_COLUMNPOS] = OID_SYSCOLUMN_COLUMNPOS;
        oids[OID_SYSCOLUMN_LASTUPDATE] = OID_SYSCOLUMN_LASTUPDATE;
        oids[OID_SYSCOLUMN_DEFAULTVAL] = OID_SYSCOLUMN_DEFAULTVAL;
        oids[DICTOID_SYSCOLUMN_DEFAULTVAL] = DICTOID_SYSCOLUMN_DEFAULTVAL;
        oids[OID_SYSCOLUMN_NULLABLE] = OID_SYSCOLUMN_NULLABLE;
        oids[OID_SYSCOLUMN_SCALE] = OID_SYSCOLUMN_SCALE;
        oids[OID_SYSCOLUMN_PRECISION] = OID_SYSCOLUMN_PRECISION;
        oids[OID_SYSCOLUMN_AUTOINC] = OID_SYSCOLUMN_AUTOINC;
        oids[OID_SYSCOLUMN_DISTCOUNT] = OID_SYSCOLUMN_DISTCOUNT;
        oids[OID_SYSCOLUMN_NULLCOUNT] = OID_SYSCOLUMN_NULLCOUNT;
        oids[OID_SYSCOLUMN_MINVALUE] = OID_SYSCOLUMN_MINVALUE;
        oids[DICTOID_SYSCOLUMN_MINVALUE] = DICTOID_SYSCOLUMN_MINVALUE;
        oids[OID_SYSCOLUMN_MAXVALUE] = OID_SYSCOLUMN_MAXVALUE;
        oids[DICTOID_SYSCOLUMN_MAXVALUE] = DICTOID_SYSCOLUMN_MAXVALUE;
        oids[OID_SYSCOLUMN_COMPRESSIONTYPE] = OID_SYSCOLUMN_COMPRESSIONTYPE;
        oids[OID_SYSCOLUMN_NEXTVALUE] = OID_SYSCOLUMN_NEXTVALUE;
    }

    ColumnDef* colDefPtr;
    ddlpackage::TableDef tableDef;
    ddlpackage::ColumnDefList& tableDefCols = tableDef.fColumns;
    std::vector <erydbSystemCatalog::OID> oidList;//oidList.push_back(fStartingColOID + i + 1);

    tableDef.fName = SYSTABLE_TABLE;
    tableDef.fQualifiedName = new QualifiedName(SYSTABLE_TABLE.c_str(), ERYDB_SCHEMA.c_str());
    ColumnDef *colDef;

    //构造 TableDef 
    cout << "Creating SYSTABLE" << endl;
    cout << "---------------------------------------" << endl;
    ////////////////SYSTABLE///////////
    int SYSTABLE_TABLE_col_index=0;
    // TableName
    {
        msg << "  Creating TableName column OID: " << OID_SYSTABLE_TABLENAME << " : " << DICTOID_SYSTABLE_TABLENAME;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSTABLE_TABLE + "." + TABLENAME_COL] = OID_SYSTABLE_TABLENAME;
        (*colMap)[SYSTABLE_TABLE + "." + TABLENAME_COL + ".dic"] = DICTOID_SYSTABLE_TABLENAME;
        oidList.push_back(OID_SYSTABLE_TABLENAME);
        oidList.push_back(DICTOID_SYSTABLE_TABLENAME);
        colDef = new ColumnDef(TABLENAME_COL.c_str(),new ColumnType(ddlpackage::DDL_VARCHAR),NULL,NULL,"元数据表，表名字段");
        colDef->fType->fLength = 65;
        colDef->fType->fCompressiontype = compressionType;
        tableDefCols.push_back(colDef);
        SYSTABLE_TABLE_col_index++;
    }
    // Schema
    {
        msg << "  Creating Schema column OID: " << OID_SYSTABLE_SCHEMA << " : " << DICTOID_SYSTABLE_SCHEMA;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSTABLE_TABLE + "." + SCHEMA_COL] = OID_SYSTABLE_SCHEMA;
        (*colMap)[SYSTABLE_TABLE + "." + SCHEMA_COL + ".dic"] = DICTOID_SYSTABLE_SCHEMA;
        oidList.push_back(OID_SYSTABLE_SCHEMA);
        oidList.push_back(DICTOID_SYSTABLE_SCHEMA);
        colDef = new ColumnDef(SCHEMA_COL.c_str(),new ColumnType(ddlpackage::DDL_VARCHAR),NULL,NULL,NULL);
        colDef->fType->fLength = 65;
        colDef->fType->fCompressiontype = compressionType;
        tableDefCols.push_back(colDef);
        SYSTABLE_TABLE_col_index++;
    }
    // ObjectId
    {
        msg << "  Creating ObjectId column OID: " << OID_SYSTABLE_OBJECTID;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSTABLE_TABLE + "." + OBJECTID_COL] = OID_SYSTABLE_OBJECTID;
        oidList.push_back(OID_SYSTABLE_OBJECTID);
        colDef = new ColumnDef(OBJECTID_COL.c_str(),new ColumnType(ddlpackage::DDL_INT),NULL,NULL,NULL); 
        colDef->fType->fLength = 4;
        colDef->fType->fScale = 0;
        colDef->fType->fCompressiontype = compressionType;
        tableDefCols.push_back(colDef);
        SYSTABLE_TABLE_col_index++;
    }
    // CreateDate
    {
        msg << "  Creating CreateDate column OID: " << OID_SYSTABLE_CREATEDATE;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSTABLE_TABLE + "." + CREATEDATE_COL] = OID_SYSTABLE_CREATEDATE;
        oidList.push_back(OID_SYSTABLE_CREATEDATE);
        colDef = new ColumnDef(CREATEDATE_COL.c_str(),new ColumnType(ddlpackage::DDL_DATE),NULL,NULL,NULL); 
        colDef->fType->fLength = 4;
        colDef->fType->fScale = 0;
        colDef->fType->fCompressiontype = compressionType;
        tableDefCols.push_back(colDef);
        SYSTABLE_TABLE_col_index++;
    }
    // LastUpdateDate
    {
        msg << "  Creating LastUpdate column OID: " << OID_SYSTABLE_LASTUPDATE;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSTABLE_TABLE + "." + LASTUPDATE_COL] = OID_SYSTABLE_LASTUPDATE;
        oidList.push_back(OID_SYSTABLE_LASTUPDATE);
        colDef = new ColumnDef(LASTUPDATE_COL.c_str(),new ColumnType(ddlpackage::DDL_DATE),NULL,NULL,NULL); 
        colDef->fType->fLength = 4;
        colDef->fType->fScale = 0;
        colDef->fType->fCompressiontype = compressionType;
        tableDefCols.push_back(colDef);
        SYSTABLE_TABLE_col_index++;
    }
    // INIT
    {
        msg << "  Creating INIT column OID: " << OID_SYSTABLE_INIT;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSTABLE_TABLE + "." + INIT_COL] = OID_SYSTABLE_INIT;
        oidList.push_back(OID_SYSTABLE_INIT);
        colDef = new ColumnDef(INIT_COL.c_str(),new ColumnType(ddlpackage::DDL_INT),NULL,NULL,NULL); 
        colDef->fType->fLength = 4;
        colDef->fType->fScale = 0;
        colDef->fType->fCompressiontype = compressionType;
        tableDefCols.push_back(colDef);
        SYSTABLE_TABLE_col_index++;
    }
    // NEXT
    {
        msg << "  Creating NEXT column OID: " << OID_SYSTABLE_NEXT;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSTABLE_TABLE + "." + NEXT_COL] = OID_SYSTABLE_NEXT;
        oidList.push_back(OID_SYSTABLE_NEXT);
        colDef = new ColumnDef(NEXT_COL.c_str(),new ColumnType(ddlpackage::DDL_INT),NULL,NULL,NULL); 
        colDef->fType->fLength = 4;
        colDef->fType->fScale = 0;
        colDef->fType->fCompressiontype = compressionType;
        tableDefCols.push_back(colDef);
        SYSTABLE_TABLE_col_index++;
    }
    //NUMOFROWS
    {
        msg << "  Creating NUMOFROWS column OID: " << OID_SYSTABLE_NUMOFROWS;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSTABLE_TABLE + "." + NUMOFROWS_COL] = OID_SYSTABLE_NUMOFROWS;
        oidList.push_back(OID_SYSTABLE_NUMOFROWS);
        colDef = new ColumnDef(NUMOFROWS_COL.c_str(),new ColumnType(ddlpackage::DDL_BIGINT),NULL,NULL,NULL); 
        colDef->fType->fLength = 8;
        colDef->fType->fScale = 0;
        colDef->fType->fCompressiontype = compressionType;
        tableDefCols.push_back(colDef);
        SYSTABLE_TABLE_col_index++;
    }
    //AVGROWLEN
    {
        msg << "  Creating AVGROWLEN column OID: " << OID_SYSTABLE_AVGROWLEN;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSTABLE_TABLE + "." + AVGROWLEN_COL] = OID_SYSTABLE_AVGROWLEN;
        oidList.push_back(OID_SYSTABLE_AVGROWLEN);
        colDef = new ColumnDef(AVGROWLEN_COL.c_str(),new ColumnType(ddlpackage::DDL_INT),NULL,NULL,NULL); 
        colDef->fType->fLength = 4;
        colDef->fType->fScale = 0;
        colDef->fType->fCompressiontype = compressionType;
        SYSTABLE_TABLE_col_index++;
    }
    //NUMOFBLOCKS
    {
        msg << "  Creating NUMOFBLOCKS column OID: " << OID_SYSTABLE_NUMOFBLOCKS;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSTABLE_TABLE + "." + NUMOFBLOCKS_COL] = OID_SYSTABLE_NUMOFBLOCKS;
        oidList.push_back(OID_SYSTABLE_NUMOFBLOCKS);
        colDef = new ColumnDef(NUMOFBLOCKS_COL.c_str(),new ColumnType(ddlpackage::DDL_BIGINT),NULL,NULL,NULL); 
        colDef->fType->fLength = 8;
        colDef->fType->fScale = 0;
        colDef->fType->fCompressiontype = compressionType;
        tableDefCols.push_back(colDef);
        SYSTABLE_TABLE_col_index++;
    }
    //AUTOINCREMENT
    {
        msg << "  Creating AUTOINCREMENT column OID: " << OID_SYSTABLE_AUTOINCREMENT;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSTABLE_TABLE + "." + AUTOINC_COL] = OID_SYSTABLE_AUTOINCREMENT;
        oidList.push_back(OID_SYSTABLE_AUTOINCREMENT);
        colDef = new ColumnDef(AUTOINC_COL.c_str(),new ColumnType(ddlpackage::DDL_BIGINT),NULL,NULL,NULL); 
        colDef->fType->fLength = 8;
        colDef->fType->fScale = 0;
        colDef->fType->fCompressiontype = compressionType;
        SYSTABLE_TABLE_col_index++;
    }
    ////////////////column table///////////
    cout << endl;
    cout << "Creating SYSCOLUMN" << endl;
    // Schema
    {
        msg << "  Creating Schema column OID: " << OID_SYSCOLUMN_SCHEMA << " : " << DICTOID_SYSCOLUMN_SCHEMA;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSCOLUMN_TABLE + "." + SCHEMA_COL] = OID_SYSCOLUMN_SCHEMA;
        (*colMap)[SYSCOLUMN_TABLE + "." + SCHEMA_COL + ".dic"] = DICTOID_SYSCOLUMN_SCHEMA;
        oidList.push_back(OID_SYSCOLUMN_SCHEMA);
        oidList.push_back(DICTOID_SYSCOLUMN_SCHEMA);
        colDef = new ColumnDef(SCHEMA_COL.c_str(),new ColumnType(ddlpackage::DDL_VARCHAR),NULL,NULL,NULL);
        colDef->fType->fLength = 65;
        colDef->fType->fCompressiontype = compressionType;
        tableDefCols.push_back(colDef);
    }
    // TableName
    {
        msg << "  Creating TableName column OID: " << OID_SYSCOLUMN_TABLENAME << " : " << DICTOID_SYSCOLUMN_TABLENAME;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSCOLUMN_TABLE + "." + TABLENAME_COL] = OID_SYSCOLUMN_TABLENAME;
        (*colMap)[SYSCOLUMN_TABLE + "." + TABLENAME_COL + ".dic"] = DICTOID_SYSCOLUMN_TABLENAME;
        oidList.push_back(OID_SYSCOLUMN_TABLENAME);
        oidList.push_back(DICTOID_SYSCOLUMN_TABLENAME);
        colDef = new ColumnDef(TABLENAME_COL.c_str(),new ColumnType(ddlpackage::DDL_VARCHAR),NULL,NULL,NULL);
        colDef->fType->fLength = 65;
        colDef->fType->fCompressiontype = compressionType;
        tableDefCols.push_back(colDef);
    }
    // ColumnName
    {
        msg << "  Creating ColumnName column OID: " << OID_SYSCOLUMN_COLNAME << " : " << DICTOID_SYSCOLUMN_COLNAME;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSCOLUMN_TABLE + "." + COLNAME_COL] = OID_SYSCOLUMN_COLNAME;
        (*colMap)[SYSCOLUMN_TABLE + "." + COLNAME_COL + ".dic"] = DICTOID_SYSCOLUMN_COLNAME;
        oidList.push_back(OID_SYSCOLUMN_COLNAME);
        oidList.push_back(DICTOID_SYSCOLUMN_COLNAME);
        colDef = new ColumnDef(COLNAME_COL.c_str(),new ColumnType(ddlpackage::DDL_VARCHAR),NULL,NULL,NULL);
        colDef->fType->fLength = 65;
        colDef->fType->fCompressiontype = compressionType;
        tableDefCols.push_back(colDef);
    }
    // ObjectId
    {
        msg << "  Creating ObjectId column OID: " << OID_SYSCOLUMN_OBJECTID;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSCOLUMN_TABLE + "." + OBJECTID_COL] = OID_SYSCOLUMN_OBJECTID;
        oidList.push_back(OID_SYSCOLUMN_OBJECTID);
        colDef = new ColumnDef(OBJECTID_COL.c_str(),new ColumnType(ddlpackage::DDL_INT),NULL,NULL,NULL);
        colDef->fType->fLength = 4;
        colDef->fType->fScale = 0;
        colDef->fType->fCompressiontype = compressionType;
        tableDefCols.push_back(colDef);
    }
    // DictOID
    {
        msg << "  Creating DictOID column OID: " << OID_SYSCOLUMN_DICTOID;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSCOLUMN_TABLE + "." + DICTOID_COL] = OID_SYSCOLUMN_DICTOID;
        oidList.push_back(OID_SYSCOLUMN_DICTOID);
        colDef = new ColumnDef(DICTOID_COL.c_str(),new ColumnType(ddlpackage::DDL_INT),NULL,NULL,NULL);
        colDef->fType->fLength = 4;
        colDef->fType->fScale = 0;
        colDef->fType->fCompressiontype = compressionType;
        tableDefCols.push_back(colDef);
    }
    // ListOID
    {
        msg << "  Creating ListOID column OID: " << OID_SYSCOLUMN_LISTOBJID;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSCOLUMN_TABLE + "." + LISTOBJID_COL] = OID_SYSCOLUMN_LISTOBJID;
        oidList.push_back(OID_SYSCOLUMN_LISTOBJID);
        colDef = new ColumnDef(LISTOBJID_COL.c_str(),new ColumnType(ddlpackage::DDL_INT),NULL,NULL,NULL);
        colDef->fType->fLength = 4;
        colDef->fType->fScale = 0;
        colDef->fType->fCompressiontype = compressionType;
        tableDefCols.push_back(colDef);
    }
    // TreeOID
    {
        msg << "  Creating TreeOID column OID: " << OID_SYSCOLUMN_TREEOBJID;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSCOLUMN_TABLE + "." + TREEOBJID_COL] = OID_SYSCOLUMN_TREEOBJID;
        oidList.push_back(OID_SYSCOLUMN_TREEOBJID);
        colDef = new ColumnDef(TREEOBJID_COL.c_str(),new ColumnType(ddlpackage::DDL_INT),NULL,NULL,NULL);
        colDef->fType->fLength = 4;
        colDef->fType->fScale = 0;
        colDef->fType->fCompressiontype = compressionType;
        tableDefCols.push_back(colDef);
    }
    // DataType
    {
        msg << "  Creating DataType column OID: " << OID_SYSCOLUMN_DATATYPE;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSCOLUMN_TABLE + "." + DATATYPE_COL] = OID_SYSCOLUMN_DATATYPE;
        oidList.push_back(OID_SYSCOLUMN_DATATYPE);
        colDef = new ColumnDef(DATATYPE_COL.c_str(),new ColumnType(ddlpackage::DDL_INT),NULL,NULL,NULL);
        colDef->fType->fLength = 4;
        colDef->fType->fScale = 0;
        colDef->fType->fCompressiontype = compressionType;
        tableDefCols.push_back(colDef);
    }
    // ColumnLength
    {
        msg << "  Creating ColumnLength column OID: " << OID_SYSCOLUMN_COLUMNLEN;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSCOLUMN_TABLE + "." + COLUMNLEN_COL] = OID_SYSCOLUMN_COLUMNLEN;
        oidList.push_back(OID_SYSCOLUMN_COLUMNLEN);
        colDef = new ColumnDef(COLUMNLEN_COL.c_str(),new ColumnType(ddlpackage::DDL_INT),NULL,NULL,NULL);
        colDef->fType->fLength = 4;
        colDef->fType->fScale = 0;
        colDef->fType->fCompressiontype = compressionType;
        tableDefCols.push_back(colDef);
    }
    // ColumnPos
    {
        msg << "  Creating ColumnPos column OID: " << OID_SYSCOLUMN_COLUMNPOS;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSCOLUMN_TABLE + "." + COLUMNPOS_COL] = OID_SYSCOLUMN_COLUMNPOS;
        oidList.push_back(OID_SYSCOLUMN_COLUMNPOS);
        colDef = new ColumnDef(COLUMNPOS_COL.c_str(),new ColumnType(ddlpackage::DDL_INT),NULL,NULL,NULL);
        colDef->fType->fLength = 4;
        colDef->fType->fScale = 0;
        colDef->fType->fCompressiontype = compressionType;
        tableDefCols.push_back(colDef);
    }
    // LastUpdate
    {
        msg << "  Creating LastUpdate column OID: " << OID_SYSCOLUMN_LASTUPDATE;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSCOLUMN_TABLE + "." + LASTUPDATE_COL] = OID_SYSCOLUMN_LASTUPDATE;
        oidList.push_back(OID_SYSCOLUMN_LASTUPDATE);
        colDef = new ColumnDef(LASTUPDATE_COL.c_str(),new ColumnType(ddlpackage::DDL_DATE),NULL,NULL,NULL);
        colDef->fType->fLength = 4;
        colDef->fType->fCompressiontype = compressionType;
        tableDefCols.push_back(colDef);
    }
    // DefaultValue
    {
        msg << "  Creating DefaultValue column OID: " << OID_SYSCOLUMN_DEFAULTVAL << " : " << DICTOID_SYSCOLUMN_DEFAULTVAL;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSCOLUMN_TABLE + "." + DEFAULTVAL_COL] = OID_SYSCOLUMN_DEFAULTVAL;
        (*colMap)[SYSCOLUMN_TABLE + "." + DEFAULTVAL_COL + ".dic"] = DICTOID_SYSCOLUMN_DEFAULTVAL;
        oidList.push_back(OID_SYSCOLUMN_DEFAULTVAL);
        oidList.push_back(DICTOID_SYSCOLUMN_DEFAULTVAL);
        colDef = new ColumnDef(DEFAULTVAL_COL.c_str(),new ColumnType(ddlpackage::DDL_VARCHAR),NULL,NULL,NULL);
        colDef->fType->fLength = 64;
        colDef->fType->fCompressiontype = compressionType;
        tableDefCols.push_back(colDef);
    }
    // Nullable
    {
        msg << "  Creating Nullable column OID: " << OID_SYSCOLUMN_NULLABLE;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSCOLUMN_TABLE + "." + NULLABLE_COL] = OID_SYSCOLUMN_NULLABLE;
        oidList.push_back(OID_SYSCOLUMN_NULLABLE);
        colDef = new ColumnDef(NULLABLE_COL.c_str(),new ColumnType(ddlpackage::DDL_INT),NULL,NULL,NULL);
        colDef->fType->fLength = 4;
        colDef->fType->fScale = 0;
        colDef->fType->fCompressiontype = compressionType;
        tableDefCols.push_back(colDef);
    }
    // Scale
    {
        msg << "  Creating Scale column OID: " << OID_SYSCOLUMN_SCALE;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSCOLUMN_TABLE + "." + SCALE_COL] = OID_SYSCOLUMN_SCALE;
        oidList.push_back(OID_SYSCOLUMN_SCALE);
        colDef = new ColumnDef(SCALE_COL.c_str(),new ColumnType(ddlpackage::DDL_INT),NULL,NULL,NULL);
        colDef->fType->fLength = 4;
        colDef->fType->fScale = 0;
        colDef->fType->fCompressiontype = compressionType;
        tableDefCols.push_back(colDef);
    }
    // Precision
    {
        msg << "  Creating Precision column OID: " << OID_SYSCOLUMN_PRECISION;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSCOLUMN_TABLE + "." + PRECISION_COL] = OID_SYSCOLUMN_PRECISION;
        oidList.push_back(OID_SYSCOLUMN_PRECISION);
        colDef = new ColumnDef(PRECISION_COL.c_str(),new ColumnType(ddlpackage::DDL_INT),NULL,NULL,NULL);
        colDef->fType->fLength = 4;
        colDef->fType->fScale = 0;
        colDef->fType->fCompressiontype = compressionType;
        tableDefCols.push_back(colDef);
    }
    // AutoInc
    {
        msg << "  Creating AutoInc column OID: " << OID_SYSCOLUMN_AUTOINC;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSCOLUMN_TABLE + "." + AUTOINC_COL] = OID_SYSCOLUMN_AUTOINC;
        oidList.push_back(OID_SYSCOLUMN_AUTOINC);
        colDef = new ColumnDef(AUTOINC_COL.c_str(),new ColumnType(ddlpackage::DDL_CHAR),NULL,NULL,NULL);
        colDef->fType->fLength = 1;
        colDef->fType->fCompressiontype = compressionType;
        tableDefCols.push_back(colDef);
    }
    // DISTCOUNT
    {
        msg << "  Creating DISTCOUNT column OID: " << OID_SYSCOLUMN_DISTCOUNT;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSCOLUMN_TABLE + "." + DISTCOUNT_COL] = OID_SYSCOLUMN_DISTCOUNT;
        oidList.push_back(OID_SYSCOLUMN_DISTCOUNT);
        colDef = new ColumnDef(DISTCOUNT_COL.c_str(),new ColumnType(ddlpackage::DDL_INT),NULL,NULL,NULL);
        colDef->fType->fLength = 4;
        colDef->fType->fScale = 0;
        colDef->fType->fCompressiontype = compressionType;
        tableDefCols.push_back(colDef);
    }
    // NULLCOUNT
    {
        msg << "  Creating NULLCOUNT column OID: " << OID_SYSCOLUMN_NULLCOUNT;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSCOLUMN_TABLE + "." + NULLCOUNT_COL] = OID_SYSCOLUMN_NULLCOUNT;
        oidList.push_back(OID_SYSCOLUMN_NULLCOUNT);
        colDef = new ColumnDef(NULLCOUNT_COL.c_str(),new ColumnType(ddlpackage::DDL_INT),NULL,NULL,NULL);
        colDef->fType->fLength = 4;
        colDef->fType->fScale = 0;
        colDef->fType->fCompressiontype = compressionType;
        tableDefCols.push_back(colDef);
    }
    // MINVALUE 
    {
        msg << "  Creating MINVALUE column OID: " << OID_SYSCOLUMN_MINVALUE << " : " << DICTOID_SYSCOLUMN_MINVALUE;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSCOLUMN_TABLE + "." + MINVALUE_COL] = OID_SYSCOLUMN_MINVALUE;
        (*colMap)[SYSCOLUMN_TABLE + "." + MINVALUE_COL + ".dic"] = DICTOID_SYSCOLUMN_MINVALUE;
        oidList.push_back(OID_SYSCOLUMN_MINVALUE);
        oidList.push_back(DICTOID_SYSCOLUMN_MINVALUE);
        colDef = new ColumnDef(MINVALUE_COL.c_str(),new ColumnType(ddlpackage::DDL_VARCHAR),NULL,NULL,NULL);
        colDef->fType->fLength = 65;
        colDef->fType->fCompressiontype = compressionType;
        tableDefCols.push_back(colDef);
    }
    // MAXVALUE 
    {
        msg << "  Creating MINVALUE column OID: " << OID_SYSCOLUMN_MAXVALUE << " : " << DICTOID_SYSCOLUMN_MAXVALUE;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSCOLUMN_TABLE + "." + MAXVALUE_COL] = OID_SYSCOLUMN_MAXVALUE;
        (*colMap)[SYSCOLUMN_TABLE + "." + MAXVALUE_COL + ".dic"] = DICTOID_SYSCOLUMN_MAXVALUE;
        oidList.push_back(OID_SYSCOLUMN_MAXVALUE);
        oidList.push_back(DICTOID_SYSCOLUMN_MAXVALUE);
        colDef = new ColumnDef(MAXVALUE_COL.c_str(),new ColumnType(ddlpackage::DDL_VARCHAR),NULL,NULL,NULL);
        colDef->fType->fLength = 65;
        colDef->fType->fCompressiontype = compressionType;
        tableDefCols.push_back(colDef);
    }
    // CompressionType
    {
        msg << "  Creating CompressionType column OID: " << OID_SYSCOLUMN_COMPRESSIONTYPE;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSCOLUMN_TABLE + "." + COMPRESSIONTYPE_COL] = OID_SYSCOLUMN_COMPRESSIONTYPE;
        oidList.push_back(OID_SYSCOLUMN_COMPRESSIONTYPE);
        colDef = new ColumnDef(COMPRESSIONTYPE_COL.c_str(),new ColumnType(ddlpackage::DDL_INT),NULL,NULL,NULL);
        colDef->fType->fLength = 4;
        colDef->fType->fScale = 0;
        colDef->fType->fCompressiontype = compressionType;
        tableDefCols.push_back(colDef);
    }
    // nextvalue
    {
        msg << "  Creating NEXTVALUE column OID: " << OID_SYSCOLUMN_NEXTVALUE;
        cout << msg.str() << endl;msg.str("");
        (*colMap)[SYSCOLUMN_TABLE + "." + NEXTVALUE_COL] = OID_SYSCOLUMN_NEXTVALUE;
        oidList.push_back(OID_SYSCOLUMN_NEXTVALUE);
        colDef = new ColumnDef(NEXTVALUE_COL.c_str(),new ColumnType(ddlpackage::DDL_UNSIGNED_BIGINT),NULL,NULL,NULL);
        colDef->fType->fLength = 8;
        colDef->fType->fScale = 0;
        colDef->fType->fCompressiontype = compressionType;
        tableDefCols.push_back(colDef);
    }
    /////////////////////////////////////////////////////////////////////////////

    ColumnDefList::const_iterator iter = tableDefCols.begin();
    uint32_t numColumns = tableDef.fColumns.size();
    uint32_t numDictCols = 0;
    for (unsigned i = 0; i < numColumns; i++)
    {
        int dataType;
        dataType = DDLPackageProcessor::convertDataType(tableDef.fColumns[i]->fType->fType);
        if ((dataType == erydbSystemCatalog::CHAR && tableDef.fColumns[i]->fType->fLength > 8) ||
            (dataType == erydbSystemCatalog::VARCHAR && tableDef.fColumns[i]->fType->fLength > 7) ||
            (dataType == erydbSystemCatalog::VARBINARY && tableDef.fColumns[i]->fType->fLength > 7))
            numDictCols++;
    }

    bytestream << (ByteStream::byte)WE_SVR_WRITE_CREATETABLEFILES;
    bytestream << uniqueId;
    bytestream << (uint32_t)txnID.id;
    bytestream << (numColumns + numDictCols);
    unsigned colNum = 0;
    unsigned dictNum = 0;
    while (iter != tableDefCols.end())
    {
        colDefPtr = *iter;
        erydbSystemCatalog::ColDataType dataType = DDLPackageProcessor::convertDataType(colDefPtr->fType->fType);
        if (dataType == erydbSystemCatalog::DECIMAL ||
            dataType == erydbSystemCatalog::UDECIMAL)
        {
            if (colDefPtr->fType->fPrecision == -1 || colDefPtr->fType->fPrecision == 0)
            {
                colDefPtr->fType->fLength = 8;
            } else if ((colDefPtr->fType->fPrecision > 0) && (colDefPtr->fType->fPrecision < 3))
            {
                colDefPtr->fType->fLength = 1;
            }

            else if (colDefPtr->fType->fPrecision < 5 && (colDefPtr->fType->fPrecision > 2))
            {
                colDefPtr->fType->fLength = 2;
            } else if (colDefPtr->fType->fPrecision > 4 && colDefPtr->fType->fPrecision < 10)
            {
                colDefPtr->fType->fLength = 4;
            } else if (colDefPtr->fType->fPrecision > 9 && colDefPtr->fType->fPrecision < 19)
            {
                colDefPtr->fType->fLength = 8;
            }
        }
        colNum++;
        uint32_t dataOid=0;
        if(colNum<=SYSTABLE_TABLE_col_index){
            dataOid=(*colMap)[SYSTABLE_TABLE + "." + colDefPtr->fName];// (fStartingColOID + (colNum++) + 1);
        }else{
            dataOid= (*colMap)[SYSCOLUMN_TABLE + "." + colDefPtr->fName];// (fStartingColOID + (colNum++) + 1);
        }
        bytestream << (uint32_t)dataOid;
        bytestream << (uint8_t)dataType;
        bytestream << (uint8_t)false;

        bytestream << (uint32_t)colDefPtr->fType->fLength;
        bytestream << dbRoot;
        bytestream << (uint32_t)colDefPtr->fType->fCompressiontype;
        
        msg << "    "<<colDefPtr->fName <<"     dataOid:"<< dataOid<<" colWidth:"<<colDefPtr->fType->fLength<<endl;
        cout << msg.str() << endl;msg.str("");
        
        
        if ((dataType == erydbSystemCatalog::CHAR && colDefPtr->fType->fLength > 8) ||
            (dataType == erydbSystemCatalog::VARCHAR && colDefPtr->fType->fLength > 7) ||
            (dataType == erydbSystemCatalog::VARBINARY && colDefPtr->fType->fLength > 7))
        {
            dataOid=0;
            if(colNum<=SYSTABLE_TABLE_col_index){
                dataOid= (*colMap)[SYSTABLE_TABLE + "." + colDefPtr->fName+".dic"];// (fStartingColOID + (colNum++) + 1);
            }else{
               dataOid= (*colMap)[SYSCOLUMN_TABLE + "." + colDefPtr->fName+".dic"];// (fStartingColOID + (colNum++) + 1);
            }
            bytestream << (uint32_t)dataOid;
            bytestream << (uint8_t)dataType;
            bytestream << (uint8_t)true;
            bytestream << (uint32_t)colDefPtr->fType->fLength;
            bytestream << dbRoot;
            bytestream << (uint32_t)colDefPtr->fType->fCompressiontype;
            
            msg << "    "<<colDefPtr->fName <<".dic dataOid:"<< dataOid<<" colWidth:"<<colDefPtr->fType->fLength<<endl;
            cout << msg.str() << endl;msg.str("");
        }
        ++iter;
    }
    
    bytestream << numDictCols;
      
    try
    {
#ifdef ERYDB_DDL_DEBUG
        cout << fTxnid.id << " create table sending WE_SVR_WRITE_CREATETABLEFILES to pm " << pmNum << endl;
#endif	
        int weSize = fWEClient.write(bytestream, dbRoot);
        rc = fWEClient.read(uniqueId, weSize, &errorMsg);
#ifdef ERYDB_DDL_DEBUG
        cout << "Create table We_SVR_WRITE_CREATETABLEFILES: " << errorMsg << endl;
#endif 
        if (rc != 0)
        {
            //drop the newly created files
            bytestream.restart();
            bytestream << (ByteStream::byte) WE_SVR_WRITE_DROPFILES;
            bytestream << uniqueId;
            bytestream << (uint32_t)(numColumns + numDictCols);
            for (StringIntMap::element_type::iterator it = colMap->begin(); it != colMap->end(); it++)
            {
                bytestream << it->second;
            }
            int weSize = fWEClient.write(bytestream, dbRoot);
            fWEClient.read(uniqueId, weSize);
            dbrm->deleteOIDs(oidList);
        }
    } catch (runtime_error& e)
    {
        errorMsg = "Lost connection to Write Engine Server";
    }
    if (rc)
    {
        cerr << errorMsg << endl; 
        throw logic_error(errorMsg); 
    }
    //flush data files
    if(oids.size()>0)
        fWriteEngine.flushDataFiles(rc, 1, oids);
    // save brm
    msg.str("  BRMWrapper saving state "); 
    rc = BRMWrapper::getInstance()->saveState();
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));

    timeval endTime;
    gettimeofday(&endTime, 0);
    double elapsedTime =
        (endTime.tv_sec + (endTime.tv_usec / 1000000.0)) -
        (startTime.tv_sec + (startTime.tv_usec / 1000000.0));
    cout << "System Catalog creation took: " << elapsedTime <<
        " seconds to complete." << endl;

    cout << endl;
    cout << "System Catalog created" << endl;
    cout << endl;
}
void SystemCatalog::build() {
    WriteEngine::TxnID txnID = 0;
    int rc;
    //int t= 1000;
    remove();
    cout << "Creating System Catalog..." << endl;
    cout << endl;
    // SYSTABLE
    timeval startTime;
    gettimeofday(&startTime, 0);
    ostringstream msg;
    WErrorCodes ec;
    //------------------------------------------------------------------------------
    // Get the DBRoot count, and rotate the tables through those DBRoots.
    // All the columns in the first table (SYSTABLE) start on DBRoot1, all the
    // columns in the second table (SYSCOLUMN) start on DBRoot2, etc.
    //------------------------------------------------------------------------------
    config::Config* cf = config::Config::makeConfig();
    string root = cf->getConfig("SystemConfig", "DBRootCount");
    uint32_t dbRootCount = cf->uFromText(root);

    //------------------------------------------------------------------------------
    // Create SYSTABLE table
    //------------------------------------------------------------------------------
    DBROOTS_struct dbRoot;
    BRMWrapper::getInstance()->getSysDataDBRoots(&dbRoot);
    //dbRoot[0] = 1;
    cout<<"SysDataDBRoots="<<dbRoot<<endl;

    int compressionType = 0;
    uint32_t partition = 0;
    uint16_t segment = 0;

    ResourceManager rm;
    std::map<uint32_t, uint32_t> oids;
    if (rm.useHdfs())
    {
        compressionType = 2;
        oids[OID_SYSTABLE_TABLENAME] = OID_SYSTABLE_TABLENAME;
        oids[DICTOID_SYSTABLE_TABLENAME] = DICTOID_SYSTABLE_TABLENAME;
        oids[OID_SYSTABLE_SCHEMA] = OID_SYSTABLE_SCHEMA;
        oids[DICTOID_SYSTABLE_SCHEMA] = DICTOID_SYSTABLE_SCHEMA;
        oids[OID_SYSTABLE_OBJECTID] = OID_SYSTABLE_OBJECTID;
        oids[OID_SYSTABLE_CREATEDATE] = OID_SYSTABLE_CREATEDATE;
        oids[OID_SYSTABLE_LASTUPDATE] = OID_SYSTABLE_LASTUPDATE;
        oids[OID_SYSTABLE_INIT] = OID_SYSTABLE_INIT;
        oids[OID_SYSTABLE_NEXT] = OID_SYSTABLE_NEXT;
        oids[OID_SYSTABLE_NUMOFROWS] = OID_SYSTABLE_NUMOFROWS;
        oids[OID_SYSTABLE_AVGROWLEN] = OID_SYSTABLE_AVGROWLEN;
        oids[OID_SYSTABLE_NUMOFBLOCKS] = OID_SYSTABLE_NUMOFBLOCKS;
        oids[OID_SYSTABLE_AUTOINCREMENT] = OID_SYSTABLE_AUTOINCREMENT;
        
    }

    fWriteEngine.setTransId(1);
    fWriteEngine.setBulkFlag(true);
    cout << "Creating SYSTABLE" << endl;
    cout << "---------------------------------------" << endl;
    // TableName
    msg << "  Creating TableName column OID: " << OID_SYSTABLE_TABLENAME;
    cout << msg.str() << endl;
    rc = fWriteEngine.createColumn(txnID, OID_SYSTABLE_TABLENAME, erydbSystemCatalog::VARCHAR, 40, dbRoot, partition, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    msg << "  Creating TableName column dictionary";
    //Dictionary files
    cout << msg.str() << endl;
    rc = fWriteEngine.createDctnry(txnID, DICTOID_SYSTABLE_TABLENAME, 65, dbRoot, partition, segment, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    // Schema
    msg << "  Creating Schema column OID: " << OID_SYSTABLE_SCHEMA;
    cout << msg.str() << endl;
    rc = fWriteEngine.createColumn(txnID, OID_SYSTABLE_SCHEMA, erydbSystemCatalog::VARCHAR, 40, dbRoot, partition, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));


    msg.str("  Creating Schema column dictionary");
    cout << msg.str() << endl;
    //Dictionary files
    rc = fWriteEngine.createDctnry(txnID, DICTOID_SYSTABLE_SCHEMA, 65, dbRoot, partition, segment, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    // ObjectId
    msg << "  Creating ObjectId column OID: " << OID_SYSTABLE_OBJECTID;
    cout << msg.str() << endl;
    rc = fWriteEngine.createColumn(txnID, OID_SYSTABLE_OBJECTID, erydbSystemCatalog::INT, 4, dbRoot, partition, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    // CreateDate
    msg << "  Creating CreateDate column OID: " << OID_SYSTABLE_CREATEDATE;
    cout << msg.str() << endl;
    rc = fWriteEngine.createColumn(txnID, OID_SYSTABLE_CREATEDATE, erydbSystemCatalog::DATE, 4, dbRoot, partition, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    // LastUpdateDate
    msg << "  Creating LastUpdate column OID: " << OID_SYSTABLE_LASTUPDATE;
    cout << msg.str() << endl;
    rc = fWriteEngine.createColumn(txnID, OID_SYSTABLE_LASTUPDATE, erydbSystemCatalog::DATE, 4, dbRoot, partition, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    // INIT
    msg << "  Creating INIT column OID: " << OID_SYSTABLE_INIT;
    cout << msg.str() << endl;
    rc = fWriteEngine.createColumn(txnID, OID_SYSTABLE_INIT, erydbSystemCatalog::INT, 4, dbRoot, partition, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    // NEXT
    msg << "  Creating NEXT column OID: " << OID_SYSTABLE_NEXT;
    cout << msg.str() << endl;
    rc = fWriteEngine.createColumn(txnID, OID_SYSTABLE_NEXT, erydbSystemCatalog::INT, 4, dbRoot, partition, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    //NUMOFROWS
    msg << "  Creating NUMOFROWS column OID: " << OID_SYSTABLE_NUMOFROWS;
    cout << msg.str() << endl;
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    //AVGROWLEN
    msg << "  Creating AVGROWLEN column OID: " << OID_SYSTABLE_AVGROWLEN;
    cout << msg.str() << endl;
    rc = fWriteEngine.createColumn(txnID, OID_SYSTABLE_AVGROWLEN, erydbSystemCatalog::INT, 4, dbRoot, partition, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    //NUMOFBLOCKS
    msg << "  Creating NUMOFBLOCKS column OID: " << OID_SYSTABLE_NUMOFBLOCKS;
    cout << msg.str() << endl;
    rc = fWriteEngine.createColumn(txnID, OID_SYSTABLE_NUMOFBLOCKS, erydbSystemCatalog::INT, 4, dbRoot, partition, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    //AUTOINCREMENT
    msg << "  Creating AUTOINCREMENT column OID: " << OID_SYSTABLE_AUTOINCREMENT;
    cout << msg.str() << endl;
    rc = fWriteEngine.createColumn(txnID, OID_SYSTABLE_AUTOINCREMENT, erydbSystemCatalog::INT, 4, dbRoot, partition, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    //------------------------------------------------------------------------------
    // Create SYSCOLUMN table
    //------------------------------------------------------------------------------
    //dbRoot++;
    //if (dbRoot > dbRootCount)
    //  dbRoot = 1;

    //SYSCOLUMN
    if (rm.useHdfs()) {
        oids[OID_SYSCOLUMN_SCHEMA] = OID_SYSCOLUMN_SCHEMA;
        oids[DICTOID_SYSCOLUMN_SCHEMA] = DICTOID_SYSCOLUMN_SCHEMA;
        oids[OID_SYSCOLUMN_TABLENAME] = OID_SYSCOLUMN_TABLENAME;
        oids[DICTOID_SYSCOLUMN_TABLENAME] = DICTOID_SYSCOLUMN_TABLENAME;
        oids[OID_SYSCOLUMN_COLNAME] = OID_SYSCOLUMN_COLNAME;
        oids[DICTOID_SYSCOLUMN_COLNAME] = DICTOID_SYSCOLUMN_COLNAME;
        oids[OID_SYSCOLUMN_OBJECTID] = OID_SYSCOLUMN_OBJECTID;
        oids[OID_SYSCOLUMN_DICTOID] = OID_SYSCOLUMN_DICTOID;
        oids[OID_SYSCOLUMN_LISTOBJID] = OID_SYSCOLUMN_LISTOBJID;
        oids[OID_SYSCOLUMN_TREEOBJID] = OID_SYSCOLUMN_TREEOBJID;
        oids[OID_SYSCOLUMN_DATATYPE] = OID_SYSCOLUMN_DATATYPE;
        oids[OID_SYSCOLUMN_COLUMNLEN] = OID_SYSCOLUMN_COLUMNLEN;
        oids[OID_SYSCOLUMN_COLUMNPOS] = OID_SYSCOLUMN_COLUMNPOS;
        oids[OID_SYSCOLUMN_LASTUPDATE] = OID_SYSCOLUMN_LASTUPDATE;
        oids[OID_SYSCOLUMN_DEFAULTVAL] = OID_SYSCOLUMN_DEFAULTVAL;
        oids[DICTOID_SYSCOLUMN_DEFAULTVAL] = DICTOID_SYSCOLUMN_DEFAULTVAL;
        oids[OID_SYSCOLUMN_NULLABLE] = OID_SYSCOLUMN_NULLABLE;
        oids[OID_SYSCOLUMN_SCALE] = OID_SYSCOLUMN_SCALE;
        oids[OID_SYSCOLUMN_PRECISION] = OID_SYSCOLUMN_PRECISION;
        oids[OID_SYSCOLUMN_AUTOINC] = OID_SYSCOLUMN_AUTOINC;
        oids[OID_SYSCOLUMN_DISTCOUNT] = OID_SYSCOLUMN_DISTCOUNT;
        oids[OID_SYSCOLUMN_NULLCOUNT] = OID_SYSCOLUMN_NULLCOUNT;
        oids[OID_SYSCOLUMN_MINVALUE] = OID_SYSCOLUMN_MINVALUE;
        oids[DICTOID_SYSCOLUMN_MINVALUE] = DICTOID_SYSCOLUMN_MINVALUE;
        oids[OID_SYSCOLUMN_MAXVALUE] = OID_SYSCOLUMN_MAXVALUE;
        oids[DICTOID_SYSCOLUMN_MAXVALUE] = DICTOID_SYSCOLUMN_MAXVALUE;
        oids[OID_SYSCOLUMN_COMPRESSIONTYPE] = OID_SYSCOLUMN_COMPRESSIONTYPE;
        oids[OID_SYSCOLUMN_NEXTVALUE] = OID_SYSCOLUMN_NEXTVALUE;
    }
    cout << endl;
    cout << "Creating SYSCOLUMN" << endl;
    // Schema
    cout << "---------------------------------------" << endl;
    msg << "  Creating Schema column OID: " << OID_SYSCOLUMN_SCHEMA;
    cout << msg.str() << endl;
    rc = fWriteEngine.createColumn(txnID, OID_SYSCOLUMN_SCHEMA, erydbSystemCatalog::VARCHAR, 40, dbRoot, partition, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));

    msg.str("  Creating Schema column dictionary...");
    //Dictionary files
    cout << msg.str() << endl;
    rc = fWriteEngine.createDctnry(txnID, DICTOID_SYSCOLUMN_SCHEMA, 65, dbRoot, partition, segment, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    // TableName
    msg << "  Creating TableName column OID: " << OID_SYSCOLUMN_TABLENAME;
    cout << msg.str() << endl;
    rc = fWriteEngine.createColumn(txnID, OID_SYSCOLUMN_TABLENAME, erydbSystemCatalog::VARCHAR, 40, dbRoot, partition, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));

    msg.str("  Creating TableName column dictionary...");
    //Dictionary files
    cout << msg.str() << endl;
    rc = fWriteEngine.createDctnry(txnID, DICTOID_SYSCOLUMN_TABLENAME, 65, dbRoot, partition, segment, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    // ColumnName
    msg << "  Creating ColumnName column OID: " << OID_SYSCOLUMN_COLNAME;
    cout << msg.str() << endl;
    rc = fWriteEngine.createColumn(txnID, OID_SYSCOLUMN_COLNAME, erydbSystemCatalog::VARCHAR, 40, dbRoot, partition, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));

    msg.str("  Creating ColumnName column dictionary...");
    //Dictionary files
    cout << msg.str() << endl;
    rc = fWriteEngine.createDctnry(txnID, DICTOID_SYSCOLUMN_COLNAME, 65, dbRoot, partition, segment, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    // ObjectID
    msg << "  Creating ObjectID column OID: " << OID_SYSCOLUMN_OBJECTID;
    cout << msg.str() << endl;
    rc = fWriteEngine.createColumn(txnID, OID_SYSCOLUMN_OBJECTID, erydbSystemCatalog::INT, 4, dbRoot, partition, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    // DictOID
    msg << "  Creating DictOID column OID: " << OID_SYSCOLUMN_DICTOID;
    cout << msg.str() << endl;
    rc = fWriteEngine.createColumn(txnID, OID_SYSCOLUMN_DICTOID, erydbSystemCatalog::INT, 4, dbRoot, partition, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    // ListOID
    msg << "  Creating ListOID column OID: " << OID_SYSCOLUMN_LISTOBJID;
    cout << msg.str() << endl;
    rc = fWriteEngine.createColumn(txnID, OID_SYSCOLUMN_LISTOBJID, erydbSystemCatalog::INT, 4, dbRoot, partition, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    // TreeOID
    msg << "  Creating TreeOID column OID: " << OID_SYSCOLUMN_TREEOBJID;
    cout << msg.str() << endl;
    rc = fWriteEngine.createColumn(txnID, OID_SYSCOLUMN_TREEOBJID, erydbSystemCatalog::INT, 4, dbRoot, partition, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    // DataType
    msg << "  Creating DataType column OID: " << OID_SYSCOLUMN_DATATYPE;
    cout << msg.str() << endl;
    rc = fWriteEngine.createColumn(txnID, OID_SYSCOLUMN_DATATYPE, erydbSystemCatalog::INT, 4, dbRoot, partition, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    // ColumnLength
    msg << "  Creating ColumnLength column OID: " << OID_SYSCOLUMN_COLUMNLEN;
    cout << msg.str() << endl;
    rc = fWriteEngine.createColumn(txnID, OID_SYSCOLUMN_COLUMNLEN, erydbSystemCatalog::INT, 4, dbRoot, partition, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    // ColumnPos
    msg << "  Creating ColumnPos column OID: " << OID_SYSCOLUMN_COLUMNPOS;
    cout << msg.str() << endl;
    rc = fWriteEngine.createColumn(txnID, OID_SYSCOLUMN_COLUMNPOS, erydbSystemCatalog::INT, 4, dbRoot, partition, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    // LastUpdate
    msg << "  Creating LastUpdate column OID: " << OID_SYSCOLUMN_LASTUPDATE;
    cout << msg.str() << endl;
    rc = fWriteEngine.createColumn(txnID, OID_SYSCOLUMN_LASTUPDATE, erydbSystemCatalog::DATE, 4, dbRoot, partition, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    // DefaultValue
    msg << "  Creating DefaultValue column OID: " << OID_SYSCOLUMN_DEFAULTVAL;
    cout << msg.str() << endl;
    rc = fWriteEngine.createColumn(txnID, OID_SYSCOLUMN_DEFAULTVAL, erydbSystemCatalog::VARCHAR, 8, dbRoot, partition, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    msg.str("  Creating DefaultValue column dictionary...");
    //Dictionary files
    cout << msg.str() << endl;
    rc = fWriteEngine.createDctnry(txnID, DICTOID_SYSCOLUMN_DEFAULTVAL, 9, dbRoot, partition, segment, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    // Nullable
    msg << "  Creating Nullable column OID: " << OID_SYSCOLUMN_NULLABLE;
    cout << msg.str() << endl;
    rc = fWriteEngine.createColumn(txnID, OID_SYSCOLUMN_NULLABLE, erydbSystemCatalog::INT, 4, dbRoot, partition, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    // Scale
    msg << "  Creating Scale column OID: " << OID_SYSCOLUMN_SCALE;
    cout << msg.str() << endl;
    rc = fWriteEngine.createColumn(txnID, OID_SYSCOLUMN_SCALE, erydbSystemCatalog::INT, 4, dbRoot, partition, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    // Precision
    msg << "  Creating Precision column OID: " << OID_SYSCOLUMN_PRECISION;
    cout << msg.str() << endl;
    rc = fWriteEngine.createColumn(txnID, OID_SYSCOLUMN_PRECISION, erydbSystemCatalog::INT, 4, dbRoot, partition, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    // AutoInc
    msg << "  Creating AutoInc column OID: " << OID_SYSCOLUMN_AUTOINC;
    cout << msg.str() << endl;
    rc = fWriteEngine.createColumn(txnID, OID_SYSCOLUMN_AUTOINC, erydbSystemCatalog::CHAR, 1, dbRoot, partition, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    // DISTCOUNT
    msg << "  Creating DISTCOUNT column OID: " << OID_SYSCOLUMN_DISTCOUNT;
    cout << msg.str() << endl;
    rc = fWriteEngine.createColumn(txnID, OID_SYSCOLUMN_DISTCOUNT, erydbSystemCatalog::INT, 4, dbRoot, partition, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    // NULLCOUNT
    msg << "  Creating NULLCOUNT column OID: " << OID_SYSCOLUMN_NULLCOUNT;
    cout << msg.str() << endl;
    rc = fWriteEngine.createColumn(txnID, OID_SYSCOLUMN_NULLCOUNT, erydbSystemCatalog::INT, 4, dbRoot, partition, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    // MINVALUE
    msg << "  Creating MINVALUE column OID: " << OID_SYSCOLUMN_MINVALUE;
    cout << msg.str() << endl;
    rc = fWriteEngine.createColumn(txnID, OID_SYSCOLUMN_MINVALUE, erydbSystemCatalog::VARCHAR, 40, dbRoot, partition, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));

    msg.str("  Creating MINVALUE column dictionary...");
    cout << msg.str() << endl;
    //Dictionary files
    rc = fWriteEngine.createDctnry(txnID, DICTOID_SYSCOLUMN_MINVALUE, 65, dbRoot, partition, segment, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    // MAXVALUE
    msg << "  Creating MAXVALUE column OID: " << OID_SYSCOLUMN_MAXVALUE;
    cout << msg.str() << endl;
    rc = fWriteEngine.createColumn(txnID, OID_SYSCOLUMN_MAXVALUE, erydbSystemCatalog::VARCHAR, 40, dbRoot, partition, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));

    msg.str("  Creating MAXVALUE column dictionary...");
    //Dictionary files
    cout << msg.str() << endl;
    rc = fWriteEngine.createDctnry(txnID, DICTOID_SYSCOLUMN_MAXVALUE, 65, dbRoot, partition, segment, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));

    // CompressionType
    msg << "  Creating CompressionType column OID: " << OID_SYSCOLUMN_COMPRESSIONTYPE;
    cout << msg.str() << endl;
    rc = fWriteEngine.createColumn(txnID, OID_SYSCOLUMN_COMPRESSIONTYPE, erydbSystemCatalog::INT, 4, dbRoot, partition, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    // nextvalue
    msg << "  Creating NEXTVALUE column OID: " << OID_SYSCOLUMN_NEXTVALUE;
    cout << msg.str() << endl;
    rc = fWriteEngine.createColumn(txnID, OID_SYSCOLUMN_NEXTVALUE, erydbSystemCatalog::UBIGINT, 8, dbRoot, partition, compressionType);
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));
    msg.str("");

    //------------------------------------------------------------------------------
    // Create SYSCONSTRAINT table
    //------------------------------------------------------------------------------
    
    //dbRoot++;
    //if (dbRoot > dbRootCount)
    //    dbRoot = 1;

    //flush data files
    fWriteEngine.flushDataFiles(rc, 1, oids);
    // save brm
    msg.str("  BRMWrapper saving state ");


    rc = BRMWrapper::getInstance()->saveState();
    if (rc) throw runtime_error(msg.str() + ec.errorString(rc));

    timeval endTime;
    gettimeofday(&endTime, 0);
    double elapsedTime =
        (endTime.tv_sec + (endTime.tv_usec / 1000000.0)) -
        (startTime.tv_sec + (startTime.tv_usec / 1000000.0));
    cout << "System Catalog creation took: " << elapsedTime <<
        " seconds to complete." << endl;

    cout << endl;
    cout << "System Catalog created" << endl;
    cout << endl;
}


void SystemCatalog::remove() {
    ColumnOpCompress0 colOp;

    for (int c = OID_SYSTABLE_TABLENAME; c <= OID_SYSTABLE_TABLENAME + 1000; c++)
        colOp.deleteFile(c);

}
