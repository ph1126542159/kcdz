#include "productDetData.h"
using namespace TriModel;
productDetData::productDetData(QObject *parent) : QObject(parent)
{
   this->setObjectName("id");
}

productDetData::productDetData(const productDetData &productdetdata):QObject(productdetdata.parent())
{
    this->setObjectName(productdetdata.objectName());
   
 this->m_id=productdetdata.id();
 this->m_taskid=productdetdata.taskid();
 this->m_codeprefix=productdetdata.codeprefix();
 this->m_outcode=productdetdata.outcode();
 this->m_encipherrange=productdetdata.encipherrange();
 this->m_produce_time=productdetdata.produce_time();
 this->m_incode=productdetdata.incode();
 this->m_pwd=productdetdata.pwd();
 this->m_status_code=productdetdata.status_code();
 this->m_assignmentnum=productdetdata.assignmentnum();
 this->m_accomplishnum=productdetdata.accomplishnum();
 this->m_abnormalnum=productdetdata.abnormalnum();
 this->m_lotnum=productdetdata.lotnum();
 this->m_staff=productdetdata.staff();
 this->m_producecomment=productdetdata.producecomment();
 this->m_barcode=productdetdata.barcode();
 this->m_det_type=productdetdata.det_type();
 this->m_production_line_id=productdetdata.production_line_id();
 this->m_line_length=productdetdata.line_length();
 this->m_factory=productdetdata.factory();
 this->m_feature=productdetdata.feature();
 this->m_no=productdetdata.no();
 this->m_channel=productdetdata.channel();
 this->m_det_date=productdetdata.det_date();
 this->m_numberinbox=productdetdata.numberinbox();
 this->m_startbox=productdetdata.startbox();
 this->m_startno=productdetdata.startno();
}
void productDetData::operator=(productDetData productdetdata)
{
    this->setObjectName(productdetdata.objectName());
    
 this->m_id=productdetdata.id();
 this->m_taskid=productdetdata.taskid();
 this->m_codeprefix=productdetdata.codeprefix();
 this->m_outcode=productdetdata.outcode();
 this->m_encipherrange=productdetdata.encipherrange();
 this->m_produce_time=productdetdata.produce_time();
 this->m_incode=productdetdata.incode();
 this->m_pwd=productdetdata.pwd();
 this->m_status_code=productdetdata.status_code();
 this->m_assignmentnum=productdetdata.assignmentnum();
 this->m_accomplishnum=productdetdata.accomplishnum();
 this->m_abnormalnum=productdetdata.abnormalnum();
 this->m_lotnum=productdetdata.lotnum();
 this->m_staff=productdetdata.staff();
 this->m_producecomment=productdetdata.producecomment();
 this->m_barcode=productdetdata.barcode();
 this->m_det_type=productdetdata.det_type();
 this->m_production_line_id=productdetdata.production_line_id();
 this->m_line_length=productdetdata.line_length();
 this->m_factory=productdetdata.factory();
 this->m_feature=productdetdata.feature();
 this->m_no=productdetdata.no();
 this->m_channel=productdetdata.channel();
 this->m_det_date=productdetdata.det_date();
 this->m_numberinbox=productdetdata.numberinbox();
 this->m_startbox=productdetdata.startbox();
 this->m_startno=productdetdata.startno();
}

bool productDetData::operator==(productDetData &other)
{
    
 if(this->m_id!=other.id()) return false;
 if(this->m_taskid!=other.taskid()) return false;
 if(this->m_codeprefix!=other.codeprefix()) return false;
 if(this->m_outcode!=other.outcode()) return false;
 if(this->m_encipherrange!=other.encipherrange()) return false;
 if(this->m_produce_time!=other.produce_time()) return false;
 if(this->m_incode!=other.incode()) return false;
 if(this->m_pwd!=other.pwd()) return false;
 if(this->m_status_code!=other.status_code()) return false;
 if(this->m_assignmentnum!=other.assignmentnum()) return false;
 if(this->m_accomplishnum!=other.accomplishnum()) return false;
 if(this->m_abnormalnum!=other.abnormalnum()) return false;
 if(this->m_lotnum!=other.lotnum()) return false;
 if(this->m_staff!=other.staff()) return false;
 if(this->m_producecomment!=other.producecomment()) return false;
 if(this->m_barcode!=other.barcode()) return false;
 if(this->m_det_type!=other.det_type()) return false;
 if(this->m_production_line_id!=other.production_line_id()) return false;
 if(this->m_line_length!=other.line_length()) return false;
 if(this->m_factory!=other.factory()) return false;
 if(this->m_feature!=other.feature()) return false;
 if(this->m_no!=other.no()) return false;
 if(this->m_channel!=other.channel()) return false;
 if(this->m_det_date!=other.det_date()) return false;
 if(this->m_numberinbox!=other.numberinbox()) return false;
 if(this->m_startbox!=other.startbox()) return false;
 if(this->m_startno!=other.startno()) return false;
     return  true;
}
bool productDetData::operator !=(productDetData &other)
{
    return !(*this==other);
}

