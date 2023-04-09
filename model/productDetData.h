#ifndef productDetData_H
#define productDetData_H
/**
*┌──────────────────────────────────────────────────────────────┐
*│　描    述：
*│　作    者：hui.peng
*│　版    本：1.0    模板代码自动生成
*│　创建时间：2023-04-08 10:47:51
*└──────────────────────────────────────────────────────────────┘
*┌──────────────────────────────────────────────────────────────┐
*│　类    名： productDetData
*└──────────────────────────────────────────────────────────────┘
*/
#include <QObject>
namespace TriModel{
/**
 * @brief 
 */
class  productDetData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(qint32 id READ id WRITE setid)
    Q_PROPERTY(QString TASKID READ taskid WRITE setTASKID)
    Q_PROPERTY(QString CODEPREFIX READ codeprefix WRITE setCODEPREFIX)
    Q_PROPERTY(QString OUTCODE READ outcode WRITE setOUTCODE)
    Q_PROPERTY(QString encipherRange READ encipherrange WRITE setencipherRange)
    Q_PROPERTY(QString PRODUCE_TIME READ produce_time WRITE setPRODUCE_TIME)
    Q_PROPERTY(QString INCODE READ incode WRITE setINCODE)
    Q_PROPERTY(QString PWD READ pwd WRITE setPWD)
    Q_PROPERTY(QString STATUS_CODE READ status_code WRITE setSTATUS_CODE)
    Q_PROPERTY(qint32 assignmentNum READ assignmentnum WRITE setassignmentNum)
    Q_PROPERTY(qint32 accomplishNum READ accomplishnum WRITE setaccomplishNum)
    Q_PROPERTY(QString abnormalNum READ abnormalnum WRITE setabnormalNum)
    Q_PROPERTY(QString lotNum READ lotnum WRITE setlotNum)
    Q_PROPERTY(QString staff READ staff WRITE setstaff)
    Q_PROPERTY(QString produceComment READ producecomment WRITE setproduceComment)
    Q_PROPERTY(QString BARCODE READ barcode WRITE setBARCODE)
    Q_PROPERTY(QString DET_TYPE READ det_type WRITE setDET_TYPE)
    Q_PROPERTY(QString PRODUCTION_LINE_ID READ production_line_id WRITE setPRODUCTION_LINE_ID)
    Q_PROPERTY(QString LINE_LENGTH READ line_length WRITE setLINE_LENGTH)
    Q_PROPERTY(QString FACTORY READ factory WRITE setFACTORY)
    Q_PROPERTY(QString FEATURE READ feature WRITE setFEATURE)
    Q_PROPERTY(QString NO READ no WRITE setNO)
    Q_PROPERTY(QString CHANNEL READ channel WRITE setCHANNEL)
    Q_PROPERTY(QString DET_DATE READ det_date WRITE setDET_DATE)
    Q_PROPERTY(QString numberInBox READ numberinbox WRITE setnumberInBox)
    Q_PROPERTY(QString startBox READ startbox WRITE setstartBox)
    Q_PROPERTY(QString startNo READ startno WRITE setstartNo)
public:
    Q_INVOKABLE explicit productDetData(QObject *parent = nullptr);
    Q_INVOKABLE productDetData(const productDetData &productdetdata);
    Q_INVOKABLE void operator=(productDetData productdetdata);
    Q_INVOKABLE bool operator==(productDetData &other);
    Q_INVOKABLE bool operator !=(productDetData &other);

    Q_INVOKABLE qint32 id() const;
    Q_INVOKABLE void setid(const qint32& id);
    Q_INVOKABLE QString taskid() const;
    Q_INVOKABLE void setTASKID(const QString& taskid);
    Q_INVOKABLE QString codeprefix() const;
    Q_INVOKABLE void setCODEPREFIX(const QString& codeprefix);
    Q_INVOKABLE QString outcode() const;
    Q_INVOKABLE void setOUTCODE(const QString& outcode);
    Q_INVOKABLE QString encipherrange() const;
    Q_INVOKABLE void setencipherRange(const QString& encipherrange);
    Q_INVOKABLE QString produce_time() const;
    Q_INVOKABLE void setPRODUCE_TIME(const QString& produce_time);
    Q_INVOKABLE QString incode() const;
    Q_INVOKABLE void setINCODE(const QString& incode);
    Q_INVOKABLE QString pwd() const;
    Q_INVOKABLE void setPWD(const QString& pwd);
    Q_INVOKABLE QString status_code() const;
    Q_INVOKABLE void setSTATUS_CODE(const QString& status_code);
    Q_INVOKABLE qint32 assignmentnum() const;
    Q_INVOKABLE void setassignmentNum(const qint32& assignmentnum);
    Q_INVOKABLE qint32 accomplishnum() const;
    Q_INVOKABLE void setaccomplishNum(const qint32& accomplishnum);
    Q_INVOKABLE QString abnormalnum() const;
    Q_INVOKABLE void setabnormalNum(const QString& abnormalnum);
    Q_INVOKABLE QString lotnum() const;
    Q_INVOKABLE void setlotNum(const QString& lotnum);
    Q_INVOKABLE QString staff() const;
    Q_INVOKABLE void setstaff(const QString& staff);
    Q_INVOKABLE QString producecomment() const;
    Q_INVOKABLE void setproduceComment(const QString& producecomment);
    Q_INVOKABLE QString barcode() const;
    Q_INVOKABLE void setBARCODE(const QString& barcode);
    Q_INVOKABLE QString det_type() const;
    Q_INVOKABLE void setDET_TYPE(const QString& det_type);
    Q_INVOKABLE QString production_line_id() const;
    Q_INVOKABLE void setPRODUCTION_LINE_ID(const QString& production_line_id);
    Q_INVOKABLE QString line_length() const;
    Q_INVOKABLE void setLINE_LENGTH(const QString& line_length);
    Q_INVOKABLE QString factory() const;
    Q_INVOKABLE void setFACTORY(const QString& factory);
    Q_INVOKABLE QString feature() const;
    Q_INVOKABLE void setFEATURE(const QString& feature);
    Q_INVOKABLE QString no() const;
    Q_INVOKABLE void setNO(const QString& no);
    Q_INVOKABLE QString channel() const;
    Q_INVOKABLE void setCHANNEL(const QString& channel);
    Q_INVOKABLE QString det_date() const;
    Q_INVOKABLE void setDET_DATE(const QString& det_date);
    Q_INVOKABLE QString numberinbox() const;
    Q_INVOKABLE void setnumberInBox(const QString& numberinbox);
    Q_INVOKABLE QString startbox() const;
    Q_INVOKABLE void setstartBox(const QString& startbox);
    Q_INVOKABLE QString startno() const;
    Q_INVOKABLE void setstartNo(const QString& startno);
signals:
public slots:
private:

    qint32 m_id;
    QString m_taskid;
    QString m_codeprefix;
    QString m_outcode;
    QString m_encipherrange;
    QString m_produce_time;
    QString m_incode;
    QString m_pwd;
    QString m_status_code;
    qint32 m_assignmentnum;
    qint32 m_accomplishnum;
    QString m_abnormalnum;
    QString m_lotnum;
    QString m_staff;
    QString m_producecomment;
    QString m_barcode;
    QString m_det_type;
    QString m_production_line_id;
    QString m_line_length;
    QString m_factory;
    QString m_feature;
    QString m_no;
    QString m_channel;
    QString m_det_date;
    QString m_numberinbox;
    QString m_startbox;
    QString m_startno;
};
} //namespace TriModel
Q_DECLARE_METATYPE(TriModel::productDetData);
#endif // productDetData_H
