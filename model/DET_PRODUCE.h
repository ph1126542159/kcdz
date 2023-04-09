#ifndef DET_PRODUCE_H
#define DET_PRODUCE_H
/**
*┌──────────────────────────────────────────────────────────────┐
*│　描    述：
*│　作    者：hui.peng
*│　版    本：1.0    模板代码自动生成
*│　创建时间：2023-04-08 10:47:51
*└──────────────────────────────────────────────────────────────┘
*┌──────────────────────────────────────────────────────────────┐
*│　类    名： DET_PRODUCE
*└──────────────────────────────────────────────────────────────┘
*/
#include <QObject>
#include "Gui/SqlObject.h"
namespace TriModel{
/**
 * @brief 
 */
class  DET_PRODUCE : public kcdz::SqlObject
{
    Q_OBJECT
     
	Q_PROPERTY(qint32 ID READ id WRITE setID) 
	Q_PROPERTY(QString TASKID READ taskid WRITE setTASKID) 
	Q_PROPERTY(QString CODEPREFIX READ codeprefix WRITE setCODEPREFIX) 
	Q_PROPERTY(QString OUTCODE READ outcode WRITE setOUTCODE) 
	Q_PROPERTY(QString BARCODE READ barcode WRITE setBARCODE) 
	Q_PROPERTY(QString INCODE READ incode WRITE setINCODE) 
	Q_PROPERTY(QString PWD READ pwd WRITE setPWD) 
	Q_PROPERTY(qint32 CHANNEL READ channel WRITE setCHANNEL) 
	Q_PROPERTY(qint32 DET_TYPE READ det_type WRITE setDET_TYPE) 
	Q_PROPERTY(qint32 STATUS_CODE READ status_code WRITE setSTATUS_CODE) 
	Q_PROPERTY(qint32 PRODUCTION_LINE_ID READ production_line_id WRITE setPRODUCTION_LINE_ID) 
	Q_PROPERTY(qint32 LINE_LENGTH READ line_length WRITE setLINE_LENGTH) 
	Q_PROPERTY(qint32 FACTORY READ factory WRITE setFACTORY) 
	Q_PROPERTY(QString FEATURE READ feature WRITE setFEATURE) 
	Q_PROPERTY(qint32 NO READ no WRITE setNO) 
	Q_PROPERTY(QString DET_DATE READ det_date WRITE setDET_DATE) 
	Q_PROPERTY(QString PRODUCE_TIME READ produce_time WRITE setPRODUCE_TIME)
public:
    Q_INVOKABLE explicit DET_PRODUCE(QObject *parent = nullptr);
    Q_INVOKABLE DET_PRODUCE(const DET_PRODUCE &det_produce);
    Q_INVOKABLE void operator=(DET_PRODUCE det_produce);
    Q_INVOKABLE bool operator==(DET_PRODUCE &other);
    Q_INVOKABLE bool operator !=(DET_PRODUCE &other);
    
	Q_INVOKABLE qint32 id() const;
	Q_INVOKABLE void setID(const qint32& id);
	Q_INVOKABLE QString taskid() const;
	Q_INVOKABLE void setTASKID(const QString& taskid);
	Q_INVOKABLE QString codeprefix() const;
	Q_INVOKABLE void setCODEPREFIX(const QString& codeprefix);
	Q_INVOKABLE QString outcode() const;
	Q_INVOKABLE void setOUTCODE(const QString& outcode);
	Q_INVOKABLE QString barcode() const;
	Q_INVOKABLE void setBARCODE(const QString& barcode);
	Q_INVOKABLE QString incode() const;
	Q_INVOKABLE void setINCODE(const QString& incode);
	Q_INVOKABLE QString pwd() const;
	Q_INVOKABLE void setPWD(const QString& pwd);
	Q_INVOKABLE qint32 channel() const;
	Q_INVOKABLE void setCHANNEL(const qint32& channel);
	Q_INVOKABLE qint32 det_type() const;
	Q_INVOKABLE void setDET_TYPE(const qint32& det_type);
	Q_INVOKABLE qint32 status_code() const;
	Q_INVOKABLE void setSTATUS_CODE(const qint32& status_code);
	Q_INVOKABLE qint32 production_line_id() const;
	Q_INVOKABLE void setPRODUCTION_LINE_ID(const qint32& production_line_id);
	Q_INVOKABLE qint32 line_length() const;
	Q_INVOKABLE void setLINE_LENGTH(const qint32& line_length);
	Q_INVOKABLE qint32 factory() const;
	Q_INVOKABLE void setFACTORY(const qint32& factory);
	Q_INVOKABLE QString feature() const;
	Q_INVOKABLE void setFEATURE(const QString& feature);
	Q_INVOKABLE qint32 no() const;
	Q_INVOKABLE void setNO(const qint32& no);
	Q_INVOKABLE QString det_date() const;
	Q_INVOKABLE void setDET_DATE(const QString& det_date);
	Q_INVOKABLE QString produce_time() const;
	Q_INVOKABLE void setPRODUCE_TIME(const QString& produce_time);

    void initSql(const QString& dbName);
signals:
public slots:
private:
    
	qint32 m_id;
	QString m_taskid;
	QString m_codeprefix;
	QString m_outcode;
	QString m_barcode;
	QString m_incode;
	QString m_pwd;
	qint32 m_channel;
	qint32 m_det_type;
	qint32 m_status_code;
	qint32 m_production_line_id;
	qint32 m_line_length;
	qint32 m_factory;
	QString m_feature;
	qint32 m_no;
	QString m_det_date;
	QString m_produce_time;
};
} //namespace TriModel
Q_DECLARE_METATYPE(TriModel::DET_PRODUCE);
#endif // DET_PRODUCE_H