qint32 productDetData::id() const 
{
	 return m_id;
}

void productDetData::setid(const qint32 &id)
{
	 m_id=id;
}

QString productDetData::taskid() const 
{
	 return m_taskid;
}

void productDetData::setTASKID(const QString &taskid)
{
	 m_taskid=taskid;
}

QString productDetData::codeprefix() const 
{
	 return m_codeprefix;
}

void productDetData::setCODEPREFIX(const QString &codeprefix)
{
	 m_codeprefix=codeprefix;
}

QString productDetData::outcode() const 
{
	 return m_outcode;
}

void productDetData::setOUTCODE(const QString &outcode)
{
	 m_outcode=outcode;
}

QString productDetData::encipherrange() const 
{
	 return m_encipherrange;
}

void productDetData::setencipherRange(const QString &encipherrange)
{
	 m_encipherrange=encipherrange;
}

QString productDetData::produce_time() const 
{
	 return m_produce_time;
}

void productDetData::setPRODUCE_TIME(const QString &produce_time)
{
	 m_produce_time=produce_time;
}

QString productDetData::incode() const 
{
	 return m_incode;
}

void productDetData::setINCODE(const QString &incode)
{
	 m_incode=incode;
}

QString productDetData::pwd() const 
{
	 return m_pwd;
}

void productDetData::setPWD(const QString &pwd)
{
	 m_pwd=pwd;
}

QString productDetData::status_code() const 
{
	 return m_status_code;
}

void productDetData::setSTATUS_CODE(const QString &status_code)
{
	 m_status_code=status_code;
}

qint32 productDetData::assignmentnum() const 
{
	 return m_assignmentnum;
}

void productDetData::setassignmentNum(const qint32 &assignmentnum)
{
	 m_assignmentnum=assignmentnum;
}

qint32 productDetData::accomplishnum() const 
{
	 return m_accomplishnum;
}

void productDetData::setaccomplishNum(const qint32 &accomplishnum)
{
	 m_accomplishnum=accomplishnum;
}

QString productDetData::abnormalnum() const 
{
	 return m_abnormalnum;
}

void productDetData::setabnormalNum(const QString &abnormalnum)
{
	 m_abnormalnum=abnormalnum;
}

QString productDetData::lotnum() const 
{
	 return m_lotnum;
}

void productDetData::setlotNum(const QString &lotnum)
{
	 m_lotnum=lotnum;
}

QString productDetData::staff() const 
{
	 return m_staff;
}

void productDetData::setstaff(const QString &staff)
{
	 m_staff=staff;
}

QString productDetData::producecomment() const 
{
	 return m_producecomment;
}

void productDetData::setproduceComment(const QString &producecomment)
{
	 m_producecomment=producecomment;
}

QString productDetData::barcode() const 
{
	 return m_barcode;
}

void productDetData::setBARCODE(const QString &barcode)
{
	 m_barcode=barcode;
}

QString productDetData::det_type() const 
{
	 return m_det_type;
}

void productDetData::setDET_TYPE(const QString &det_type)
{
	 m_det_type=det_type;
}

QString productDetData::production_line_id() const 
{
	 return m_production_line_id;
}

void productDetData::setPRODUCTION_LINE_ID(const QString &production_line_id)
{
	 m_production_line_id=production_line_id;
}

QString productDetData::line_length() const 
{
	 return m_line_length;
}

void productDetData::setLINE_LENGTH(const QString &line_length)
{
	 m_line_length=line_length;
}

QString productDetData::factory() const 
{
	 return m_factory;
}

void productDetData::setFACTORY(const QString &factory)
{
	 m_factory=factory;
}

QString productDetData::feature() const 
{
	 return m_feature;
}

void productDetData::setFEATURE(const QString &feature)
{
	 m_feature=feature;
}

QString productDetData::no() const 
{
	 return m_no;
}

void productDetData::setNO(const QString &no)
{
	 m_no=no;
}

QString productDetData::channel() const 
{
	 return m_channel;
}

void productDetData::setCHANNEL(const QString &channel)
{
	 m_channel=channel;
}

QString productDetData::det_date() const 
{
	 return m_det_date;
}

void productDetData::setDET_DATE(const QString &det_date)
{
	 m_det_date=det_date;
}

QString productDetData::numberinbox() const 
{
	 return m_numberinbox;
}

void productDetData::setnumberInBox(const QString &numberinbox)
{
	 m_numberinbox=numberinbox;
}

QString productDetData::startbox() const 
{
	 return m_startbox;
}

void productDetData::setstartBox(const QString &startbox)
{
	 m_startbox=startbox;
}

QString productDetData::startno() const 
{
	 return m_startno;
}

void productDetData::setstartNo(const QString &startno)
{
	 m_startno=startno;
}

