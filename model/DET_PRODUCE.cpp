#include "DET_PRODUCE.h"
using namespace TriModel;
DET_PRODUCE::DET_PRODUCE(QObject *parent) : kcdz::SqlObject(parent)
{
   this->setObjectName("ID");
}

DET_PRODUCE::DET_PRODUCE(const DET_PRODUCE &det_produce):kcdz::SqlObject(det_produce.parent())
{
    this->setObjectName(det_produce.objectName());
   
 this->m_id=det_produce.id();
 this->m_taskid=det_produce.taskid();
 this->m_codeprefix=det_produce.codeprefix();
 this->m_outcode=det_produce.outcode();
 this->m_barcode=det_produce.barcode();
 this->m_incode=det_produce.incode();
 this->m_pwd=det_produce.pwd();
 this->m_channel=det_produce.channel();
 this->m_det_type=det_produce.det_type();
 this->m_status_code=det_produce.status_code();
 this->m_production_line_id=det_produce.production_line_id();
 this->m_line_length=det_produce.line_length();
 this->m_factory=det_produce.factory();
 this->m_feature=det_produce.feature();
 this->m_no=det_produce.no();
 this->m_det_date=det_produce.det_date();
 this->m_produce_time=det_produce.produce_time();
}
void DET_PRODUCE::operator=(DET_PRODUCE det_produce)
{
    this->setObjectName(det_produce.objectName());
    
 this->m_id=det_produce.id();
 this->m_taskid=det_produce.taskid();
 this->m_codeprefix=det_produce.codeprefix();
 this->m_outcode=det_produce.outcode();
 this->m_barcode=det_produce.barcode();
 this->m_incode=det_produce.incode();
 this->m_pwd=det_produce.pwd();
 this->m_channel=det_produce.channel();
 this->m_det_type=det_produce.det_type();
 this->m_status_code=det_produce.status_code();
 this->m_production_line_id=det_produce.production_line_id();
 this->m_line_length=det_produce.line_length();
 this->m_factory=det_produce.factory();
 this->m_feature=det_produce.feature();
 this->m_no=det_produce.no();
 this->m_det_date=det_produce.det_date();
 this->m_produce_time=det_produce.produce_time();
}

bool DET_PRODUCE::operator==(DET_PRODUCE &other)
{
    
 if(this->m_id!=other.id()) return false;
 if(this->m_taskid!=other.taskid()) return false;
 if(this->m_codeprefix!=other.codeprefix()) return false;
 if(this->m_outcode!=other.outcode()) return false;
 if(this->m_barcode!=other.barcode()) return false;
 if(this->m_incode!=other.incode()) return false;
 if(this->m_pwd!=other.pwd()) return false;
 if(this->m_channel!=other.channel()) return false;
 if(this->m_det_type!=other.det_type()) return false;
 if(this->m_status_code!=other.status_code()) return false;
 if(this->m_production_line_id!=other.production_line_id()) return false;
 if(this->m_line_length!=other.line_length()) return false;
 if(this->m_factory!=other.factory()) return false;
 if(this->m_feature!=other.feature()) return false;
 if(this->m_no!=other.no()) return false;
 if(this->m_det_date!=other.det_date()) return false;
 if(this->m_produce_time!=other.produce_time()) return false;
     return  true;
}
bool DET_PRODUCE::operator !=(DET_PRODUCE &other)
{
    return !(*this==other);
}

qint32 DET_PRODUCE::id() const 
{
	 return m_id;
}

void DET_PRODUCE::setID(const qint32 &id)
{
	 m_id=id;
}

QString DET_PRODUCE::taskid() const 
{
	 return m_taskid;
}

void DET_PRODUCE::setTASKID(const QString &taskid)
{
	 m_taskid=taskid;
}

QString DET_PRODUCE::codeprefix() const 
{
	 return m_codeprefix;
}

void DET_PRODUCE::setCODEPREFIX(const QString &codeprefix)
{
	 m_codeprefix=codeprefix;
}

QString DET_PRODUCE::outcode() const 
{
	 return m_outcode;
}

void DET_PRODUCE::setOUTCODE(const QString &outcode)
{
	 m_outcode=outcode;
}

QString DET_PRODUCE::barcode() const 
{
	 return m_barcode;
}

void DET_PRODUCE::setBARCODE(const QString &barcode)
{
	 m_barcode=barcode;
}

QString DET_PRODUCE::incode() const 
{
	 return m_incode;
}

void DET_PRODUCE::setINCODE(const QString &incode)
{
	 m_incode=incode;
}

QString DET_PRODUCE::pwd() const 
{
	 return m_pwd;
}

void DET_PRODUCE::setPWD(const QString &pwd)
{
	 m_pwd=pwd;
}

qint32 DET_PRODUCE::channel() const 
{
	 return m_channel;
}

void DET_PRODUCE::setCHANNEL(const qint32 &channel)
{
	 m_channel=channel;
}

qint32 DET_PRODUCE::det_type() const 
{
	 return m_det_type;
}

void DET_PRODUCE::setDET_TYPE(const qint32 &det_type)
{
	 m_det_type=det_type;
}

qint32 DET_PRODUCE::status_code() const 
{
	 return m_status_code;
}

void DET_PRODUCE::setSTATUS_CODE(const qint32 &status_code)
{
	 m_status_code=status_code;
}

qint32 DET_PRODUCE::production_line_id() const 
{
	 return m_production_line_id;
}

void DET_PRODUCE::setPRODUCTION_LINE_ID(const qint32 &production_line_id)
{
	 m_production_line_id=production_line_id;
}

qint32 DET_PRODUCE::line_length() const 
{
	 return m_line_length;
}

void DET_PRODUCE::setLINE_LENGTH(const qint32 &line_length)
{
	 m_line_length=line_length;
}

qint32 DET_PRODUCE::factory() const 
{
	 return m_factory;
}

void DET_PRODUCE::setFACTORY(const qint32 &factory)
{
	 m_factory=factory;
}

QString DET_PRODUCE::feature() const 
{
	 return m_feature;
}

void DET_PRODUCE::setFEATURE(const QString &feature)
{
	 m_feature=feature;
}

qint32 DET_PRODUCE::no() const 
{
	 return m_no;
}

void DET_PRODUCE::setNO(const qint32 &no)
{
	 m_no=no;
}

QString DET_PRODUCE::det_date() const 
{
	 return m_det_date;
}

void DET_PRODUCE::setDET_DATE(const QString &det_date)
{
	 m_det_date=det_date;
}

QString DET_PRODUCE::produce_time() const 
{
	 return m_produce_time;
}

void DET_PRODUCE::setPRODUCE_TIME(const QString &produce_time)
{
     m_produce_time=produce_time;
}

void DET_PRODUCE::initSql(const QString& dbName)
{
     setPrimaryKeys(QStringList()<<"ID");
     setDatabaseName(dbName);
     setDriver(kcdz::SqlObject::SQLite);
     setTable("DET_PRODUCE");
     initialize();
}

